#include "include/QCPLayout.hpp"

namespace QCP {
/*! \class QCPLayout
  \brief The abstract base class for layouts

  This is an abstract base class for layout elements whose main purpose is to define the position
  and size of other child layout elements. In most cases, layouts don't draw anything themselves
  (but there are exceptions to this, e.g. QCPLegend).

  QCPLayout derives from QCPLayoutElement, and thus can itself be nested in other layouts.

  QCPLayout introduces a common interface for accessing and manipulating the child elements. Those
  functions are most notably \ref elementCount, \ref elementAt, \ref takeAt, \ref take, \ref
  simplify, \ref removeAt, \ref remove and \ref clear. Individual subclasses may add more functions
  to this interface which are more specialized to the form of the layout. For example, \ref
  QCPLayoutGrid adds functions that take row and column indices to access cells of the layout grid
  more conveniently.

  Since this is an abstract base class, you can't instantiate it directly. Rather use one of its
  subclasses like QCPLayoutGrid or QCPLayoutInset.

  For a general introduction to the layout system, see the dedicated documentation page \ref
  thelayoutsystem "The Layout System".
*/

/* start documentation of pure virtual functions */

/*! \fn virtual int QCPLayout::elementCount() const = 0

  Returns the number of elements/cells in the layout.

  \see elements, elementAt
*/

/*! \fn virtual QCPLayoutElement* QCPLayout::elementAt(int index) const = 0

  Returns the element in the cell with the given \a index. If \a index is invalid, returns \c
  nullptr.

  Note that even if \a index is valid, the respective cell may be empty in some layouts (e.g.
  QCPLayoutGrid), so this function may return \c nullptr in those cases. You may use this function
  to check whether a cell is empty or not.

  \see elements, elementCount, takeAt
*/

/*! \fn virtual QCPLayoutElement* QCPLayout::takeAt(int index) = 0

  Removes the element with the given \a index from the layout and returns it.

  If the \a index is invalid or the cell with that index is empty, returns \c nullptr.

  Note that some layouts don't remove the respective cell right away but leave an empty cell after
  successful removal of the layout element. To collapse empty cells, use \ref simplify.

  \see elementAt, take
*/

/*! \fn virtual bool QCPLayout::take(QCPLayoutElement* element) = 0

  Removes the specified \a element from the layout and returns true on success.

  If the \a element isn't in this layout, returns false.

  Note that some layouts don't remove the respective cell right away but leave an empty cell after
  successful removal of the layout element. To collapse empty cells, use \ref simplify.

  \see takeAt
*/

/* end documentation of pure virtual functions */

/*!
  Creates an instance of QCPLayout and sets default values. Note that since QCPLayout
  is an abstract base class, it can't be instantiated directly.
*/
    QCPLayout::QCPLayout()
    {
    }

/*!
  If \a phase is \ref upLayout, calls \ref updateLayout, which subclasses may reimplement to
  reposition and resize their cells.

  Finally, the call is propagated down to all child \ref QCPLayoutElement "QCPLayoutElements".

  For details about this method and the update phases, see the documentation of \ref
  QCPLayoutElement::update.
*/
    void QCPLayout::update(UpdatePhase phase)
    {
        QCPLayoutElement::update(phase);

// set child element rects according to layout:
        if (phase == upLayout)
            updateLayout();

// propagate update call to child elements:
        const int elCount = elementCount();
        for (int i=0; i<elCount; ++i)
        {
            if (QCPLayoutElement *el = elementAt(i))
                el->update(phase);
        }
    }

/* inherits documentation from base class */
    QList<QCPLayoutElement*> QCPLayout::elements(bool recursive) const
    {
        const int c = elementCount();
        QList<QCPLayoutElement*> result;
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
        result.reserve(c);
#endif
        for (int i=0; i<c; ++i)
            result.append(elementAt(i));
        if (recursive)
        {
            for (int i=0; i<c; ++i)
            {
                if (result.at(i))
                    result << result.at(i)->elements(recursive);
            }
        }
        return result;
    }

/*!
  Simplifies the layout by collapsing empty cells. The exact behavior depends on subclasses, the
  default implementation does nothing.

  Not all layouts need simplification. For example, QCPLayoutInset doesn't use explicit
  simplification while QCPLayoutGrid does.
*/
    void QCPLayout::simplify()
    {
    }

/*!
  Removes and deletes the element at the provided \a index. Returns true on success. If \a index is
  invalid or points to an empty cell, returns false.

  This function internally uses \ref takeAt to remove the element from the layout and then deletes
  the returned element. Note that some layouts don't remove the respective cell right away but leave an
  empty cell after successful removal of the layout element. To collapse empty cells, use \ref
  simplify.

  \see remove, takeAt
*/
    bool QCPLayout::removeAt(int index)
    {
        if (QCPLayoutElement *el = takeAt(index))
        {
            delete el;
            return true;
        } else
            return false;
    }

/*!
  Removes and deletes the provided \a element. Returns true on success. If \a element is not in the
  layout, returns false.

  This function internally uses \ref takeAt to remove the element from the layout and then deletes
  the element. Note that some layouts don't remove the respective cell right away but leave an
  empty cell after successful removal of the layout element. To collapse empty cells, use \ref
  simplify.

  \see removeAt, take
*/
    bool QCPLayout::remove(QCPLayoutElement *element)
    {
        if (take(element))
        {
            delete element;
            return true;
        } else
            return false;
    }

/*!
  Removes and deletes all layout elements in this layout. Finally calls \ref simplify to make sure
  all empty cells are collapsed.

  \see remove, removeAt
*/
    void QCPLayout::clear()
    {
        for (int i=elementCount()-1; i>=0; --i)
        {
            if (elementAt(i))
                removeAt(i);
        }
        simplify();
    }

/*!
  Subclasses call this method to report changed (minimum/maximum) size constraints.

  If the parent of this layout is again a QCPLayout, forwards the call to the parent's \ref
  sizeConstraintsChanged. If the parent is a QWidget (i.e. is the \ref QCustomPlot::plotLayout of
  QCustomPlot), calls QWidget::updateGeometry, so if the QCustomPlot widget is inside a Qt QLayout,
  it may update itself and resize cells accordingly.
*/
    void QCPLayout::sizeConstraintsChanged() const
    {
        if (QWidget *w = qobject_cast<QWidget*>(parent()))
            w->updateGeometry();
        else if (QCPLayout *l = qobject_cast<QCPLayout*>(parent()))
            l->sizeConstraintsChanged();
    }

/*! \internal

  Subclasses reimplement this method to update the position and sizes of the child elements/cells
  via calling their \ref QCPLayoutElement::setOuterRect. The default implementation does nothing.

  The geometry used as a reference is the inner \ref rect of this layout. Child elements should stay
  within that rect.

  \ref getSectionSizes may help with the reimplementation of this function.

  \see update
*/
    void QCPLayout::updateLayout()
    {
    }


/*! \internal

  Associates \a el with this layout. This is done by setting the \ref QCPLayoutElement::layout, the
  \ref QCPLayerable::parentLayerable and the QObject parent to this layout.

  Further, if \a el didn't previously have a parent plot, calls \ref
  QCPLayerable::initializeParentPlot on \a el to set the paret plot.

  This method is used by subclass specific methods that add elements to the layout. Note that this
  method only changes properties in \a el. The removal from the old layout and the insertion into
  the new layout must be done additionally.
*/
    void QCPLayout::adoptElement(QCPLayoutElement *el)
    {
        if (el)
        {
            el->mParentLayout = this;
            el->setParentLayerable(this);
            el->setParent(this);
            if (!el->parentPlot())
                el->initializeParentPlot(mParentPlot);
            el->layoutChanged();
        } else
            qDebug() << Q_FUNC_INFO << "Null element passed";
    }

/*! \internal

  Disassociates \a el from this layout. This is done by setting the \ref QCPLayoutElement::layout
  and the \ref QCPLayerable::parentLayerable to zero. The QObject parent is set to the parent
  QCustomPlot.

  This method is used by subclass specific methods that remove elements from the layout (e.g. \ref
  take or \ref takeAt). Note that this method only changes properties in \a el. The removal from
  the old layout must be done additionally.
*/
    void QCPLayout::releaseElement(QCPLayoutElement *el)
    {
        if (el)
        {
            el->mParentLayout = nullptr;
            el->setParentLayerable(nullptr);
            el->setParent(mParentPlot);
// Note: Don't initializeParentPlot(0) here, because layout element will stay in same parent plot
        } else
            qDebug() << Q_FUNC_INFO << "Null element passed";
    }

/*! \internal

  This is a helper function for the implementation of \ref updateLayout in subclasses.

  It calculates the sizes of one-dimensional sections with provided constraints on maximum section
  sizes, minimum section sizes, relative stretch factors and the final total size of all sections.

  The QVector entries refer to the sections. Thus all QVectors must have the same size.

  \a maxSizes gives the maximum allowed size of each section. If there shall be no maximum size
  imposed, set all vector values to Qt's QWIDGETSIZE_MAX.

  \a minSizes gives the minimum allowed size of each section. If there shall be no minimum size
  imposed, set all vector values to zero. If the \a minSizes entries add up to a value greater than
  \a totalSize, sections will be scaled smaller than the proposed minimum sizes. (In other words,
  not exceeding the allowed total size is taken to be more important than not going below minimum
  section sizes.)

  \a stretchFactors give the relative proportions of the sections to each other. If all sections
  shall be scaled equally, set all values equal. If the first section shall be double the size of
  each individual other section, set the first number of \a stretchFactors to double the value of
  the other individual values (e.g. {2, 1, 1, 1}).

  \a totalSize is the value that the final section sizes will add up to. Due to rounding, the
  actual sum may differ slightly. If you want the section sizes to sum up to exactly that value,
  you could distribute the remaining difference on the sections.

  The return value is a QVector containing the section sizes.
*/
    QVector<int> QCPLayout::getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const
    {
        if (maxSizes.size() != minSizes.size() || minSizes.size() != stretchFactors.size())
        {
            qDebug() << Q_FUNC_INFO << "Passed vector sizes aren't equal:" << maxSizes << minSizes << stretchFactors;
            return QVector<int>();
        }
        if (stretchFactors.isEmpty())
            return QVector<int>();
        int sectionCount = stretchFactors.size();
        QVector<double> sectionSizes(sectionCount);
// if provided total size is forced smaller than total minimum size, ignore minimum sizes (squeeze sections):
        int minSizeSum = 0;
        for (int i=0; i<sectionCount; ++i)
            minSizeSum += minSizes.at(i);
        if (totalSize < minSizeSum)
        {
// new stretch factors are minimum sizes and minimum sizes are set to zero:
            for (int i=0; i<sectionCount; ++i)
            {
                stretchFactors[i] = minSizes.at(i);
                minSizes[i] = 0;
            }
        }

        QList<int> minimumLockedSections;
        QList<int> unfinishedSections;
        for (int i=0; i<sectionCount; ++i)
            unfinishedSections.append(i);
        double freeSize = totalSize;

        int outerIterations = 0;
        while (!unfinishedSections.isEmpty() && outerIterations < sectionCount*2) // the iteration check ist just a failsafe in case something really strange happens
        {
            ++outerIterations;
            int innerIterations = 0;
            while (!unfinishedSections.isEmpty() && innerIterations < sectionCount*2) // the iteration check ist just a failsafe in case something really strange happens
            {
                ++innerIterations;
// find section that hits its maximum next:
                int nextId = -1;
                double nextMax = 1e12;
                foreach (int secId, unfinishedSections)
                {
                    double hitsMaxAt = (maxSizes.at(secId)-sectionSizes.at(secId))/stretchFactors.at(secId);
                    if (hitsMaxAt < nextMax)
                    {
                        nextMax = hitsMaxAt;
                        nextId = secId;
                    }
                }
// check if that maximum is actually within the bounds of the total size (i.e. can we stretch all remaining sections so far that the found section
// actually hits its maximum, without exceeding the total size when we add up all sections)
                double stretchFactorSum = 0;
                foreach (int secId, unfinishedSections)
                stretchFactorSum += stretchFactors.at(secId);
                double nextMaxLimit = freeSize/stretchFactorSum;
                if (nextMax < nextMaxLimit) // next maximum is actually hit, move forward to that point and fix the size of that section
                {
                    foreach (int secId, unfinishedSections)
                    {
                        sectionSizes[secId] += nextMax*stretchFactors.at(secId); // increment all sections
                        freeSize -= nextMax*stretchFactors.at(secId);
                    }
                    unfinishedSections.removeOne(nextId); // exclude the section that is now at maximum from further changes
                } else // next maximum isn't hit, just distribute rest of free space on remaining sections
                {
                    foreach (int secId, unfinishedSections)
                    sectionSizes[secId] += nextMaxLimit*stretchFactors.at(secId); // increment all sections
                    unfinishedSections.clear();
                }
            }
            if (innerIterations == sectionCount*2)
                qDebug() << Q_FUNC_INFO << "Exceeded maximum expected inner iteration count, layouting aborted. Input was:" << maxSizes << minSizes << stretchFactors << totalSize;

// now check whether the resulting section sizes violate minimum restrictions:
            bool foundMinimumViolation = false;
            for (int i=0; i<sectionSizes.size(); ++i)
            {
                if (minimumLockedSections.contains(i))
                    continue;
                if (sectionSizes.at(i) < minSizes.at(i)) // section violates minimum
                {
                    sectionSizes[i] = minSizes.at(i); // set it to minimum
                    foundMinimumViolation = true; // make sure we repeat the whole optimization process
                    minimumLockedSections.append(i);
                }
            }
            if (foundMinimumViolation)
            {
                freeSize = totalSize;
                for (int i=0; i<sectionCount; ++i)
                {
                    if (!minimumLockedSections.contains(i)) // only put sections that haven't hit their minimum back into the pool
                        unfinishedSections.append(i);
                    else
                        freeSize -= sectionSizes.at(i); // remove size of minimum locked sections from available space in next round
                }
// reset all section sizes to zero that are in unfinished sections (all others have been set to their minimum):
                foreach (int secId, unfinishedSections)
                sectionSizes[secId] = 0;
            }
        }
        if (outerIterations == sectionCount*2)
            qDebug() << Q_FUNC_INFO << "Exceeded maximum expected outer iteration count, layouting aborted. Input was:" << maxSizes << minSizes << stretchFactors << totalSize;

        QVector<int> result(sectionCount);
        for (int i=0; i<sectionCount; ++i)
            result[i] = qRound(sectionSizes.at(i));
        return result;
    }

/*! \internal

  This is a helper function for the implementation of subclasses.

  It returns the minimum size that should finally be used for the outer rect of the passed layout
  element \a el.

  It takes into account whether a manual minimum size is set (\ref
  QCPLayoutElement::setMinimumSize), which size constraint is set (\ref
  QCPLayoutElement::setSizeConstraintRect), as well as the minimum size hint, if no manual minimum
  size was set (\ref QCPLayoutElement::minimumOuterSizeHint).
*/
    QSize QCPLayout::getFinalMinimumOuterSize(const QCPLayoutElement *el)
    {
        QSize minOuterHint = el->minimumOuterSizeHint();
        QSize minOuter = el->minimumSize(); // depending on sizeConstraitRect this might be with respect to inner rect, so possibly add margins in next four lines (preserving unset minimum of 0)
        if (minOuter.width() > 0 && el->sizeConstraintRect() == QCPLayoutElement::scrInnerRect)
            minOuter.rwidth() += el->margins().left() + el->margins().right();
        if (minOuter.height() > 0 && el->sizeConstraintRect() == QCPLayoutElement::scrInnerRect)
            minOuter.rheight() += el->margins().top() + el->margins().bottom();

        return {minOuter.width() > 0 ? minOuter.width() : minOuterHint.width(),
                minOuter.height() > 0 ? minOuter.height() : minOuterHint.height()};
    }

/*! \internal

  This is a helper function for the implementation of subclasses.

  It returns the maximum size that should finally be used for the outer rect of the passed layout
  element \a el.

  It takes into account whether a manual maximum size is set (\ref
  QCPLayoutElement::setMaximumSize), which size constraint is set (\ref
  QCPLayoutElement::setSizeConstraintRect), as well as the maximum size hint, if no manual maximum
  size was set (\ref QCPLayoutElement::maximumOuterSizeHint).
*/
    QSize QCPLayout::getFinalMaximumOuterSize(const QCPLayoutElement *el)
    {
        QSize maxOuterHint = el->maximumOuterSizeHint();
        QSize maxOuter = el->maximumSize(); // depending on sizeConstraitRect this might be with respect to inner rect, so possibly add margins in next four lines (preserving unset maximum of QWIDGETSIZE_MAX)
        if (maxOuter.width() < QWIDGETSIZE_MAX && el->sizeConstraintRect() == QCPLayoutElement::scrInnerRect)
            maxOuter.rwidth() += el->margins().left() + el->margins().right();
        if (maxOuter.height() < QWIDGETSIZE_MAX && el->sizeConstraintRect() == QCPLayoutElement::scrInnerRect)
            maxOuter.rheight() += el->margins().top() + el->margins().bottom();

        return {maxOuter.width() < QWIDGETSIZE_MAX ? maxOuter.width() : maxOuterHint.width(),
                maxOuter.height() < QWIDGETSIZE_MAX ? maxOuter.height() : maxOuterHint.height()};
    }

} // QCP