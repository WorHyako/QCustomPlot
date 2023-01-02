#include "include/QCPLayoutElement.hpp"

namespace QCP {
/*! \class QCPLayoutElement
  \brief The abstract base class for all objects that form \ref thelayoutsystem "the layout system".

  This is an abstract base class. As such, it can't be instantiated directly, rather use one of its subclasses.

  A Layout element is a rectangular object which can be placed in layouts. It has an outer rect
  (QCPLayoutElement::outerRect) and an inner rect (\ref QCPLayoutElement::rect). The difference
  between outer and inner rect is called its margin. The margin can either be set to automatic or
  manual (\ref setAutoMargins) on a per-side basis. If a side is set to manual, that margin can be
  set explicitly with \ref setMargins and will stay fixed at that value. If it's set to automatic,
  the layout element subclass will control the value itself (via \ref calculateAutoMargin).

  Layout elements can be placed in layouts (base class QCPLayout) like QCPLayoutGrid. The top level
  layout is reachable via \ref QCustomPlot::plotLayout, and is a \ref QCPLayoutGrid. Since \ref
  QCPLayout itself derives from \ref QCPLayoutElement, layouts can be nested.

  Thus in QCustomPlot one can divide layout elements into two categories: The ones that are
  invisible by themselves, because they don't draw anything. Their only purpose is to manage the
  position and size of other layout elements. This category of layout elements usually use
  QCPLayout as base class. Then there is the category of layout elements which actually draw
  something. For example, QCPAxisRect, QCPLegend and QCPTextElement are of this category. This does
  not necessarily mean that the latter category can't have child layout elements. QCPLegend for
  instance, actually derives from QCPLayoutGrid and the individual legend items are child layout
  elements in the grid layout.
*/

/* start documentation of inline functions */

/*! \fn QCPLayout *QCPLayoutElement::layout() const

  Returns the parent layout of this layout element.
*/

/*! \fn QRect QCPLayoutElement::rect() const

  Returns the inner rect of this layout element. The inner rect is the outer rect (\ref outerRect, \ref
  setOuterRect) shrinked by the margins (\ref setMargins, \ref setAutoMargins).

  In some cases, the area between outer and inner rect is left blank. In other cases the margin
  area is used to display peripheral graphics while the main content is in the inner rect. This is
  where automatic margin calculation becomes interesting because it allows the layout element to
  adapt the margins to the peripheral graphics it wants to draw. For example, \ref QCPAxisRect
  draws the axis labels and tick labels in the margin area, thus needs to adjust the margins (if
  \ref setAutoMargins is enabled) according to the space required by the labels of the axes.

  \see outerRect
*/

/*! \fn QRect QCPLayoutElement::outerRect() const

  Returns the outer rect of this layout element. The outer rect is the inner rect expanded by the
  margins (\ref setMargins, \ref setAutoMargins). The outer rect is used (and set via \ref
  setOuterRect) by the parent \ref QCPLayout to control the size of this layout element.

  \see rect
*/

/* end documentation of inline functions */

/*!
  Creates an instance of QCPLayoutElement and sets default values.
*/
    QCPLayoutElement::QCPLayoutElement(QCustomPlot *parentPlot) :
    QCPLayerable(parentPlot), // parenthood is changed as soon as layout element gets inserted into a layout (except for top level layout)
    mParentLayout(nullptr),
    mMinimumSize(),
    mMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX),
    mSizeConstraintRect(scrInnerRect),
    mRect(0, 0, 0, 0),
    mOuterRect(0, 0, 0, 0),
    mMargins(0, 0, 0, 0),
    mMinimumMargins(0, 0, 0, 0),
    mAutoMargins(QCP::msAll)
    {
    }

    QCPLayoutElement::~QCPLayoutElement()
    {
        setMarginGroup(QCP::msAll, nullptr); // unregister at margin groups, if there are any
// unregister at layout:
        if (qobject_cast<QCPLayout*>(mParentLayout)) // the qobject_cast is just a safeguard in case the layout forgets to call clear() in its dtor and this dtor is called by QObject dtor
            mParentLayout->take(this);
    }

/*!
  Sets the outer rect of this layout element. If the layout element is inside a layout, the layout
  sets the position and size of this layout element using this function.

  Calling this function externally has no effect, since the layout will overwrite any changes to
  the outer rect upon the next replot.

  The layout element will adapt its inner \ref rect by applying the margins inward to the outer rect.

  \see rect
*/
    void QCPLayoutElement::setOuterRect(const QRect &rect)
    {
        if (mOuterRect != rect)
        {
            mOuterRect = rect;
            mRect = mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(), -mMargins.bottom());
        }
    }

/*!
  Sets the margins of this layout element. If \ref setAutoMargins is disabled for some or all
  sides, this function is used to manually set the margin on those sides. Sides that are still set
  to be handled automatically are ignored and may have any value in \a margins.

  The margin is the distance between the outer rect (controlled by the parent layout via \ref
  setOuterRect) and the inner \ref rect (which usually contains the main content of this layout
  element).

  \see setAutoMargins
*/
    void QCPLayoutElement::setMargins(const QMargins &margins)
    {
        if (mMargins != margins)
        {
            mMargins = margins;
            mRect = mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(), -mMargins.bottom());
        }
    }

/*!
  If \ref setAutoMargins is enabled on some or all margins, this function is used to provide
  minimum values for those margins.

  The minimum values are not enforced on margin sides that were set to be under manual control via
  \ref setAutoMargins.

  \see setAutoMargins
*/
    void QCPLayoutElement::setMinimumMargins(const QMargins &margins)
    {
        if (mMinimumMargins != margins)
        {
            mMinimumMargins = margins;
        }
    }

/*!
  Sets on which sides the margin shall be calculated automatically. If a side is calculated
  automatically, a minimum margin value may be provided with \ref setMinimumMargins. If a side is
  set to be controlled manually, the value may be specified with \ref setMargins.

  Margin sides that are under automatic control may participate in a \ref QCPMarginGroup (see \ref
  setMarginGroup), to synchronize (align) it with other layout elements in the plot.

  \see setMinimumMargins, setMargins, QCP::MarginSide
*/
    void QCPLayoutElement::setAutoMargins(QCP::MarginSides sides)
    {
        mAutoMargins = sides;
    }

/*!
  Sets the minimum size of this layout element. A parent layout tries to respect the \a size here
  by changing row/column sizes in the layout accordingly.

  If the parent layout size is not sufficient to satisfy all minimum size constraints of its child
  layout elements, the layout may set a size that is actually smaller than \a size. QCustomPlot
  propagates the layout's size constraints to the outside by setting its own minimum QWidget size
  accordingly, so violations of \a size should be exceptions.

  Whether this constraint applies to the inner or the outer rect can be specified with \ref
  setSizeConstraintRect (see \ref rect and \ref outerRect).
*/
    void QCPLayoutElement::setMinimumSize(const QSize &size)
    {
        if (mMinimumSize != size)
        {
            mMinimumSize = size;
            if (mParentLayout)
                mParentLayout->sizeConstraintsChanged();
        }
    }

/*! \overload

  Sets the minimum size of this layout element.

  Whether this constraint applies to the inner or the outer rect can be specified with \ref
  setSizeConstraintRect (see \ref rect and \ref outerRect).
*/
    void QCPLayoutElement::setMinimumSize(int width, int height)
    {
        setMinimumSize(QSize(width, height));
    }

/*!
  Sets the maximum size of this layout element. A parent layout tries to respect the \a size here
  by changing row/column sizes in the layout accordingly.

  Whether this constraint applies to the inner or the outer rect can be specified with \ref
  setSizeConstraintRect (see \ref rect and \ref outerRect).
*/
    void QCPLayoutElement::setMaximumSize(const QSize &size)
    {
        if (mMaximumSize != size)
        {
            mMaximumSize = size;
            if (mParentLayout)
                mParentLayout->sizeConstraintsChanged();
        }
    }

/*! \overload

  Sets the maximum size of this layout element.

  Whether this constraint applies to the inner or the outer rect can be specified with \ref
  setSizeConstraintRect (see \ref rect and \ref outerRect).
*/
    void QCPLayoutElement::setMaximumSize(int width, int height)
    {
        setMaximumSize(QSize(width, height));
    }

/*!
  Sets to which rect of a layout element the size constraints apply. Size constraints can be set
  via \ref setMinimumSize and \ref setMaximumSize.

  The outer rect (\ref outerRect) includes the margins (e.g. in the case of a QCPAxisRect the axis
  labels), whereas the inner rect (\ref rect) does not.

  \see setMinimumSize, setMaximumSize
*/
    void QCPLayoutElement::setSizeConstraintRect(SizeConstraintRect constraintRect)
    {
        if (mSizeConstraintRect != constraintRect)
        {
            mSizeConstraintRect = constraintRect;
            if (mParentLayout)
                mParentLayout->sizeConstraintsChanged();
        }
    }

/*!
  Sets the margin \a group of the specified margin \a sides.

  Margin groups allow synchronizing specified margins across layout elements, see the documentation
  of \ref QCPMarginGroup.

  To unset the margin group of \a sides, set \a group to \c nullptr.

  Note that margin groups only work for margin sides that are set to automatic (\ref
  setAutoMargins).

  \see QCP::MarginSide
*/
    void QCPLayoutElement::setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group)
    {
        QVector<QCP::MarginSide> sideVector;
        if (sides.testFlag(QCP::msLeft)) sideVector.append(QCP::msLeft);
        if (sides.testFlag(QCP::msRight)) sideVector.append(QCP::msRight);
        if (sides.testFlag(QCP::msTop)) sideVector.append(QCP::msTop);
        if (sides.testFlag(QCP::msBottom)) sideVector.append(QCP::msBottom);

        foreach (QCP::MarginSide side, sideVector)
        {
            if (marginGroup(side) != group)
            {
                QCPMarginGroup *oldGroup = marginGroup(side);
                if (oldGroup) // unregister at old group
                    oldGroup->removeChild(side, this);

                if (!group) // if setting to 0, remove hash entry. Else set hash entry to new group and register there
                {
                    mMarginGroups.remove(side);
                } else // setting to a new group
                {
                    mMarginGroups[side] = group;
                    group->addChild(side, this);
                }
            }
        }
    }

/*!
  Updates the layout element and sub-elements. This function is automatically called before every
  replot by the parent layout element. It is called multiple times, once for every \ref
  UpdatePhase. The phases are run through in the order of the enum values. For details about what
  happens at the different phases, see the documentation of \ref UpdatePhase.

  Layout elements that have child elements should call the \ref update method of their child
  elements, and pass the current \a phase unchanged.

  The default implementation executes the automatic margin mechanism in the \ref upMargins phase.
  Subclasses should make sure to call the base class implementation.
*/
    void QCPLayoutElement::update(UpdatePhase phase)
    {
        if (phase == upMargins)
        {
            if (mAutoMargins != QCP::msNone)
            {
// set the margins of this layout element according to automatic margin calculation, either directly or via a margin group:
                QMargins newMargins = mMargins;
                const QList<QCP::MarginSide> allMarginSides = QList<QCP::MarginSide>() << QCP::msLeft << QCP::msRight << QCP::msTop << QCP::msBottom;
                foreach (QCP::MarginSide side, allMarginSides)
                {
                    if (mAutoMargins.testFlag(side)) // this side's margin shall be calculated automatically
                    {
                        if (mMarginGroups.contains(side))
                            QCP::setMarginValue(newMargins, side, mMarginGroups[side]->commonMargin(side)); // this side is part of a margin group, so get the margin value from that group
                        else
                            QCP::setMarginValue(newMargins, side, calculateAutoMargin(side)); // this side is not part of a group, so calculate the value directly
// apply minimum margin restrictions:
                        if (QCP::getMarginValue(newMargins, side) < QCP::getMarginValue(mMinimumMargins, side))
                            QCP::setMarginValue(newMargins, side, QCP::getMarginValue(mMinimumMargins, side));
                    }
                }
                setMargins(newMargins);
            }
        }
    }

/*!
  Returns the suggested minimum size this layout element (the \ref outerRect) may be compressed to,
  if no manual minimum size is set.

  if a minimum size (\ref setMinimumSize) was not set manually, parent layouts use the returned size
  (usually indirectly through \ref QCPLayout::getFinalMinimumOuterSize) to determine the minimum
  allowed size of this layout element.

  A manual minimum size is considered set if it is non-zero.

  The default implementation simply returns the sum of the horizontal margins for the width and the
  sum of the vertical margins for the height. Reimplementations may use their detailed knowledge
  about the layout element's content to provide size hints.
*/
    QSize QCPLayoutElement::minimumOuterSizeHint() const
    {
        return {mMargins.left()+mMargins.right(), mMargins.top()+mMargins.bottom()};
    }

/*!
  Returns the suggested maximum size this layout element (the \ref outerRect) may be expanded to,
  if no manual maximum size is set.

  if a maximum size (\ref setMaximumSize) was not set manually, parent layouts use the returned
  size (usually indirectly through \ref QCPLayout::getFinalMaximumOuterSize) to determine the
  maximum allowed size of this layout element.

  A manual maximum size is considered set if it is smaller than Qt's \c QWIDGETSIZE_MAX.

  The default implementation simply returns \c QWIDGETSIZE_MAX for both width and height, implying
  no suggested maximum size. Reimplementations may use their detailed knowledge about the layout
  element's content to provide size hints.
*/
    QSize QCPLayoutElement::maximumOuterSizeHint() const
    {
        return {QWIDGETSIZE_MAX, QWIDGETSIZE_MAX};
    }

/*!
  Returns a list of all child elements in this layout element. If \a recursive is true, all
  sub-child elements are included in the list, too.

  \warning There may be \c nullptr entries in the returned list. For example, QCPLayoutGrid may
  have empty cells which yield \c nullptr at the respective index.
*/
    QList<QCPLayoutElement*> QCPLayoutElement::elements(bool recursive) const
    {
        Q_UNUSED(recursive)
        return QList<QCPLayoutElement*>();
    }

/*!
  Layout elements are sensitive to events inside their outer rect. If \a pos is within the outer
  rect, this method returns a value corresponding to 0.99 times the parent plot's selection
  tolerance. However, layout elements are not selectable by default. So if \a onlySelectable is
  true, -1.0 is returned.

  See \ref QCPLayerable::selectTest for a general explanation of this virtual method.

  QCPLayoutElement subclasses may reimplement this method to provide more specific selection test
  behaviour.
*/
    double QCPLayoutElement::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
    {
        Q_UNUSED(details)

        if (onlySelectable)
            return -1;

        if (QRectF(mOuterRect).contains(pos))
        {
            if (mParentPlot)
                return mParentPlot->selectionTolerance()*0.99;
            else
            {
                qDebug() << Q_FUNC_INFO << "parent plot not defined";
                return -1;
            }
        } else
            return -1;
    }

/*! \internal

  propagates the parent plot initialization to all child elements, by calling \ref
  QCPLayerable::initializeParentPlot on them.
*/
    void QCPLayoutElement::parentPlotInitialized(QCustomPlot *parentPlot)
    {
        foreach (QCPLayoutElement *el, elements(false))
        {
            if (!el->parentPlot())
                el->initializeParentPlot(parentPlot);
        }
    }

/*! \internal

  Returns the margin size for this \a side. It is used if automatic margins is enabled for this \a
  side (see \ref setAutoMargins). If a minimum margin was set with \ref setMinimumMargins, the
  returned value will not be smaller than the specified minimum margin.

  The default implementation just returns the respective manual margin (\ref setMargins) or the
  minimum margin, whichever is larger.
*/
    int QCPLayoutElement::calculateAutoMargin(QCP::MarginSide side)
    {
        return qMax(QCP::getMarginValue(mMargins, side), QCP::getMarginValue(mMinimumMargins, side));
    }

/*! \internal

  This virtual method is called when this layout element was moved to a different QCPLayout, or
  when this layout element has changed its logical position (e.g. row and/or column) within the
  same QCPLayout. Subclasses may use this to react accordingly.

  Since this method is called after the completion of the move, you can access the new parent
  layout via \ref layout().

  The default implementation does nothing.
*/
    void QCPLayoutElement::layoutChanged()
    {
    }

} // QCP