#include "include/QCPItemPosition.hpp"

namespace QCP {
/*! \class QCPItemPosition
  \brief Manages the position of an item.

  Every item has at least one public QCPItemPosition member pointer which provides ways to position the
  item on the QCustomPlot surface. Some items have multiple positions, for example QCPItemRect has two:
  \a topLeft and \a bottomRight.

  QCPItemPosition has a type (\ref PositionType) that can be set with \ref setType. This type
  defines how coordinates passed to \ref setCoords are to be interpreted, e.g. as absolute pixel
  coordinates, as plot coordinates of certain axes (\ref QCPItemPosition::setAxes), as fractions of
  the axis rect (\ref QCPItemPosition::setAxisRect), etc. For more advanced plots it is also
  possible to assign different types per X/Y coordinate of the position (see \ref setTypeX, \ref
  setTypeY). This way an item could be positioned for example at a fixed pixel distance from the
  top in the Y direction, while following a plot coordinate in the X direction.

  A QCPItemPosition may have a parent QCPItemAnchor, see \ref setParentAnchor. This way you can tie
  multiple items together. If the QCPItemPosition has a parent, its coordinates (\ref setCoords)
  are considered to be absolute pixels in the reference frame of the parent anchor, where (0, 0)
  means directly ontop of the parent anchor. For example, You could attach the \a start position of
  a QCPItemLine to the \a bottom anchor of a QCPItemText to make the starting point of the line
  always be centered under the text label, no matter where the text is moved to. For more advanced
  plots, it is possible to assign different parent anchors per X/Y coordinate of the position, see
  \ref setParentAnchorX, \ref setParentAnchorY. This way an item could follow another item in the X
  direction but stay at a fixed position in the Y direction. Or even follow item A in X, and item B
  in Y.

  Note that every QCPItemPosition inherits from QCPItemAnchor and thus can itself be used as parent
  anchor for other positions.

  To set the apparent pixel position on the QCustomPlot surface directly, use \ref setPixelPosition. This
  works no matter what type this QCPItemPosition is or what parent-child situation it is in, as \ref
  setPixelPosition transforms the coordinates appropriately, to make the position appear at the specified
  pixel values.
*/

/* start documentation of inline functions */

/*! \fn QCPItemPosition::PositionType *QCPItemPosition::type() const

  Returns the current position type.

  If different types were set for X and Y (\ref setTypeX, \ref setTypeY), this method returns the
  type of the X coordinate. In that case rather use \a typeX() and \a typeY().

  \see setType
*/

/*! \fn QCPItemAnchor *QCPItemPosition::parentAnchor() const

  Returns the current parent anchor.

  If different parent anchors were set for X and Y (\ref setParentAnchorX, \ref setParentAnchorY),
  this method returns the parent anchor of the Y coordinate. In that case rather use \a
  parentAnchorX() and \a parentAnchorY().

  \see setParentAnchor
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPItemPosition. You shouldn't create QCPItemPosition instances directly, even if
  you want to make a new item subclass. Use \ref QCPAbstractItem::createPosition instead, as
  explained in the subclassing section of the QCPAbstractItem documentation.
*/
    QCPItemPosition::QCPItemPosition(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString &name) :
    QCPItemAnchor(parentPlot, parentItem, name),
    mPositionTypeX(ptAbsolute),
    mPositionTypeY(ptAbsolute),
    mKey(0),
    mValue(0),
    mParentAnchorX(nullptr),
    mParentAnchorY(nullptr)
    {
    }

    QCPItemPosition::~QCPItemPosition()
    {
// unregister as parent at children:
// Note: this is done in ~QCPItemAnchor again, but it's important QCPItemPosition does it itself, because only then
//       the setParentAnchor(0) call the correct QCPItemPosition::pixelPosition function instead of QCPItemAnchor::pixelPosition
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
// unregister as child in parent:
        if (mParentAnchorX)
            mParentAnchorX->removeChildX(this);
        if (mParentAnchorY)
            mParentAnchorY->removeChildY(this);
    }

/* can't make this a header inline function, because QPointer breaks with forward declared types, see QTBUG-29588 */
    QCPAxisRect *QCPItemPosition::axisRect() const
    {
        return mAxisRect.data();
    }

/*!
  Sets the type of the position. The type defines how the coordinates passed to \ref setCoords
  should be handled and how the QCPItemPosition should behave in the plot.

  The possible values for \a type can be separated in two main categories:

  \li The position is regarded as a point in plot coordinates. This corresponds to \ref ptPlotCoords
  and requires two axes that define the plot coordinate system. They can be specified with \ref setAxes.
  By default, the QCustomPlot's x- and yAxis are used.

  \li The position is fixed on the QCustomPlot surface, i.e. independent of axis ranges. This
  corresponds to all other types, i.e. \ref ptAbsolute, \ref ptViewportRatio and \ref
  ptAxisRectRatio. They differ only in the way the absolute position is described, see the
  documentation of \ref PositionType for details. For \ref ptAxisRectRatio, note that you can specify
  the axis rect with \ref setAxisRect. By default this is set to the main axis rect.

  Note that the position type \ref ptPlotCoords is only available (and sensible) when the position
  has no parent anchor (\ref setParentAnchor).

  If the type is changed, the apparent pixel position on the plot is preserved. This means
  the coordinates as retrieved with coords() and set with \ref setCoords may change in the process.

  This method sets the type for both X and Y directions. It is also possible to set different types
  for X and Y, see \ref setTypeX, \ref setTypeY.
*/
    void QCPItemPosition::setType(QCPItemPosition::PositionType type)
    {
        setTypeX(type);
        setTypeY(type);
    }

/*!
  This method sets the position type of the X coordinate to \a type.

  For a detailed description of what a position type is, see the documentation of \ref setType.

  \see setType, setTypeY
*/
    void QCPItemPosition::setTypeX(QCPItemPosition::PositionType type)
    {
        if (mPositionTypeX != type)
        {
// if switching from or to coordinate type that isn't valid (e.g. because axes or axis rect
// were deleted), don't try to recover the pixelPosition() because it would output a qDebug warning.
            bool retainPixelPosition = true;
            if ((mPositionTypeX == ptPlotCoords || type == ptPlotCoords) && (!mKeyAxis || !mValueAxis))
                retainPixelPosition = false;
            if ((mPositionTypeX == ptAxisRectRatio || type == ptAxisRectRatio) && (!mAxisRect))
                retainPixelPosition = false;

            QPointF pixel;
            if (retainPixelPosition)
                pixel = pixelPosition();

            mPositionTypeX = type;

            if (retainPixelPosition)
                setPixelPosition(pixel);
        }
    }

/*!
  This method sets the position type of the Y coordinate to \a type.

  For a detailed description of what a position type is, see the documentation of \ref setType.

  \see setType, setTypeX
*/
    void QCPItemPosition::setTypeY(QCPItemPosition::PositionType type)
    {
        if (mPositionTypeY != type)
        {
// if switching from or to coordinate type that isn't valid (e.g. because axes or axis rect
// were deleted), don't try to recover the pixelPosition() because it would output a qDebug warning.
            bool retainPixelPosition = true;
            if ((mPositionTypeY == ptPlotCoords || type == ptPlotCoords) && (!mKeyAxis || !mValueAxis))
                retainPixelPosition = false;
            if ((mPositionTypeY == ptAxisRectRatio || type == ptAxisRectRatio) && (!mAxisRect))
                retainPixelPosition = false;

            QPointF pixel;
            if (retainPixelPosition)
                pixel = pixelPosition();

            mPositionTypeY = type;

            if (retainPixelPosition)
                setPixelPosition(pixel);
        }
    }

/*!
  Sets the parent of this QCPItemPosition to \a parentAnchor. This means the position will now
  follow any position changes of the anchor. The local coordinate system of positions with a parent
  anchor always is absolute pixels, with (0, 0) being exactly on top of the parent anchor. (Hence
  the type shouldn't be set to \ref ptPlotCoords for positions with parent anchors.)

  if \a keepPixelPosition is true, the current pixel position of the QCPItemPosition is preserved
  during reparenting. If it's set to false, the coordinates are set to (0, 0), i.e. the position
  will be exactly on top of the parent anchor.

  To remove this QCPItemPosition from any parent anchor, set \a parentAnchor to \c nullptr.

  If the QCPItemPosition previously had no parent and the type is \ref ptPlotCoords, the type is
  set to \ref ptAbsolute, to keep the position in a valid state.

  This method sets the parent anchor for both X and Y directions. It is also possible to set
  different parents for X and Y, see \ref setParentAnchorX, \ref setParentAnchorY.
*/
    bool QCPItemPosition::setParentAnchor(QCPItemAnchor *parentAnchor, bool keepPixelPosition)
    {
        bool successX = setParentAnchorX(parentAnchor, keepPixelPosition);
        bool successY = setParentAnchorY(parentAnchor, keepPixelPosition);
        return successX && successY;
    }

/*!
  This method sets the parent anchor of the X coordinate to \a parentAnchor.

  For a detailed description of what a parent anchor is, see the documentation of \ref setParentAnchor.

  \see setParentAnchor, setParentAnchorY
*/
    bool QCPItemPosition::setParentAnchorX(QCPItemAnchor *parentAnchor, bool keepPixelPosition)
    {
// make sure self is not assigned as parent:
        if (parentAnchor == this)
        {
            qDebug() << Q_FUNC_INFO << "can't set self as parent anchor" << reinterpret_cast<quintptr>(parentAnchor);
            return false;
        }
// make sure no recursive parent-child-relationships are created:
        QCPItemAnchor *currentParent = parentAnchor;
        while (currentParent)
        {
            if (QCPItemPosition *currentParentPos = currentParent->toQCPItemPosition())
            {
// is a QCPItemPosition, might have further parent, so keep iterating
                if (currentParentPos == this)
                {
                    qDebug() << Q_FUNC_INFO << "can't create recursive parent-child-relationship" << reinterpret_cast<quintptr>(parentAnchor);
                    return false;
                }
                currentParent = currentParentPos->parentAnchorX();
            } else
            {
// is a QCPItemAnchor, can't have further parent. Now make sure the parent items aren't the
// same, to prevent a position being child of an anchor which itself depends on the position,
// because they're both on the same item:
                if (currentParent->mParentItem == mParentItem)
                {
                    qDebug() << Q_FUNC_INFO << "can't set parent to be an anchor which itself depends on this position" << reinterpret_cast<quintptr>(parentAnchor);
                    return false;
                }
                break;
            }
        }

// if previously no parent set and PosType is still ptPlotCoords, set to ptAbsolute:
        if (!mParentAnchorX && mPositionTypeX == ptPlotCoords)
            setTypeX(ptAbsolute);

// save pixel position:
        QPointF pixelP;
        if (keepPixelPosition)
            pixelP = pixelPosition();
// unregister at current parent anchor:
        if (mParentAnchorX)
            mParentAnchorX->removeChildX(this);
// register at new parent anchor:
        if (parentAnchor)
            parentAnchor->addChildX(this);
        mParentAnchorX = parentAnchor;
// restore pixel position under new parent:
        if (keepPixelPosition)
            setPixelPosition(pixelP);
        else
            setCoords(0, coords().y());
        return true;
    }

/*!
  This method sets the parent anchor of the Y coordinate to \a parentAnchor.

  For a detailed description of what a parent anchor is, see the documentation of \ref setParentAnchor.

  \see setParentAnchor, setParentAnchorX
*/
    bool QCPItemPosition::setParentAnchorY(QCPItemAnchor *parentAnchor, bool keepPixelPosition)
    {
// make sure self is not assigned as parent:
        if (parentAnchor == this)
        {
            qDebug() << Q_FUNC_INFO << "can't set self as parent anchor" << reinterpret_cast<quintptr>(parentAnchor);
            return false;
        }
// make sure no recursive parent-child-relationships are created:
        QCPItemAnchor *currentParent = parentAnchor;
        while (currentParent)
        {
            if (QCPItemPosition *currentParentPos = currentParent->toQCPItemPosition())
            {
// is a QCPItemPosition, might have further parent, so keep iterating
                if (currentParentPos == this)
                {
                    qDebug() << Q_FUNC_INFO << "can't create recursive parent-child-relationship" << reinterpret_cast<quintptr>(parentAnchor);
                    return false;
                }
                currentParent = currentParentPos->parentAnchorY();
            } else
            {
// is a QCPItemAnchor, can't have further parent. Now make sure the parent items aren't the
// same, to prevent a position being child of an anchor which itself depends on the position,
// because they're both on the same item:
                if (currentParent->mParentItem == mParentItem)
                {
                    qDebug() << Q_FUNC_INFO << "can't set parent to be an anchor which itself depends on this position" << reinterpret_cast<quintptr>(parentAnchor);
                    return false;
                }
                break;
            }
        }

// if previously no parent set and PosType is still ptPlotCoords, set to ptAbsolute:
        if (!mParentAnchorY && mPositionTypeY == ptPlotCoords)
            setTypeY(ptAbsolute);

// save pixel position:
        QPointF pixelP;
        if (keepPixelPosition)
            pixelP = pixelPosition();
// unregister at current parent anchor:
        if (mParentAnchorY)
            mParentAnchorY->removeChildY(this);
// register at new parent anchor:
        if (parentAnchor)
            parentAnchor->addChildY(this);
        mParentAnchorY = parentAnchor;
// restore pixel position under new parent:
        if (keepPixelPosition)
            setPixelPosition(pixelP);
        else
            setCoords(coords().x(), 0);
        return true;
    }

/*!
  Sets the coordinates of this QCPItemPosition. What the coordinates mean, is defined by the type
  (\ref setType, \ref setTypeX, \ref setTypeY).

  For example, if the type is \ref ptAbsolute, \a key and \a value mean the x and y pixel position
  on the QCustomPlot surface. In that case the origin (0, 0) is in the top left corner of the
  QCustomPlot viewport. If the type is \ref ptPlotCoords, \a key and \a value mean a point in the
  plot coordinate system defined by the axes set by \ref setAxes. By default those are the
  QCustomPlot's xAxis and yAxis. See the documentation of \ref setType for other available
  coordinate types and their meaning.

  If different types were configured for X and Y (\ref setTypeX, \ref setTypeY), \a key and \a
  value must also be provided in the different coordinate systems. Here, the X type refers to \a
  key, and the Y type refers to \a value.

  \see setPixelPosition
*/
    void QCPItemPosition::setCoords(double key, double value)
    {
        mKey = key;
        mValue = value;
    }

/*! \overload

  Sets the coordinates as a QPointF \a pos where pos.x has the meaning of \a key and pos.y the
  meaning of \a value of the \ref setCoords(double key, double value) method.
*/
    void QCPItemPosition::setCoords(const QPointF &pos)
    {
        setCoords(pos.x(), pos.y());
    }

/*!
  Returns the final absolute pixel position of the QCPItemPosition on the QCustomPlot surface. It
  includes all effects of type (\ref setType) and possible parent anchors (\ref setParentAnchor).

  \see setPixelPosition
*/
    QPointF QCPItemPosition::pixelPosition() const
    {
        QPointF result;

// determine X:
        switch (mPositionTypeX)
        {
            case ptAbsolute:
            {
                result.rx() = mKey;
                if (mParentAnchorX)
                    result.rx() += mParentAnchorX->pixelPosition().x();
                break;
            }
            case ptViewportRatio:
            {
                result.rx() = mKey*mParentPlot->viewport().width();
                if (mParentAnchorX)
                    result.rx() += mParentAnchorX->pixelPosition().x();
                else
                    result.rx() += mParentPlot->viewport().left();
                break;
            }
            case ptAxisRectRatio:
            {
                if (mAxisRect)
                {
                    result.rx() = mKey*mAxisRect.data()->width();
                    if (mParentAnchorX)
                        result.rx() += mParentAnchorX->pixelPosition().x();
                    else
                        result.rx() += mAxisRect.data()->left();
                } else
                    qDebug() << Q_FUNC_INFO << "Item position type x is ptAxisRectRatio, but no axis rect was defined";
                break;
            }
            case ptPlotCoords:
            {
                if (mKeyAxis && mKeyAxis.data()->orientation() == Qt::Horizontal)
                    result.rx() = mKeyAxis.data()->coordToPixel(mKey);
                else if (mValueAxis && mValueAxis.data()->orientation() == Qt::Horizontal)
                    result.rx() = mValueAxis.data()->coordToPixel(mValue);
                else
                    qDebug() << Q_FUNC_INFO << "Item position type x is ptPlotCoords, but no axes were defined";
                break;
            }
        }

// determine Y:
        switch (mPositionTypeY)
        {
            case ptAbsolute:
            {
                result.ry() = mValue;
                if (mParentAnchorY)
                    result.ry() += mParentAnchorY->pixelPosition().y();
                break;
            }
            case ptViewportRatio:
            {
                result.ry() = mValue*mParentPlot->viewport().height();
                if (mParentAnchorY)
                    result.ry() += mParentAnchorY->pixelPosition().y();
                else
                    result.ry() += mParentPlot->viewport().top();
                break;
            }
            case ptAxisRectRatio:
            {
                if (mAxisRect)
                {
                    result.ry() = mValue*mAxisRect.data()->height();
                    if (mParentAnchorY)
                        result.ry() += mParentAnchorY->pixelPosition().y();
                    else
                        result.ry() += mAxisRect.data()->top();
                } else
                    qDebug() << Q_FUNC_INFO << "Item position type y is ptAxisRectRatio, but no axis rect was defined";
                break;
            }
            case ptPlotCoords:
            {
                if (mKeyAxis && mKeyAxis.data()->orientation() == Qt::Vertical)
                    result.ry() = mKeyAxis.data()->coordToPixel(mKey);
                else if (mValueAxis && mValueAxis.data()->orientation() == Qt::Vertical)
                    result.ry() = mValueAxis.data()->coordToPixel(mValue);
                else
                    qDebug() << Q_FUNC_INFO << "Item position type y is ptPlotCoords, but no axes were defined";
                break;
            }
        }

        return result;
    }

/*!
  When \ref setType is \ref ptPlotCoords, this function may be used to specify the axes the
  coordinates set with \ref setCoords relate to. By default they are set to the initial xAxis and
  yAxis of the QCustomPlot.
*/
    void QCPItemPosition::setAxes(QCPAxis *keyAxis, QCPAxis *valueAxis)
    {
        mKeyAxis = keyAxis;
        mValueAxis = valueAxis;
    }

/*!
  When \ref setType is \ref ptAxisRectRatio, this function may be used to specify the axis rect the
  coordinates set with \ref setCoords relate to. By default this is set to the main axis rect of
  the QCustomPlot.
*/
    void QCPItemPosition::setAxisRect(QCPAxisRect *axisRect)
    {
        mAxisRect = axisRect;
    }

/*!
  Sets the apparent pixel position. This works no matter what type (\ref setType) this
  QCPItemPosition is or what parent-child situation it is in, as coordinates are transformed
  appropriately, to make the position finally appear at the specified pixel values.

  Only if the type is \ref ptAbsolute and no parent anchor is set, this function's effect is
  identical to that of \ref setCoords.

  \see pixelPosition, setCoords
*/
    void QCPItemPosition::setPixelPosition(const QPointF &pixelPosition)
    {
        double x = pixelPosition.x();
        double y = pixelPosition.y();

        switch (mPositionTypeX)
        {
            case ptAbsolute:
            {
                if (mParentAnchorX)
                    x -= mParentAnchorX->pixelPosition().x();
                break;
            }
            case ptViewportRatio:
            {
                if (mParentAnchorX)
                    x -= mParentAnchorX->pixelPosition().x();
                else
                    x -= mParentPlot->viewport().left();
                x /= double(mParentPlot->viewport().width());
                break;
            }
            case ptAxisRectRatio:
            {
                if (mAxisRect)
                {
                    if (mParentAnchorX)
                        x -= mParentAnchorX->pixelPosition().x();
                    else
                        x -= mAxisRect.data()->left();
                    x /= double(mAxisRect.data()->width());
                } else
                    qDebug() << Q_FUNC_INFO << "Item position type x is ptAxisRectRatio, but no axis rect was defined";
                break;
            }
            case ptPlotCoords:
            {
                if (mKeyAxis && mKeyAxis.data()->orientation() == Qt::Horizontal)
                    x = mKeyAxis.data()->pixelToCoord(x);
                else if (mValueAxis && mValueAxis.data()->orientation() == Qt::Horizontal)
                    y = mValueAxis.data()->pixelToCoord(x);
                else
                    qDebug() << Q_FUNC_INFO << "Item position type x is ptPlotCoords, but no axes were defined";
                break;
            }
        }

        switch (mPositionTypeY)
        {
            case ptAbsolute:
            {
                if (mParentAnchorY)
                    y -= mParentAnchorY->pixelPosition().y();
                break;
            }
            case ptViewportRatio:
            {
                if (mParentAnchorY)
                    y -= mParentAnchorY->pixelPosition().y();
                else
                    y -= mParentPlot->viewport().top();
                y /= double(mParentPlot->viewport().height());
                break;
            }
            case ptAxisRectRatio:
            {
                if (mAxisRect)
                {
                    if (mParentAnchorY)
                        y -= mParentAnchorY->pixelPosition().y();
                    else
                        y -= mAxisRect.data()->top();
                    y /= double(mAxisRect.data()->height());
                } else
                    qDebug() << Q_FUNC_INFO << "Item position type y is ptAxisRectRatio, but no axis rect was defined";
                break;
            }
            case ptPlotCoords:
            {
                if (mKeyAxis && mKeyAxis.data()->orientation() == Qt::Vertical)
                    x = mKeyAxis.data()->pixelToCoord(y);
                else if (mValueAxis && mValueAxis.data()->orientation() == Qt::Vertical)
                    y = mValueAxis.data()->pixelToCoord(y);
                else
                    qDebug() << Q_FUNC_INFO << "Item position type y is ptPlotCoords, but no axes were defined";
                break;
            }
        }

        setCoords(x, y);
    }


} // QCp