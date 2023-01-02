#include "include/QCPMarginGroup.hpp"

namespace QCP {
/*! \class QCPMarginGroup
  \brief A margin group allows synchronization of margin sides if working with multiple layout elements.

  QCPMarginGroup allows you to tie a margin side of two or more layout elements together, such that
  they will all have the same size, based on the largest required margin in the group.

  \n
  \image html QCPMarginGroup.png "Demonstration of QCPMarginGroup"
  \n

  In certain situations it is desirable that margins at specific sides are synchronized across
  layout elements. For example, if one QCPAxisRect is below another one in a grid layout, it will
  provide a cleaner look to the user if the left and right margins of the two axis rects are of the
  same size. The left axis of the top axis rect will then be at the same horizontal position as the
  left axis of the lower axis rect, making them appear aligned. The same applies for the right
  axes. This is what QCPMarginGroup makes possible.

  To add/remove a specific side of a layout element to/from a margin group, use the \ref
  QCPLayoutElement::setMarginGroup method. To completely break apart the margin group, either call
  \ref clear, or just delete the margin group.

  \section QCPMarginGroup-example Example

  First create a margin group:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpmargingroup-creation-1
  Then set this group on the layout element sides:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpmargingroup-creation-2
  Here, we've used the first two axis rects of the plot and synchronized their left margins with
  each other and their right margins with each other.
*/

/* start documentation of inline functions */

/*! \fn QList<QCPLayoutElement*> QCPMarginGroup::elements(QCP::MarginSide side) const

  Returns a list of all layout elements that have their margin \a side associated with this margin
  group.
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPMarginGroup instance in \a parentPlot.
*/
    QCPMarginGroup::QCPMarginGroup(QCustomPlot *parentPlot) :
    QObject(parentPlot),
    mParentPlot(parentPlot)
    {
        mChildren.insert(QCP::msLeft, QList<QCPLayoutElement*>());
        mChildren.insert(QCP::msRight, QList<QCPLayoutElement*>());
        mChildren.insert(QCP::msTop, QList<QCPLayoutElement*>());
        mChildren.insert(QCP::msBottom, QList<QCPLayoutElement*>());
    }

    QCPMarginGroup::~QCPMarginGroup()
    {
        clear();
    }

/*!
  Returns whether this margin group is empty. If this function returns true, no layout elements use
  this margin group to synchronize margin sides.
*/
    bool QCPMarginGroup::isEmpty() const
    {
        QHashIterator<QCP::MarginSide, QList<QCPLayoutElement*> > it(mChildren);
        while (it.hasNext())
        {
            it.next();
            if (!it.value().isEmpty())
                return false;
        }
        return true;
    }

/*!
  Clears this margin group. The synchronization of the margin sides that use this margin group is
  lifted and they will use their individual margin sizes again.
*/
    void QCPMarginGroup::clear()
    {
// make all children remove themselves from this margin group:
        QHashIterator<QCP::MarginSide, QList<QCPLayoutElement*> > it(mChildren);
        while (it.hasNext())
        {
            it.next();
            const QList<QCPLayoutElement*> elements = it.value();
            for (int i=elements.size()-1; i>=0; --i)
                elements.at(i)->setMarginGroup(it.key(), nullptr); // removes itself from mChildren via removeChild
        }
    }

/*! \internal

  Returns the synchronized common margin for \a side. This is the margin value that will be used by
  the layout element on the respective side, if it is part of this margin group.

  The common margin is calculated by requesting the automatic margin (\ref
  QCPLayoutElement::calculateAutoMargin) of each element associated with \a side in this margin
  group, and choosing the largest returned value. (QCPLayoutElement::minimumMargins is taken into
  account, too.)
*/
    int QCPMarginGroup::commonMargin(QCP::MarginSide side) const
    {
// query all automatic margins of the layout elements in this margin group side and find maximum:
        int result = 0;
        foreach (QCPLayoutElement *el, mChildren.value(side))
        {
            if (!el->autoMargins().testFlag(side))
                continue;
            int m = qMax(el->calculateAutoMargin(side), QCP::getMarginValue(el->minimumMargins(), side));
            if (m > result)
                result = m;
        }
        return result;
    }

/*! \internal

  Adds \a element to the internal list of child elements, for the margin \a side.

  This function does not modify the margin group property of \a element.
*/
    void QCPMarginGroup::addChild(QCP::MarginSide side, QCPLayoutElement *element)
    {
        if (!mChildren[side].contains(element))
            mChildren[side].append(element);
        else
            qDebug() << Q_FUNC_INFO << "element is already child of this margin group side" << reinterpret_cast<quintptr>(element);
    }

/*! \internal

  Removes \a element from the internal list of child elements, for the margin \a side.

  This function does not modify the margin group property of \a element.
*/
    void QCPMarginGroup::removeChild(QCP::MarginSide side, QCPLayoutElement *element)
    {
        if (!mChildren[side].removeOne(element))
            qDebug() << Q_FUNC_INFO << "element is not child of this margin group side" << reinterpret_cast<quintptr>(element);
    }


} // QCP