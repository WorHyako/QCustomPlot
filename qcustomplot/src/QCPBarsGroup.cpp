#include "include/QCPBarsGroup.hpp"

namespace QCP {
/*! \class QCPBarsGroup
  \brief Groups multiple QCPBars together so they appear side by side

  \image html QCPBarsGroup.png

  When showing multiple QCPBars in one plot which have bars at identical keys, it may be desirable
  to have them appearing next to each other at each key. This is what adding the respective QCPBars
  plottables to a QCPBarsGroup achieves. (An alternative approach is to stack them on top of each
  other, see \ref QCPBars::moveAbove.)

  \section qcpbarsgroup-usage Usage

  To add a QCPBars plottable to the group, create a new group and then add the respective bars
  intances:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbarsgroup-creation
  Alternatively to appending to the group like shown above, you can also set the group on the
  QCPBars plottable via \ref QCPBars::setBarsGroup.

  The spacing between the bars can be configured via \ref setSpacingType and \ref setSpacing. The
  bars in this group appear in the plot in the order they were appended. To insert a bars plottable
  at a certain index position, or to reposition a bars plottable which is already in the group, use
  \ref insert.

  To remove specific bars from the group, use either \ref remove or call \ref
  QCPBars::setBarsGroup "QCPBars::setBarsGroup(0)" on the respective bars plottable.

  To clear the entire group, call \ref clear, or simply delete the group.

  \section qcpbarsgroup-example Example

  The image above is generated with the following code:
  \snippet documentation/doc-image-generator/mainwindow.cpp qcpbarsgroup-example
*/

/* start of documentation of inline functions */

/*! \fn QList<QCPBars*> QCPBarsGroup::bars() const

  Returns all bars currently in this group.

  \see bars(int index)
*/

/*! \fn int QCPBarsGroup::size() const

  Returns the number of QCPBars plottables that are part of this group.

*/

/*! \fn bool QCPBarsGroup::isEmpty() const

  Returns whether this bars group is empty.

  \see size
*/

/*! \fn bool QCPBarsGroup::contains(QCPBars *bars)

  Returns whether the specified \a bars plottable is part of this group.

*/

/* end of documentation of inline functions */

/*!
  Constructs a new bars group for the specified QCustomPlot instance.
*/
    QCPBarsGroup::QCPBarsGroup(QCustomPlot *parentPlot) :
    QObject(parentPlot),
    mParentPlot(parentPlot),
    mSpacingType(stAbsolute),
    mSpacing(4)
    {
    }

    QCPBarsGroup::~QCPBarsGroup()
    {
        clear();
    }

/*!
  Sets how the spacing between adjacent bars is interpreted. See \ref SpacingType.

  The actual spacing can then be specified with \ref setSpacing.

  \see setSpacing
*/
    void QCPBarsGroup::setSpacingType(SpacingType spacingType)
    {
        mSpacingType = spacingType;
    }

/*!
  Sets the spacing between adjacent bars. What the number passed as \a spacing actually means, is
  defined by the current \ref SpacingType, which can be set with \ref setSpacingType.

  \see setSpacingType
*/
    void QCPBarsGroup::setSpacing(double spacing)
    {
        mSpacing = spacing;
    }

/*!
  Returns the QCPBars instance with the specified \a index in this group. If no such QCPBars
  exists, returns \c nullptr.

  \see bars(), size
*/
    QCPBars *QCPBarsGroup::bars(int index) const
    {
        if (index >= 0 && index < mBars.size())
        {
            return mBars.at(index);
        } else
        {
            qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
            return nullptr;
        }
    }

/*!
  Removes all QCPBars plottables from this group.

  \see isEmpty
*/
    void QCPBarsGroup::clear()
    {
        const QList<QCPBars*> oldBars = mBars;
        foreach (QCPBars *bars, oldBars)
        bars->setBarsGroup(nullptr); // removes itself from mBars via removeBars
    }

/*!
  Adds the specified \a bars plottable to this group. Alternatively, you can also use \ref
  QCPBars::setBarsGroup on the \a bars instance.

  \see insert, remove
*/
    void QCPBarsGroup::append(QCPBars *bars)
    {
        if (!bars)
        {
            qDebug() << Q_FUNC_INFO << "bars is 0";
            return;
        }

        if (!mBars.contains(bars))
            bars->setBarsGroup(this);
        else
            qDebug() << Q_FUNC_INFO << "bars plottable is already in this bars group:" << reinterpret_cast<quintptr>(bars);
    }

/*!
  Inserts the specified \a bars plottable into this group at the specified index position \a i.
  This gives you full control over the ordering of the bars.

  \a bars may already be part of this group. In that case, \a bars is just moved to the new index
  position.

  \see append, remove
*/
    void QCPBarsGroup::insert(int i, QCPBars *bars)
    {
        if (!bars)
        {
            qDebug() << Q_FUNC_INFO << "bars is 0";
            return;
        }

// first append to bars list normally:
        if (!mBars.contains(bars))
            bars->setBarsGroup(this);
// then move to according position:
        mBars.move(mBars.indexOf(bars), qBound(0, i, mBars.size()-1));
    }

/*!
  Removes the specified \a bars plottable from this group.

  \see contains, clear
*/
    void QCPBarsGroup::remove(QCPBars *bars)
    {
        if (!bars)
        {
            qDebug() << Q_FUNC_INFO << "bars is 0";
            return;
        }

        if (mBars.contains(bars))
            bars->setBarsGroup(nullptr);
        else
            qDebug() << Q_FUNC_INFO << "bars plottable is not in this bars group:" << reinterpret_cast<quintptr>(bars);
    }

/*! \internal

  Adds the specified \a bars to the internal mBars list of bars. This method does not change the
  barsGroup property on \a bars.

  \see unregisterBars
*/
    void QCPBarsGroup::registerBars(QCPBars *bars)
    {
        if (!mBars.contains(bars))
            mBars.append(bars);
    }

/*! \internal

  Removes the specified \a bars from the internal mBars list of bars. This method does not change
  the barsGroup property on \a bars.

  \see registerBars
*/
    void QCPBarsGroup::unregisterBars(QCPBars *bars)
    {
        mBars.removeOne(bars);
    }

/*! \internal

  Returns the pixel offset in the key dimension the specified \a bars plottable should have at the
  given key coordinate \a keyCoord. The offset is relative to the pixel position of the key
  coordinate \a keyCoord.
*/
    double QCPBarsGroup::keyPixelOffset(const QCPBars *bars, double keyCoord)
    {
// find list of all base bars in case some mBars are stacked:
        QList<const QCPBars*> baseBars;
        foreach (const QCPBars *b, mBars)
        {
            while (b->barBelow())
                b = b->barBelow();
            if (!baseBars.contains(b))
                baseBars.append(b);
        }
// find base bar this "bars" is stacked on:
        const QCPBars *thisBase = bars;
        while (thisBase->barBelow())
            thisBase = thisBase->barBelow();

// determine key pixel offset of this base bars considering all other base bars in this barsgroup:
        double result = 0;
        int index = baseBars.indexOf(thisBase);
        if (index >= 0)
        {
            if (baseBars.size() % 2 == 1 && index == (baseBars.size()-1)/2) // is center bar (int division on purpose)
            {
                return result;
            } else
            {
                double lowerPixelWidth, upperPixelWidth;
                int startIndex;
                int dir = (index <= (baseBars.size()-1)/2) ? -1 : 1; // if bar is to lower keys of center, dir is negative
                if (baseBars.size() % 2 == 0) // even number of bars
                {
                    startIndex = baseBars.size()/2 + (dir < 0 ? -1 : 0);
                    result += getPixelSpacing(baseBars.at(startIndex), keyCoord)*0.5; // half of middle spacing
                } else // uneven number of bars
                {
                    startIndex = (baseBars.size()-1)/2+dir;
                    baseBars.at((baseBars.size()-1)/2)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
                    result += qAbs(upperPixelWidth-lowerPixelWidth)*0.5; // half of center bar
                    result += getPixelSpacing(baseBars.at((baseBars.size()-1)/2), keyCoord); // center bar spacing
                }
                for (int i = startIndex; i != index; i += dir) // add widths and spacings of bars in between center and our bars
                {
                    baseBars.at(i)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
                    result += qAbs(upperPixelWidth-lowerPixelWidth);
                    result += getPixelSpacing(baseBars.at(i), keyCoord);
                }
// finally half of our bars width:
                baseBars.at(index)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
                result += qAbs(upperPixelWidth-lowerPixelWidth)*0.5;
// correct sign of result depending on orientation and direction of key axis:
                result *= dir*thisBase->keyAxis()->pixelOrientation();
            }
        }
        return result;
    }

/*! \internal

  Returns the spacing in pixels which is between this \a bars and the following one, both at the
  key coordinate \a keyCoord.

  \note Typically the returned value doesn't depend on \a bars or \a keyCoord. \a bars is only
  needed to get access to the key axis transformation and axis rect for the modes \ref
  stAxisRectRatio and \ref stPlotCoords. The \a keyCoord is only relevant for spacings given in
  \ref stPlotCoords on a logarithmic axis.
*/
    double QCPBarsGroup::getPixelSpacing(const QCPBars *bars, double keyCoord)
    {
        switch (mSpacingType)
        {
            case stAbsolute:
            {
                return mSpacing;
            }
            case stAxisRectRatio:
            {
                if (bars->keyAxis()->orientation() == Qt::Horizontal)
                    return bars->keyAxis()->axisRect()->width()*mSpacing;
                else
                    return bars->keyAxis()->axisRect()->height()*mSpacing;
            }
            case stPlotCoords:
            {
                double keyPixel = bars->keyAxis()->coordToPixel(keyCoord);
                return qAbs(bars->keyAxis()->coordToPixel(keyCoord+mSpacing)-keyPixel);
            }
        }
        return 0;
    }

} // QCP