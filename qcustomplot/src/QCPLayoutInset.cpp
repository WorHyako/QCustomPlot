#include "include/QCPLayoutInset.hpp"

namespace QCP {
/*! \class QCPLayoutInset
  \brief A layout that places child elements aligned to the border or arbitrarily positioned

  Elements are placed either aligned to the border or at arbitrary position in the area of the
  layout. Which placement applies is controlled with the \ref InsetPlacement (\ref
  setInsetPlacement).

  Elements are added via \ref addElement(QCPLayoutElement *element, Qt::Alignment alignment) or
  addElement(QCPLayoutElement *element, const QRectF &rect). If the first method is used, the inset
  placement will default to \ref ipBorderAligned and the element will be aligned according to the
  \a alignment parameter. The second method defaults to \ref ipFree and allows placing elements at
  arbitrary position and size, defined by \a rect.

  The alignment or rect can be set via \ref setInsetAlignment or \ref setInsetRect, respectively.

  This is the layout that every QCPAxisRect has as \ref QCPAxisRect::insetLayout.
*/

/* start documentation of inline functions */

/*! \fn virtual void QCPLayoutInset::simplify()

  The QCPInsetLayout does not need simplification since it can never have empty cells due to its
  linear index structure. This method does nothing.
*/

/* end documentation of inline functions */

/*!
  Creates an instance of QCPLayoutInset and sets default values.
*/
    QCPLayoutInset::QCPLayoutInset()
    {
    }

    QCPLayoutInset::~QCPLayoutInset()
    {
// clear all child layout elements. This is important because only the specific layouts know how
// to handle removing elements (clear calls virtual removeAt method to do that).
        clear();
    }

/*!
  Returns the placement type of the element with the specified \a index.
*/
    QCPLayoutInset::InsetPlacement QCPLayoutInset::insetPlacement(int index) const
    {
        if (elementAt(index))
            return mInsetPlacement.at(index);
        else
        {
            qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
            return ipFree;
        }
    }

/*!
  Returns the alignment of the element with the specified \a index. The alignment only has a
  meaning, if the inset placement (\ref setInsetPlacement) is \ref ipBorderAligned.
*/
    Qt::Alignment QCPLayoutInset::insetAlignment(int index) const
    {
        if (elementAt(index))
            return mInsetAlignment.at(index);
        else
        {
            qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
#if QT_VERSION < QT_VERSION_CHECK(5, 2, 0)
            return nullptr;
#else
            return {};
#endif
        }
    }

/*!
  Returns the rect of the element with the specified \a index. The rect only has a
  meaning, if the inset placement (\ref setInsetPlacement) is \ref ipFree.
*/
    QRectF QCPLayoutInset::insetRect(int index) const
    {
        if (elementAt(index))
            return mInsetRect.at(index);
        else
        {
            qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
            return {};
        }
    }

/*!
  Sets the inset placement type of the element with the specified \a index to \a placement.

  \see InsetPlacement
*/
    void QCPLayoutInset::setInsetPlacement(int index, QCPLayoutInset::InsetPlacement placement)
    {
        if (elementAt(index))
            mInsetPlacement[index] = placement;
        else
            qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    }

/*!
  If the inset placement (\ref setInsetPlacement) is \ref ipBorderAligned, this function
  is used to set the alignment of the element with the specified \a index to \a alignment.

  \a alignment is an or combination of the following alignment flags: Qt::AlignLeft,
  Qt::AlignHCenter, Qt::AlighRight, Qt::AlignTop, Qt::AlignVCenter, Qt::AlignBottom. Any other
  alignment flags will be ignored.
*/
    void QCPLayoutInset::setInsetAlignment(int index, Qt::Alignment alignment)
    {
        if (elementAt(index))
            mInsetAlignment[index] = alignment;
        else
            qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    }

/*!
  If the inset placement (\ref setInsetPlacement) is \ref ipFree, this function is used to set the
  position and size of the element with the specified \a index to \a rect.

  \a rect is given in fractions of the whole inset layout rect. So an inset with rect (0, 0, 1, 1)
  will span the entire layout. An inset with rect (0.6, 0.1, 0.35, 0.35) will be in the top right
  corner of the layout, with 35% width and height of the parent layout.

  Note that the minimum and maximum sizes of the embedded element (\ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize) are enforced.
*/
    void QCPLayoutInset::setInsetRect(int index, const QRectF &rect)
    {
        if (elementAt(index))
            mInsetRect[index] = rect;
        else
            qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    }

/* inherits documentation from base class */
    void QCPLayoutInset::updateLayout()
    {
        for (int i=0; i<mElements.size(); ++i)
        {
            QCPLayoutElement *el = mElements.at(i);
            QRect insetRect;
            QSize finalMinSize = getFinalMinimumOuterSize(el);
            QSize finalMaxSize = getFinalMaximumOuterSize(el);
            if (mInsetPlacement.at(i) == ipFree)
            {
                insetRect = QRect(int( rect().x()+rect().width()*mInsetRect.at(i).x() ),
                                  int( rect().y()+rect().height()*mInsetRect.at(i).y() ),
                                  int( rect().width()*mInsetRect.at(i).width() ),
                                  int( rect().height()*mInsetRect.at(i).height() ));
                if (insetRect.size().width() < finalMinSize.width())
                    insetRect.setWidth(finalMinSize.width());
                if (insetRect.size().height() < finalMinSize.height())
                    insetRect.setHeight(finalMinSize.height());
                if (insetRect.size().width() > finalMaxSize.width())
                    insetRect.setWidth(finalMaxSize.width());
                if (insetRect.size().height() > finalMaxSize.height())
                    insetRect.setHeight(finalMaxSize.height());
            } else if (mInsetPlacement.at(i) == ipBorderAligned)
            {
                insetRect.setSize(finalMinSize);
                Qt::Alignment al = mInsetAlignment.at(i);
                if (al.testFlag(Qt::AlignLeft)) insetRect.moveLeft(rect().x());
                else if (al.testFlag(Qt::AlignRight)) insetRect.moveRight(rect().x()+rect().width());
                else insetRect.moveLeft(int( rect().x()+rect().width()*0.5-finalMinSize.width()*0.5 )); // default to Qt::AlignHCenter
                if (al.testFlag(Qt::AlignTop)) insetRect.moveTop(rect().y());
                else if (al.testFlag(Qt::AlignBottom)) insetRect.moveBottom(rect().y()+rect().height());
                else insetRect.moveTop(int( rect().y()+rect().height()*0.5-finalMinSize.height()*0.5 )); // default to Qt::AlignVCenter
            }
            mElements.at(i)->setOuterRect(insetRect);
        }
    }

/* inherits documentation from base class */
    int QCPLayoutInset::elementCount() const
    {
        return mElements.size();
    }

/* inherits documentation from base class */
    QCPLayoutElement *QCPLayoutInset::elementAt(int index) const
    {
        if (index >= 0 && index < mElements.size())
            return mElements.at(index);
        else
            return nullptr;
    }

/* inherits documentation from base class */
    QCPLayoutElement *QCPLayoutInset::takeAt(int index)
    {
        if (QCPLayoutElement *el = elementAt(index))
        {
            releaseElement(el);
            mElements.removeAt(index);
            mInsetPlacement.removeAt(index);
            mInsetAlignment.removeAt(index);
            mInsetRect.removeAt(index);
            return el;
        } else
        {
            qDebug() << Q_FUNC_INFO << "Attempt to take invalid index:" << index;
            return nullptr;
        }
    }

/* inherits documentation from base class */
    bool QCPLayoutInset::take(QCPLayoutElement *element)
    {
        if (element)
        {
            for (int i=0; i<elementCount(); ++i)
            {
                if (elementAt(i) == element)
                {
                    takeAt(i);
                    return true;
                }
            }
            qDebug() << Q_FUNC_INFO << "Element not in this layout, couldn't take";
        } else
            qDebug() << Q_FUNC_INFO << "Can't take nullptr element";
        return false;
    }

/*!
  The inset layout is sensitive to events only at areas where its (visible) child elements are
  sensitive. If the selectTest method of any of the child elements returns a positive number for \a
  pos, this method returns a value corresponding to 0.99 times the parent plot's selection
  tolerance. The inset layout is not selectable itself by default. So if \a onlySelectable is true,
  -1.0 is returned.

  See \ref QCPLayerable::selectTest for a general explanation of this virtual method.
*/
    double QCPLayoutInset::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
    {
        Q_UNUSED(details)
        if (onlySelectable)
            return -1;

        foreach (QCPLayoutElement *el, mElements)
        {
// inset layout shall only return positive selectTest, if actually an inset object is at pos
// else it would block the entire underlying QCPAxisRect with its surface.
            if (el->realVisibility() && el->selectTest(pos, onlySelectable) >= 0)
                return mParentPlot->selectionTolerance()*0.99;
        }
        return -1;
    }

/*!
  Adds the specified \a element to the layout as an inset aligned at the border (\ref
  setInsetAlignment is initialized with \ref ipBorderAligned). The alignment is set to \a
  alignment.

  \a alignment is an or combination of the following alignment flags: Qt::AlignLeft,
  Qt::AlignHCenter, Qt::AlighRight, Qt::AlignTop, Qt::AlignVCenter, Qt::AlignBottom. Any other
  alignment flags will be ignored.

  \see addElement(QCPLayoutElement *element, const QRectF &rect)
*/
    void QCPLayoutInset::addElement(QCPLayoutElement *element, Qt::Alignment alignment)
    {
        if (element)
        {
            if (element->layout()) // remove from old layout first
                element->layout()->take(element);
            mElements.append(element);
            mInsetPlacement.append(ipBorderAligned);
            mInsetAlignment.append(alignment);
            mInsetRect.append(QRectF(0.6, 0.6, 0.4, 0.4));
            adoptElement(element);
        } else
            qDebug() << Q_FUNC_INFO << "Can't add nullptr element";
    }

/*!
  Adds the specified \a element to the layout as an inset with free positioning/sizing (\ref
  setInsetAlignment is initialized with \ref ipFree). The position and size is set to \a
  rect.

  \a rect is given in fractions of the whole inset layout rect. So an inset with rect (0, 0, 1, 1)
  will span the entire layout. An inset with rect (0.6, 0.1, 0.35, 0.35) will be in the top right
  corner of the layout, with 35% width and height of the parent layout.

  \see addElement(QCPLayoutElement *element, Qt::Alignment alignment)
*/
    void QCPLayoutInset::addElement(QCPLayoutElement *element, const QRectF &rect)
    {
        if (element)
        {
            if (element->layout()) // remove from old layout first
                element->layout()->take(element);
            mElements.append(element);
            mInsetPlacement.append(ipFree);
            mInsetAlignment.append(Qt::AlignRight|Qt::AlignTop);
            mInsetRect.append(rect);
            adoptElement(element);
        } else
            qDebug() << Q_FUNC_INFO << "Can't add nullptr element";
    }
/* end of 'src/layout.cpp' */


/* including file 'src/lineending.cpp'      */
/* modified 2022-11-06T12:45:56, size 11189 */

} // QCP