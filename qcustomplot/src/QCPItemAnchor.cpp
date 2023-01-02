#include "include/QCPItemAnchor.hpp"

namespace QCP {
/*! \class QCPItemAnchor
  \brief An anchor of an item to which positions can be attached to.

  An item (QCPAbstractItem) may have one or more anchors. Unlike QCPItemPosition, an anchor doesn't
  control anything on its item, but provides a way to tie other items via their positions to the
  anchor.

  For example, a QCPItemRect is defined by its positions \a topLeft and \a bottomRight.
  Additionally it has various anchors like \a top, \a topRight or \a bottomLeft etc. So you can
  attach the \a start (which is a QCPItemPosition) of a QCPItemLine to one of the anchors by
  calling QCPItemPosition::setParentAnchor on \a start, passing the wanted anchor of the
  QCPItemRect. This way the start of the line will now always follow the respective anchor location
  on the rect item.

  Note that QCPItemPosition derives from QCPItemAnchor, so every position can also serve as an
  anchor to other positions.

  To learn how to provide anchors in your own item subclasses, see the subclassing section of the
  QCPAbstractItem documentation.
*/

/* start documentation of inline functions */

/*! \fn virtual QCPItemPosition *QCPItemAnchor::toQCPItemPosition()

  Returns \c nullptr if this instance is merely a QCPItemAnchor, and a valid pointer of type
  QCPItemPosition* if it actually is a QCPItemPosition (which is a subclass of QCPItemAnchor).

  This safe downcast functionality could also be achieved with a dynamic_cast. However, QCustomPlot avoids
  dynamic_cast to work with projects that don't have RTTI support enabled (e.g. -fno-rtti flag with
  gcc compiler).
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPItemAnchor. You shouldn't create QCPItemAnchor instances directly, even if
  you want to make a new item subclass. Use \ref QCPAbstractItem::createAnchor instead, as
  explained in the subclassing section of the QCPAbstractItem documentation.
*/
    QCPItemAnchor::QCPItemAnchor(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString &name, int anchorId) :
            mName(name),
            mParentPlot(parentPlot),
            mParentItem(parentItem),
            mAnchorId(anchorId)
    {
    }

    QCPItemAnchor::~QCPItemAnchor()
    {
// unregister as parent at children:
        foreach (QCPItemPosition *child, mChildrenX.values())
        {
            if (child->parentAnchorX() == this)
                child->setParentAnchorX(nullptr); // this acts back on this anchor and child removes itself from mChildrenX
        }
        foreach (QCPItemPosition *child, mChildrenY.values())
        {
            if (child->parentAnchorY() == this)
                child->setParentAnchorY(nullptr); // this acts back on this anchor and child removes itself from mChildrenY
        }
    }

/*!
  Returns the final absolute pixel position of the QCPItemAnchor on the QCustomPlot surface.

  The pixel information is internally retrieved via QCPAbstractItem::anchorPixelPosition of the
  parent item, QCPItemAnchor is just an intermediary.
*/
    QPointF QCPItemAnchor::pixelPosition() const
    {
        if (mParentItem)
        {
            if (mAnchorId > -1)
            {
                return mParentItem->anchorPixelPosition(mAnchorId);
            } else
            {
                qDebug() << Q_FUNC_INFO << "no valid anchor id set:" << mAnchorId;
                return {};
            }
        } else
        {
            qDebug() << Q_FUNC_INFO << "no parent item set";
            return {};
        }
    }

/*! \internal

  Adds \a pos to the childX list of this anchor, which keeps track of which children use this
  anchor as parent anchor for the respective coordinate. This is necessary to notify the children
  prior to destruction of the anchor.

  Note that this function does not change the parent setting in \a pos.
*/
    void QCPItemAnchor::addChildX(QCPItemPosition *pos)
    {
        if (!mChildrenX.contains(pos))
            mChildrenX.insert(pos);
        else
            qDebug() << Q_FUNC_INFO << "provided pos is child already" << reinterpret_cast<quintptr>(pos);
    }

/*! \internal

  Removes \a pos from the childX list of this anchor.

  Note that this function does not change the parent setting in \a pos.
*/
    void QCPItemAnchor::removeChildX(QCPItemPosition *pos)
    {
        if (!mChildrenX.remove(pos))
            qDebug() << Q_FUNC_INFO << "provided pos isn't child" << reinterpret_cast<quintptr>(pos);
    }

/*! \internal

  Adds \a pos to the childY list of this anchor, which keeps track of which children use this
  anchor as parent anchor for the respective coordinate. This is necessary to notify the children
  prior to destruction of the anchor.

  Note that this function does not change the parent setting in \a pos.
*/
    void QCPItemAnchor::addChildY(QCPItemPosition *pos)
    {
        if (!mChildrenY.contains(pos))
            mChildrenY.insert(pos);
        else
            qDebug() << Q_FUNC_INFO << "provided pos is child already" << reinterpret_cast<quintptr>(pos);
    }

/*! \internal

  Removes \a pos from the childY list of this anchor.

  Note that this function does not change the parent setting in \a pos.
*/
    void QCPItemAnchor::removeChildY(QCPItemPosition *pos)
    {
        if (!mChildrenY.remove(pos))
            qDebug() << Q_FUNC_INFO << "provided pos isn't child" << reinterpret_cast<quintptr>(pos);
    }

} // QCP