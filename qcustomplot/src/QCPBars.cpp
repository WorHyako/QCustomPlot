#include "include/QCPBars.hpp"

namespace QCP {
/*! \class QCPBars
  \brief A plottable representing a bar chart in a plot.

  \image html QCPBars.png

  To plot data, assign it with the \ref setData or \ref addData functions.

  \section qcpbars-appearance Changing the appearance

  The appearance of the bars is determined by the pen and the brush (\ref setPen, \ref setBrush).
  The width of the individual bars can be controlled with \ref setWidthType and \ref setWidth.

  Bar charts are stackable. This means, two QCPBars plottables can be placed on top of each other
  (see \ref QCPBars::moveAbove). So when two bars are at the same key position, they will appear
  stacked.

  If you would like to group multiple QCPBars plottables together so they appear side by side as
  shown below, use QCPBarsGroup.

  \image html QCPBarsGroup.png

  \section qcpbars-usage Usage

  Like all data representing objects in QCustomPlot, the QCPBars is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::removePlottable, etc.)

  Usually, you first create an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbars-creation-1
  which registers it with the QCustomPlot instance of the passed axes. Note that this QCustomPlot instance takes
  ownership of the plottable, so do not delete it manually but use QCustomPlot::removePlottable() instead.
  The newly created plottable can be modified, e.g.:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbars-creation-2
*/

/* start of documentation of inline functions */

/*! \fn QSharedPointer<QCPBarsDataContainer> QCPBars::data() const

  Returns a shared pointer to the internal data storage of type \ref QCPBarsDataContainer. You may
  use it to directly manipulate the data, which may be more convenient and faster than using the
  regular \ref setData or \ref addData methods.
*/

/*! \fn QCPBars *QCPBars::barBelow() const
  Returns the bars plottable that is directly below this bars plottable.
  If there is no such plottable, returns \c nullptr.

  \see barAbove, moveBelow, moveAbove
*/

/*! \fn QCPBars *QCPBars::barAbove() const
  Returns the bars plottable that is directly above this bars plottable.
  If there is no such plottable, returns \c nullptr.

  \see barBelow, moveBelow, moveAbove
*/

/* end of documentation of inline functions */

/*!
  Constructs a bar chart which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.

  The created QCPBars is automatically registered with the QCustomPlot instance inferred from \a
  keyAxis. This QCustomPlot instance takes ownership of the QCPBars, so do not delete it manually
  but use QCustomPlot::removePlottable() instead.
*/
    QCPBars::QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis) :
            QCPAbstractPlottable1D<QCPBarsData>(keyAxis, valueAxis),
            mWidth(0.75),
            mWidthType(wtPlotCoords),
            mBarsGroup(nullptr),
            mBaseValue(0),
            mStackingGap(1)
    {
// modify inherited properties from abstract plottable:
        mPen.setColor(Qt::blue);
        mPen.setStyle(Qt::SolidLine);
        mBrush.setColor(QColor(40, 50, 255, 30));
        mBrush.setStyle(Qt::SolidPattern);
        mSelectionDecorator->setBrush(QBrush(QColor(160, 160, 255)));
    }

    QCPBars::~QCPBars()
    {
        setBarsGroup(nullptr);
        if (mBarBelow || mBarAbove)
            connectBars(mBarBelow.data(), mBarAbove.data()); // take this bar out of any stacking
    }

/*! \overload

  Replaces the current data container with the provided \a data container.

  Since a QSharedPointer is used, multiple QCPBars may share the same data container safely.
  Modifying the data in the container will then affect all bars that share the container. Sharing
  can be achieved by simply exchanging the data containers wrapped in shared pointers:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbars-datasharing-1

  If you do not wish to share containers, but create a copy from an existing container, rather use
  the \ref QCPDataContainer<DataType>::set method on the bar's data container directly:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpbars-datasharing-2

  \see addData
*/
    void QCPBars::setData(QSharedPointer<QCPBarsDataContainer> data)
    {
        mDataContainer = data;
    }

/*! \overload

  Replaces the current data with the provided points in \a keys and \a values. The provided
  vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.

  If you can guarantee that the passed data points are sorted by \a keys in ascending order, you
  can set \a alreadySorted to true, to improve performance by saving a sorting run.

  \see addData
*/
    void QCPBars::setData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted)
    {
        mDataContainer->clear();
        addData(keys, values, alreadySorted);
    }

/*!
  Sets the width of the bars.

  How the number passed as \a width is interpreted (e.g. screen pixels, plot coordinates,...),
  depends on the currently set width type, see \ref setWidthType and \ref WidthType.
*/
    void QCPBars::setWidth(double width)
    {
        mWidth = width;
    }

/*!
  Sets how the width of the bars is defined. See the documentation of \ref WidthType for an
  explanation of the possible values for \a widthType.

  The default value is \ref wtPlotCoords.

  \see setWidth
*/
    void QCPBars::setWidthType(QCPBars::WidthType widthType)
    {
        mWidthType = widthType;
    }

/*!
  Sets to which QCPBarsGroup this QCPBars instance belongs to. Alternatively, you can also use \ref
  QCPBarsGroup::append.

  To remove this QCPBars from any group, set \a barsGroup to \c nullptr.
*/
    void QCPBars::setBarsGroup(QCPBarsGroup *barsGroup)
    {
// deregister at old group:
        if (mBarsGroup)
            mBarsGroup->unregisterBars(this);
        mBarsGroup = barsGroup;
// register at new group:
        if (mBarsGroup)
            mBarsGroup->registerBars(this);
    }

/*!
  Sets the base value of this bars plottable.

  The base value defines where on the value coordinate the bars start. How far the bars extend from
  the base value is given by their individual value data. For example, if the base value is set to
  1, a bar with data value 2 will have its lowest point at value coordinate 1 and highest point at
  3.

  For stacked bars, only the base value of the bottom-most QCPBars has meaning.

  The default base value is 0.
*/
    void QCPBars::setBaseValue(double baseValue)
    {
        mBaseValue = baseValue;
    }

/*!
  If this bars plottable is stacked on top of another bars plottable (\ref moveAbove), this method
  allows specifying a distance in \a pixels, by which the drawn bar rectangles will be separated by
  the bars below it.
*/
    void QCPBars::setStackingGap(double pixels)
    {
        mStackingGap = pixels;
    }

/*! \overload

  Adds the provided points in \a keys and \a values to the current data. The provided vectors
  should have equal length. Else, the number of added points will be the size of the smallest
  vector.

  If you can guarantee that the passed data points are sorted by \a keys in ascending order, you
  can set \a alreadySorted to true, to improve performance by saving a sorting run.

  Alternatively, you can also access and modify the data directly via the \ref data method, which
  returns a pointer to the internal data container.
*/
    void QCPBars::addData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted)
    {
        if (keys.size() != values.size())
            qDebug() << Q_FUNC_INFO << "keys and values have different sizes:" << keys.size() << values.size();
        const int n = qMin(keys.size(), values.size());
        QVector<QCPBarsData> tempData(n);
        QVector<QCPBarsData>::iterator it = tempData.begin();
        const QVector<QCPBarsData>::iterator itEnd = tempData.end();
        int i = 0;
        while (it != itEnd)
        {
            it->key = keys[i];
            it->value = values[i];
            ++it;
            ++i;
        }
        mDataContainer->add(tempData, alreadySorted); // don't modify tempData beyond this to prevent copy on write
    }

/*! \overload
  Adds the provided data point as \a key and \a value to the current data.

  Alternatively, you can also access and modify the data directly via the \ref data method, which
  returns a pointer to the internal data container.
*/
    void QCPBars::addData(double key, double value)
    {
        mDataContainer->add(QCPBarsData(key, value));
    }

/*!
  Moves this bars plottable below \a bars. In other words, the bars of this plottable will appear
  below the bars of \a bars. The move target \a bars must use the same key and value axis as this
  plottable.

  Inserting into and removing from existing bar stacking is handled gracefully. If \a bars already
  has a bars object below itself, this bars object is inserted between the two. If this bars object
  is already between two other bars, the two other bars will be stacked on top of each other after
  the operation.

  To remove this bars plottable from any stacking, set \a bars to \c nullptr.

  \see moveBelow, barAbove, barBelow
*/
    void QCPBars::moveBelow(QCPBars *bars)
    {
        if (bars == this) return;
        if (bars && (bars->keyAxis() != mKeyAxis.data() || bars->valueAxis() != mValueAxis.data()))
        {
            qDebug() << Q_FUNC_INFO << "passed QCPBars* doesn't have same key and value axis as this QCPBars";
            return;
        }
// remove from stacking:
        connectBars(mBarBelow.data(), mBarAbove.data()); // Note: also works if one (or both) of them is 0
// if new bar given, insert this bar below it:
        if (bars)
        {
            if (bars->mBarBelow)
                connectBars(bars->mBarBelow.data(), this);
            connectBars(this, bars);
        }
    }

/*!
  Moves this bars plottable above \a bars. In other words, the bars of this plottable will appear
  above the bars of \a bars. The move target \a bars must use the same key and value axis as this
  plottable.

  Inserting into and removing from existing bar stacking is handled gracefully. If \a bars already
  has a bars object above itself, this bars object is inserted between the two. If this bars object
  is already between two other bars, the two other bars will be stacked on top of each other after
  the operation.

  To remove this bars plottable from any stacking, set \a bars to \c nullptr.

  \see moveBelow, barBelow, barAbove
*/
    void QCPBars::moveAbove(QCPBars *bars)
    {
        if (bars == this) return;
        if (bars && (bars->keyAxis() != mKeyAxis.data() || bars->valueAxis() != mValueAxis.data()))
        {
            qDebug() << Q_FUNC_INFO << "passed QCPBars* doesn't have same key and value axis as this QCPBars";
            return;
        }
// remove from stacking:
        connectBars(mBarBelow.data(), mBarAbove.data()); // Note: also works if one (or both) of them is 0
// if new bar given, insert this bar above it:
        if (bars)
        {
            if (bars->mBarAbove)
                connectBars(this, bars->mBarAbove.data());
            connectBars(bars, this);
        }
    }

/*!
  \copydoc QCPPlottableInterface1D::selectTestRect
*/
    QCPDataSelection QCPBars::selectTestRect(const QRectF &rect, bool onlySelectable) const
    {
        QCPDataSelection result;
        if ((onlySelectable && mSelectable == QCP::stNone) || mDataContainer->isEmpty())
            return result;
        if (!mKeyAxis || !mValueAxis)
            return result;

        QCPBarsDataContainer::const_iterator visibleBegin, visibleEnd;
        getVisibleDataBounds(visibleBegin, visibleEnd);

        for (QCPBarsDataContainer::const_iterator it=visibleBegin; it!=visibleEnd; ++it)
        {
            if (rect.intersects(getBarRect(it->key, it->value)))
                result.addDataRange(QCPDataRange(int(it-mDataContainer->constBegin()), int(it-mDataContainer->constBegin()+1)), false);
        }
        result.simplify();
        return result;
    }

/*!
  Implements a selectTest specific to this plottable's point geometry.

  If \a details is not 0, it will be set to a \ref QCPDataSelection, describing the closest data
  point to \a pos.

  \seebaseclassmethod \ref QCPAbstractPlottable::selectTest
*/
    double QCPBars::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
    {
        Q_UNUSED(details)
        if ((onlySelectable && mSelectable == QCP::stNone) || mDataContainer->isEmpty())
            return -1;
        if (!mKeyAxis || !mValueAxis)
            return -1;

        if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()) || mParentPlot->interactions().testFlag(QCP::iSelectPlottablesBeyondAxisRect))
        {
// get visible data range:
            QCPBarsDataContainer::const_iterator visibleBegin, visibleEnd;
            getVisibleDataBounds(visibleBegin, visibleEnd);
            for (QCPBarsDataContainer::const_iterator it=visibleBegin; it!=visibleEnd; ++it)
            {
                if (getBarRect(it->key, it->value).contains(pos))
                {
                    if (details)
                    {
                        int pointIndex = int(it-mDataContainer->constBegin());
                        details->setValue(QCPDataSelection(QCPDataRange(pointIndex, pointIndex+1)));
                    }
                    return mParentPlot->selectionTolerance()*0.99;
                }
            }
        }
        return -1;
    }

/* inherits documentation from base class */
    QCPRange QCPBars::getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain) const
    {
/* Note: If this QCPBars uses absolute pixels as width (or is in a QCPBarsGroup with spacing in
  absolute pixels), using this method to adapt the key axis range to fit the bars into the
  currently visible axis range will not work perfectly. Because in the moment the axis range is
  changed to the new range, the fixed pixel widths/spacings will represent different coordinate
  spans than before, which in turn would require a different key range to perfectly fit, and so on.
  The only solution would be to iteratively approach the perfect fitting axis range, but the
  mismatch isn't large enough in most applications, to warrant this here. If a user does need a
  better fit, he should call the corresponding axis rescale multiple times in a row.
  */
        QCPRange range;
        range = mDataContainer->keyRange(foundRange, inSignDomain);

// determine exact range of bars by including bar width and barsgroup offset:
        if (foundRange && mKeyAxis)
        {
            double lowerPixelWidth, upperPixelWidth, keyPixel;
// lower range bound:
            getPixelWidth(range.lower, lowerPixelWidth, upperPixelWidth);
            keyPixel = mKeyAxis.data()->coordToPixel(range.lower) + lowerPixelWidth;
            if (mBarsGroup)
                keyPixel += mBarsGroup->keyPixelOffset(this, range.lower);
            const double lowerCorrected = mKeyAxis.data()->pixelToCoord(keyPixel);
            if (!qIsNaN(lowerCorrected) && qIsFinite(lowerCorrected) && range.lower > lowerCorrected)
                range.lower = lowerCorrected;
// upper range bound:
            getPixelWidth(range.upper, lowerPixelWidth, upperPixelWidth);
            keyPixel = mKeyAxis.data()->coordToPixel(range.upper) + upperPixelWidth;
            if (mBarsGroup)
                keyPixel += mBarsGroup->keyPixelOffset(this, range.upper);
            const double upperCorrected = mKeyAxis.data()->pixelToCoord(keyPixel);
            if (!qIsNaN(upperCorrected) && qIsFinite(upperCorrected) && range.upper < upperCorrected)
                range.upper = upperCorrected;
        }
        return range;
    }

/* inherits documentation from base class */
    QCPRange QCPBars::getValueRange(bool &foundRange, QCP::SignDomain inSignDomain, const QCPRange &inKeyRange) const
    {
// Note: can't simply use mDataContainer->valueRange here because we need to
// take into account bar base value and possible stacking of multiple bars
        QCPRange range;
        range.lower = mBaseValue;
        range.upper = mBaseValue;
        bool haveLower = true; // set to true, because baseValue should always be visible in bar charts
        bool haveUpper = true; // set to true, because baseValue should always be visible in bar charts
        QCPBarsDataContainer::const_iterator itBegin = mDataContainer->constBegin();
        QCPBarsDataContainer::const_iterator itEnd = mDataContainer->constEnd();
        if (inKeyRange != QCPRange())
        {
            itBegin = mDataContainer->findBegin(inKeyRange.lower, false);
            itEnd = mDataContainer->findEnd(inKeyRange.upper, false);
        }
        for (QCPBarsDataContainer::const_iterator it = itBegin; it != itEnd; ++it)
        {
            const double current = it->value + getStackedBaseValue(it->key, it->value >= 0);
            if (qIsNaN(current)) continue;
            if (inSignDomain == QCP::sdBoth || (inSignDomain == QCP::sdNegative && current < 0) || (inSignDomain == QCP::sdPositive && current > 0))
            {
                if (current < range.lower || !haveLower)
                {
                    range.lower = current;
                    haveLower = true;
                }
                if (current > range.upper || !haveUpper)
                {
                    range.upper = current;
                    haveUpper = true;
                }
            }
        }

        foundRange = true; // return true because bar charts always have the 0-line visible
        return range;
    }

/* inherits documentation from base class */
    QPointF QCPBars::dataPixelPosition(int index) const
    {
        if (index >= 0 && index < mDataContainer->size())
        {
            QCPAxis *keyAxis = mKeyAxis.data();
            QCPAxis *valueAxis = mValueAxis.data();
            if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return {}; }

            const QCPDataContainer<QCPBarsData>::const_iterator it = mDataContainer->constBegin()+index;
            const double valuePixel = valueAxis->coordToPixel(getStackedBaseValue(it->key, it->value >= 0) + it->value);
            const double keyPixel = keyAxis->coordToPixel(it->key) + (mBarsGroup ? mBarsGroup->keyPixelOffset(this, it->key) : 0);
            if (keyAxis->orientation() == Qt::Horizontal)
                return {keyPixel, valuePixel};
            else
                return {valuePixel, keyPixel};
        } else
        {
            qDebug() << Q_FUNC_INFO << "Index out of bounds" << index;
            return {};
        }
    }

/* inherits documentation from base class */
    void QCPBars::draw(QCPPainter *painter)
    {
        if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
        if (mDataContainer->isEmpty()) return;

        QCPBarsDataContainer::const_iterator visibleBegin, visibleEnd;
        getVisibleDataBounds(visibleBegin, visibleEnd);

// loop over and draw segments of unselected/selected data:
        QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments;
        getDataSegments(selectedSegments, unselectedSegments);
        allSegments << unselectedSegments << selectedSegments;
        for (int i=0; i<allSegments.size(); ++i)
        {
            bool isSelectedSegment = i >= unselectedSegments.size();
            QCPBarsDataContainer::const_iterator begin = visibleBegin;
            QCPBarsDataContainer::const_iterator end = visibleEnd;
            mDataContainer->limitIteratorsToDataRange(begin, end, allSegments.at(i));
            if (begin == end)
                continue;

            for (QCPBarsDataContainer::const_iterator it=begin; it!=end; ++it)
            {
// check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
                if (QCP::isInvalidData(it->key, it->value))
qDebug() << Q_FUNC_INFO << "Data point at" << it->key << "of drawn range invalid." << "Plottable name:" << name();
#endif
// draw bar:
                if (isSelectedSegment && mSelectionDecorator)
                {
                    mSelectionDecorator->applyBrush(painter);
                    mSelectionDecorator->applyPen(painter);
                } else
                {
                    painter->setBrush(mBrush);
                    painter->setPen(mPen);
                }
                applyDefaultAntialiasingHint(painter);
                painter->drawPolygon(getBarRect(it->key, it->value));
            }
        }

// draw other selection decoration that isn't just line/scatter pens and brushes:
        if (mSelectionDecorator)
            mSelectionDecorator->drawDecoration(painter, selection());
    }

/* inherits documentation from base class */
    void QCPBars::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
    {
// draw filled rect:
        applyDefaultAntialiasingHint(painter);
        painter->setBrush(mBrush);
        painter->setPen(mPen);
        QRectF r = QRectF(0, 0, rect.width()*0.67, rect.height()*0.67);
        r.moveCenter(rect.center());
        painter->drawRect(r);
    }

/*!  \internal

  called by \ref draw to determine which data (key) range is visible at the current key axis range
  setting, so only that needs to be processed. It also takes into account the bar width.

  \a begin returns an iterator to the lowest data point that needs to be taken into account when
  plotting. Note that in order to get a clean plot all the way to the edge of the axis rect, \a
  lower may still be just outside the visible range.

  \a end returns an iterator one higher than the highest visible data point. Same as before, \a end
  may also lie just outside of the visible range.

  if the plottable contains no data, both \a begin and \a end point to constEnd.
*/
    void QCPBars::getVisibleDataBounds(QCPBarsDataContainer::const_iterator &begin, QCPBarsDataContainer::const_iterator &end) const
    {
        if (!mKeyAxis)
        {
            qDebug() << Q_FUNC_INFO << "invalid key axis";
            begin = mDataContainer->constEnd();
            end = mDataContainer->constEnd();
            return;
        }
        if (mDataContainer->isEmpty())
        {
            begin = mDataContainer->constEnd();
            end = mDataContainer->constEnd();
            return;
        }

// get visible data range as QMap iterators
        begin = mDataContainer->findBegin(mKeyAxis.data()->range().lower);
        end = mDataContainer->findEnd(mKeyAxis.data()->range().upper);
        double lowerPixelBound = mKeyAxis.data()->coordToPixel(mKeyAxis.data()->range().lower);
        double upperPixelBound = mKeyAxis.data()->coordToPixel(mKeyAxis.data()->range().upper);
        bool isVisible = false;
// walk left from begin to find lower bar that actually is completely outside visible pixel range:
        QCPBarsDataContainer::const_iterator it = begin;
        while (it != mDataContainer->constBegin())
        {
            --it;
            const QRectF barRect = getBarRect(it->key, it->value);
            if (mKeyAxis.data()->orientation() == Qt::Horizontal)
                isVisible = ((!mKeyAxis.data()->rangeReversed() && barRect.right() >= lowerPixelBound) || (mKeyAxis.data()->rangeReversed() && barRect.left() <= lowerPixelBound));
            else // keyaxis is vertical
                isVisible = ((!mKeyAxis.data()->rangeReversed() && barRect.top() <= lowerPixelBound) || (mKeyAxis.data()->rangeReversed() && barRect.bottom() >= lowerPixelBound));
            if (isVisible)
                begin = it;
            else
                break;
        }
// walk right from ubound to find upper bar that actually is completely outside visible pixel range:
        it = end;
        while (it != mDataContainer->constEnd())
        {
            const QRectF barRect = getBarRect(it->key, it->value);
            if (mKeyAxis.data()->orientation() == Qt::Horizontal)
                isVisible = ((!mKeyAxis.data()->rangeReversed() && barRect.left() <= upperPixelBound) || (mKeyAxis.data()->rangeReversed() && barRect.right() >= upperPixelBound));
            else // keyaxis is vertical
                isVisible = ((!mKeyAxis.data()->rangeReversed() && barRect.bottom() >= upperPixelBound) || (mKeyAxis.data()->rangeReversed() && barRect.top() <= upperPixelBound));
            if (isVisible)
                end = it+1;
            else
                break;
            ++it;
        }
    }

/*! \internal

  Returns the rect in pixel coordinates of a single bar with the specified \a key and \a value. The
  rect is shifted according to the bar stacking (see \ref moveAbove) and base value (see \ref
  setBaseValue), and to have non-overlapping border lines with the bars stacked below.
*/
    QRectF QCPBars::getBarRect(double key, double value) const
    {
        QCPAxis *keyAxis = mKeyAxis.data();
        QCPAxis *valueAxis = mValueAxis.data();
        if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return {}; }

        double lowerPixelWidth, upperPixelWidth;
        getPixelWidth(key, lowerPixelWidth, upperPixelWidth);
        double base = getStackedBaseValue(key, value >= 0);
        double basePixel = valueAxis->coordToPixel(base);
        double valuePixel = valueAxis->coordToPixel(base+value);
        double keyPixel = keyAxis->coordToPixel(key);
        if (mBarsGroup)
            keyPixel += mBarsGroup->keyPixelOffset(this, key);
        double bottomOffset = (mBarBelow && mPen != Qt::NoPen ? 1 : 0)*(mPen.isCosmetic() ? 1 : mPen.widthF());
        bottomOffset += mBarBelow ? mStackingGap : 0;
        bottomOffset *= (value<0 ? -1 : 1)*valueAxis->pixelOrientation();
        if (qAbs(valuePixel-basePixel) <= qAbs(bottomOffset))
            bottomOffset = valuePixel-basePixel;
        if (keyAxis->orientation() == Qt::Horizontal)
        {
            return QRectF(QPointF(keyPixel+lowerPixelWidth, valuePixel), QPointF(keyPixel+upperPixelWidth, basePixel+bottomOffset)).normalized();
        } else
        {
            return QRectF(QPointF(basePixel+bottomOffset, keyPixel+lowerPixelWidth), QPointF(valuePixel, keyPixel+upperPixelWidth)).normalized();
        }
    }

/*! \internal

  This function is used to determine the width of the bar at coordinate \a key, according to the
  specified width (\ref setWidth) and width type (\ref setWidthType).

  The output parameters \a lower and \a upper return the number of pixels the bar extends to lower
  and higher keys, relative to the \a key coordinate (so with a non-reversed horizontal axis, \a
  lower is negative and \a upper positive).
*/
    void QCPBars::getPixelWidth(double key, double &lower, double &upper) const
    {
        lower = 0;
        upper = 0;
        switch (mWidthType)
        {
            case wtAbsolute:
            {
                upper = mWidth*0.5*mKeyAxis.data()->pixelOrientation();
                lower = -upper;
                break;
            }
            case wtAxisRectRatio:
            {
                if (mKeyAxis && mKeyAxis.data()->axisRect())
                {
                    if (mKeyAxis.data()->orientation() == Qt::Horizontal)
                        upper = mKeyAxis.data()->axisRect()->width()*mWidth*0.5*mKeyAxis.data()->pixelOrientation();
                    else
                        upper = mKeyAxis.data()->axisRect()->height()*mWidth*0.5*mKeyAxis.data()->pixelOrientation();
                    lower = -upper;
                } else
                    qDebug() << Q_FUNC_INFO << "No key axis or axis rect defined";
                break;
            }
            case wtPlotCoords:
            {
                if (mKeyAxis)
                {
                    double keyPixel = mKeyAxis.data()->coordToPixel(key);
                    upper = mKeyAxis.data()->coordToPixel(key+mWidth*0.5)-keyPixel;
                    lower = mKeyAxis.data()->coordToPixel(key-mWidth*0.5)-keyPixel;
// no need to qSwap(lower, higher) when range reversed, because higher/lower are gained by
// coordinate transform which includes range direction
                } else
                    qDebug() << Q_FUNC_INFO << "No key axis defined";
                break;
            }
        }
    }

/*! \internal

  This function is called to find at which value to start drawing the base of a bar at \a key, when
  it is stacked on top of another QCPBars (e.g. with \ref moveAbove).

  positive and negative bars are separated per stack (positive are stacked above baseValue upwards,
  negative are stacked below baseValue downwards). This can be indicated with \a positive. So if the
  bar for which we need the base value is negative, set \a positive to false.
*/
    double QCPBars::getStackedBaseValue(double key, bool positive) const
    {
        if (mBarBelow)
        {
            double max = 0; // don't initialize with mBaseValue here because only base value of bottom-most bar has meaning in a bar stack
// find bars of mBarBelow that are approximately at key and find largest one:
            double epsilon = qAbs(key)*(sizeof(key)==4 ? 1e-6 : 1e-14); // should be safe even when changed to use float at some point
            if (key == 0)
                epsilon = (sizeof(key)==4 ? 1e-6 : 1e-14);
            QCPBarsDataContainer::const_iterator it = mBarBelow.data()->mDataContainer->findBegin(key-epsilon);
            QCPBarsDataContainer::const_iterator itEnd = mBarBelow.data()->mDataContainer->findEnd(key+epsilon);
            while (it != itEnd)
            {
                if (it->key > key-epsilon && it->key < key+epsilon)
                {
                    if ((positive && it->value > max) ||
                        (!positive && it->value < max))
                        max = it->value;
                }
                ++it;
            }
// recurse down the bar-stack to find the total height:
            return max + mBarBelow.data()->getStackedBaseValue(key, positive);
        } else
            return mBaseValue;
    }

/*! \internal

  Connects \a below and \a above to each other via their mBarAbove/mBarBelow properties. The bar(s)
  currently above lower and below upper will become disconnected to lower/upper.

  If lower is zero, upper will be disconnected at the bottom.
  If upper is zero, lower will be disconnected at the top.
*/
    void QCPBars::connectBars(QCPBars *lower, QCPBars *upper)
    {
        if (!lower && !upper) return;

        if (!lower) // disconnect upper at bottom
        {
// disconnect old bar below upper:
            if (upper->mBarBelow && upper->mBarBelow.data()->mBarAbove.data() == upper)
                upper->mBarBelow.data()->mBarAbove = nullptr;
            upper->mBarBelow = nullptr;
        } else if (!upper) // disconnect lower at top
        {
// disconnect old bar above lower:
            if (lower->mBarAbove && lower->mBarAbove.data()->mBarBelow.data() == lower)
                lower->mBarAbove.data()->mBarBelow = nullptr;
            lower->mBarAbove = nullptr;
        } else // connect lower and upper
        {
// disconnect old bar above lower:
            if (lower->mBarAbove && lower->mBarAbove.data()->mBarBelow.data() == lower)
                lower->mBarAbove.data()->mBarBelow = nullptr;
// disconnect old bar below upper:
            if (upper->mBarBelow && upper->mBarBelow.data()->mBarAbove.data() == upper)
                upper->mBarBelow.data()->mBarAbove = nullptr;
            lower->mBarAbove = upper;
            upper->mBarBelow = lower;
        }
    }
} // QCP