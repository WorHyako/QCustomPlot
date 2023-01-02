#include "include/QCPColorMap.hpp"

using namespace QCP;

/*! \class QCPColorMap
  \brief A plottable representing a two-dimensional color map in a plot.

  \image html QCPColorMap.png

  The data is stored in the class \ref QCPColorMapData, which can be accessed via the data()
  method.

  A color map has three dimensions to represent a data point: The \a key dimension, the \a value
  dimension and the \a data dimension. As with other plottables such as graphs, \a key and \a value
  correspond to two orthogonal axes on the QCustomPlot surface that you specify in the QCPColorMap
  constructor. The \a data dimension however is encoded as the color of the point at (\a key, \a
  value).

  Set the number of points (or \a cells) in the key/value dimension via \ref
  QCPColorMapData::setSize. The plot coordinate range over which these points will be displayed is
  specified via \ref QCPColorMapData::setRange. The first cell will be centered on the lower range
  boundary and the last cell will be centered on the upper range boundary. The data can be set by
  either accessing the cells directly with QCPColorMapData::setCell or by addressing the cells via
  their plot coordinates with \ref QCPColorMapData::setData. If possible, you should prefer
  setCell, since it doesn't need to do any coordinate transformation and thus performs a bit
  better.

  The cell with index (0, 0) is at the bottom left, if the color map uses normal (i.e. not reversed)
  key and value axes.

  To show the user which colors correspond to which \a data values, a \ref QCPColorScale is
  typically placed to the right of the axis rect. See the documentation there for details on how to
  add and use a color scale.

  \section qcpcolormap-appearance Changing the appearance

  Most important to the appearance is the color gradient, which can be specified via \ref
  setGradient. See the documentation of \ref QCPColorGradient for details on configuring a color
  gradient.

  The \a data range that is mapped to the colors of the gradient can be specified with \ref
  setDataRange. To make the data range encompass the whole data set minimum to maximum, call \ref
  rescaleDataRange. If your data may contain NaN values, use \ref QCPColorGradient::setNanHandling
  to define how they are displayed.

  \section qcpcolormap-transparency Transparency

  Transparency in color maps can be achieved by two mechanisms. On one hand, you can specify alpha
  values for color stops of the \ref QCPColorGradient, via the regular QColor interface. This will
  cause the color map data which gets mapped to colors around those color stops to appear with the
  accordingly interpolated transparency.

  On the other hand you can also directly apply an alpha value to each cell independent of its
  data, by using the alpha map feature of \ref QCPColorMapData. The relevant methods are \ref
  QCPColorMapData::setAlpha, QCPColorMapData::fillAlpha and \ref QCPColorMapData::clearAlpha().

  The two transparencies will be joined together in the plot and otherwise not interfere with each
  other. They are mixed in a multiplicative matter, so an alpha of e.g. 50% (128/255) in both modes
  simultaneously, will result in a total transparency of 25% (64/255).

  \section qcpcolormap-usage Usage

  Like all data representing objects in QCustomPlot, the QCPColorMap is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::removePlottable, etc.)

  Usually, you first create an instance:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolormap-creation-1
  which registers it with the QCustomPlot instance of the passed axes. Note that this QCustomPlot instance takes
  ownership of the plottable, so do not delete it manually but use QCustomPlot::removePlottable() instead.
  The newly created plottable can be modified, e.g.:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpcolormap-creation-2

  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::setData method as it uses a linear transformation to
  determine the cell index. Rather directly access the cell index with \ref
  QCPColorMapData::setCell.
*/

/* start documentation of inline functions */

/*! \fn QCPColorMapData *QCPColorMap::data() const

  Returns a pointer to the internal data storage of type \ref QCPColorMapData. Access this to
  modify data points (cells) and the color map key/value range.

  \see setData
*/

/* end documentation of inline functions */

/* start documentation of signals */

/*! \fn void QCPColorMap::dataRangeChanged(const QCPRange &newRange);

  This signal is emitted when the data range changes.

  \see setDataRange
*/

/*! \fn void QCPColorMap::dataScaleTypeChanged(QCPAxis::ScaleType scaleType);

  This signal is emitted when the data scale type changes.

  \see setDataScaleType
*/

/*! \fn void QCPColorMap::gradientChanged(const QCPColorGradient &newGradient);

  This signal is emitted when the gradient changes.

  \see setGradient
*/

/* end documentation of signals */

/*!
  Constructs a color map with the specified \a keyAxis and \a valueAxis.

  The created QCPColorMap is automatically registered with the QCustomPlot instance inferred from
  \a keyAxis. This QCustomPlot instance takes ownership of the QCPColorMap, so do not delete it
  manually but use QCustomPlot::removePlottable() instead.
*/
QCPColorMap::QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis) :
QCPAbstractPlottable(keyAxis, valueAxis),
mDataScaleType(QCPAxis::stLinear),
mMapData(new QCPColorMapData(10, 10, QCPRange(0, 5), QCPRange(0, 5))),
mGradient(QCPColorGradient::gpCold),
mInterpolate(true),
mTightBoundary(false),
mMapImageInvalidated(true)
{
}

QCPColorMap::~QCPColorMap()
{
    delete mMapData;
}

/*!
  Replaces the current \ref data with the provided \a data.

  If \a copy is set to true, the \a data object will only be copied. if false, the color map
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
*/
void QCPColorMap::setData(QCPColorMapData *data, bool copy)
{
    if (mMapData == data)
    {
        qDebug() << Q_FUNC_INFO << "The data pointer is already in (and owned by) this plottable" << reinterpret_cast<quintptr>(data);
        return;
    }
    if (copy)
    {
        *mMapData = *data;
    } else
    {
        delete mMapData;
        mMapData = data;
    }
    mMapImageInvalidated = true;
}

/*!
  Sets the data range of this color map to \a dataRange. The data range defines which data values
  are mapped to the color gradient.

  To make the data range span the full range of the data set, use \ref rescaleDataRange.

  \see QCPColorScale::setDataRange
*/
void QCPColorMap::setDataRange(const QCPRange &dataRange)
{
    if (!QCPRange::validRange(dataRange)) return;
    if (mDataRange.lower != dataRange.lower || mDataRange.upper != dataRange.upper)
    {
        if (mDataScaleType == QCPAxis::stLogarithmic)
            mDataRange = dataRange.sanitizedForLogScale();
        else
            mDataRange = dataRange.sanitizedForLinScale();
        mMapImageInvalidated = true;
        emit dataRangeChanged(mDataRange);
    }
}

/*!
  Sets whether the data is correlated with the color gradient linearly or logarithmically.

  \see QCPColorScale::setDataScaleType
*/
void QCPColorMap::setDataScaleType(QCPAxis::ScaleType scaleType)
{
    if (mDataScaleType != scaleType)
    {
        mDataScaleType = scaleType;
        mMapImageInvalidated = true;
        emit dataScaleTypeChanged(mDataScaleType);
        if (mDataScaleType == QCPAxis::stLogarithmic)
            setDataRange(mDataRange.sanitizedForLogScale());
    }
}

/*!
  Sets the color gradient that is used to represent the data. For more details on how to create an
  own gradient or use one of the preset gradients, see \ref QCPColorGradient.

  The colors defined by the gradient will be used to represent data values in the currently set
  data range, see \ref setDataRange. Data points that are outside this data range will either be
  colored uniformly with the respective gradient boundary color, or the gradient will repeat,
  depending on \ref QCPColorGradient::setPeriodic.

  \see QCPColorScale::setGradient
*/
void QCPColorMap::setGradient(const QCPColorGradient &gradient)
{
    if (mGradient != gradient)
    {
        mGradient = gradient;
        mMapImageInvalidated = true;
        emit gradientChanged(mGradient);
    }
}

/*!
  Sets whether the color map image shall use bicubic interpolation when displaying the color map
  shrinked or expanded, and not at a 1:1 pixel-to-data scale.

  \image html QCPColorMap-interpolate.png "A 10*10 color map, with interpolation and without interpolation enabled"
*/
void QCPColorMap::setInterpolate(bool enabled)
{
    mInterpolate = enabled;
    mMapImageInvalidated = true; // because oversampling factors might need to change
}

/*!
  Sets whether the outer most data rows and columns are clipped to the specified key and value
  range (see \ref QCPColorMapData::setKeyRange, \ref QCPColorMapData::setValueRange).

  if \a enabled is set to false, the data points at the border of the color map are drawn with the
  same width and height as all other data points. Since the data points are represented by
  rectangles of one color centered on the data coordinate, this means that the shown color map
  extends by half a data point over the specified key/value range in each direction.

  \image html QCPColorMap-tightboundary.png "A color map, with tight boundary enabled and disabled"
*/
void QCPColorMap::setTightBoundary(bool enabled)
{
    mTightBoundary = enabled;
}

/*!
  Associates the color scale \a colorScale with this color map.

  This means that both the color scale and the color map synchronize their gradient, data range and
  data scale type (\ref setGradient, \ref setDataRange, \ref setDataScaleType). Multiple color maps
  can be associated with one single color scale. This causes the color maps to also synchronize
  those properties, via the mutual color scale.

  This function causes the color map to adopt the current color gradient, data range and data scale
  type of \a colorScale. After this call, you may change these properties at either the color map
  or the color scale, and the setting will be applied to both.

  Pass \c nullptr as \a colorScale to disconnect the color scale from this color map again.
*/
void QCPColorMap::setColorScale(QCPColorScale *colorScale)
{
    if (mColorScale) // unconnect signals from old color scale
    {
        disconnect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
        disconnect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
        disconnect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
        disconnect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
        disconnect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
        disconnect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
    }
    mColorScale = colorScale;
    if (mColorScale) // connect signals to new color scale
    {
        setGradient(mColorScale.data()->gradient());
        setDataRange(mColorScale.data()->dataRange());
        setDataScaleType(mColorScale.data()->dataScaleType());
        connect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
        connect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
        connect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
        connect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
        connect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
        connect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
    }
}

/*!
  Sets the data range (\ref setDataRange) to span the minimum and maximum values that occur in the
  current data set. This corresponds to the \ref rescaleKeyAxis or \ref rescaleValueAxis methods,
  only for the third data dimension of the color map.

  The minimum and maximum values of the data set are buffered in the internal QCPColorMapData
  instance (\ref data). As data is updated via its \ref QCPColorMapData::setCell or \ref
  QCPColorMapData::setData, the buffered minimum and maximum values are updated, too. For
  performance reasons, however, they are only updated in an expanding fashion. So the buffered
  maximum can only increase and the buffered minimum can only decrease. In consequence, changes to
  the data that actually lower the maximum of the data set (by overwriting the cell holding the
  current maximum with a smaller value), aren't recognized and the buffered maximum overestimates
  the true maximum of the data set. The same happens for the buffered minimum. To recalculate the
  true minimum and maximum by explicitly looking at each cell, the method
  QCPColorMapData::recalculateDataBounds can be used. For convenience, setting the parameter \a
  recalculateDataBounds calls this method before setting the data range to the buffered minimum and
  maximum.

  \see setDataRange
*/
void QCPColorMap::rescaleDataRange(bool recalculateDataBounds)
{
    if (recalculateDataBounds)
        mMapData->recalculateDataBounds();
    setDataRange(mMapData->dataBounds());
}

/*!
  Takes the current appearance of the color map and updates the legend icon, which is used to
  represent this color map in the legend (see \ref QCPLegend).

  The \a transformMode specifies whether the rescaling is done by a faster, low quality image
  scaling algorithm (Qt::FastTransformation) or by a slower, higher quality algorithm
  (Qt::SmoothTransformation).

  The current color map appearance is scaled down to \a thumbSize. Ideally, this should be equal to
  the size of the legend icon (see \ref QCPLegend::setIconSize). If it isn't exactly the configured
  legend icon size, the thumb will be rescaled during drawing of the legend item.

  \see setDataRange
*/
void QCPColorMap::updateLegendIcon(Qt::TransformationMode transformMode, const QSize &thumbSize)
{
    if (mMapImage.isNull() && !data()->isEmpty())
        updateMapImage(); // try to update map image if it's null (happens if no draw has happened yet)

    if (!mMapImage.isNull()) // might still be null, e.g. if data is empty, so check here again
    {
        bool mirrorX = (keyAxis()->orientation() == Qt::Horizontal ? keyAxis() : valueAxis())->rangeReversed();
        bool mirrorY = (valueAxis()->orientation() == Qt::Vertical ? valueAxis() : keyAxis())->rangeReversed();
        mLegendIcon = QPixmap::fromImage(mMapImage.mirrored(mirrorX, mirrorY)).scaled(thumbSize, Qt::KeepAspectRatio, transformMode);
    }
}

/* inherits documentation from base class */
double QCPColorMap::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(details)
    if ((onlySelectable && mSelectable == QCP::stNone) || mMapData->isEmpty())
        return -1;
    if (!mKeyAxis || !mValueAxis)
        return -1;

    if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()) || mParentPlot->interactions().testFlag(QCP::iSelectPlottablesBeyondAxisRect))
    {
        double posKey, posValue;
        pixelsToCoords(pos, posKey, posValue);
        if (mMapData->keyRange().contains(posKey) && mMapData->valueRange().contains(posValue))
        {
            if (details)
                details->setValue(QCPDataSelection(QCPDataRange(0, 1))); // temporary solution, to facilitate whole-plottable selection. Replace in future version with segmented 2D selection.
            return mParentPlot->selectionTolerance()*0.99;
        }
    }
    return -1;
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain) const
{
    foundRange = true;
    QCPRange result = mMapData->keyRange();
    result.normalize();
    if (inSignDomain == QCP::sdPositive)
    {
        if (result.lower <= 0 && result.upper > 0)
            result.lower = result.upper*1e-3;
        else if (result.lower <= 0 && result.upper <= 0)
            foundRange = false;
    } else if (inSignDomain == QCP::sdNegative)
    {
        if (result.upper >= 0 && result.lower < 0)
            result.upper = result.lower*1e-3;
        else if (result.upper >= 0 && result.lower >= 0)
            foundRange = false;
    }
    return result;
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getValueRange(bool &foundRange, QCP::SignDomain inSignDomain, const QCPRange &inKeyRange) const
{
    if (inKeyRange != QCPRange())
    {
        if (mMapData->keyRange().upper < inKeyRange.lower || mMapData->keyRange().lower > inKeyRange.upper)
        {
            foundRange = false;
            return {};
        }
    }

    foundRange = true;
    QCPRange result = mMapData->valueRange();
    result.normalize();
    if (inSignDomain == QCP::sdPositive)
    {
        if (result.lower <= 0 && result.upper > 0)
            result.lower = result.upper*1e-3;
        else if (result.lower <= 0 && result.upper <= 0)
            foundRange = false;
    } else if (inSignDomain == QCP::sdNegative)
    {
        if (result.upper >= 0 && result.lower < 0)
            result.upper = result.lower*1e-3;
        else if (result.upper >= 0 && result.lower >= 0)
            foundRange = false;
    }
    return result;
}

/*! \internal

  Updates the internal map image buffer by going through the internal \ref QCPColorMapData and
  turning the data values into color pixels with \ref QCPColorGradient::colorize.

  This method is called by \ref QCPColorMap::draw if either the data has been modified or the map image
  has been invalidated for a different reason (e.g. a change of the data range with \ref
  setDataRange).

  If the map cell count is low, the image created will be oversampled in order to avoid a
  QPainter::drawImage bug which makes inner pixel boundaries jitter when stretch-drawing images
  without smooth transform enabled. Accordingly, oversampling isn't performed if \ref
  setInterpolate is true.
*/
void QCPColorMap::updateMapImage()
{
    QCPAxis *keyAxis = mKeyAxis.data();
    if (!keyAxis) return;
    if (mMapData->isEmpty()) return;

    const QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    const int keySize = mMapData->keySize();
    const int valueSize = mMapData->valueSize();
    int keyOversamplingFactor = mInterpolate ? 1 : int(1.0+100.0/double(keySize)); // make mMapImage have at least size 100, factor becomes 1 if size > 200 or interpolation is on
    int valueOversamplingFactor = mInterpolate ? 1 : int(1.0+100.0/double(valueSize)); // make mMapImage have at least size 100, factor becomes 1 if size > 200 or interpolation is on

// resize mMapImage to correct dimensions including possible oversampling factors, according to key/value axes orientation:
    if (keyAxis->orientation() == Qt::Horizontal && (mMapImage.width() != keySize*keyOversamplingFactor || mMapImage.height() != valueSize*valueOversamplingFactor))
        mMapImage = QImage(QSize(keySize*keyOversamplingFactor, valueSize*valueOversamplingFactor), format);
    else if (keyAxis->orientation() == Qt::Vertical && (mMapImage.width() != valueSize*valueOversamplingFactor || mMapImage.height() != keySize*keyOversamplingFactor))
        mMapImage = QImage(QSize(valueSize*valueOversamplingFactor, keySize*keyOversamplingFactor), format);

    if (mMapImage.isNull())
    {
        qDebug() << Q_FUNC_INFO << "Couldn't create map image (possibly too large for memory)";
        mMapImage = QImage(QSize(10, 10), format);
        mMapImage.fill(Qt::black);
    } else
    {
        QImage *localMapImage = &mMapImage; // this is the image on which the colorization operates. Either the final mMapImage, or if we need oversampling, mUndersampledMapImage
        if (keyOversamplingFactor > 1 || valueOversamplingFactor > 1)
        {
// resize undersampled map image to actual key/value cell sizes:
            if (keyAxis->orientation() == Qt::Horizontal && (mUndersampledMapImage.width() != keySize || mUndersampledMapImage.height() != valueSize))
                mUndersampledMapImage = QImage(QSize(keySize, valueSize), format);
            else if (keyAxis->orientation() == Qt::Vertical && (mUndersampledMapImage.width() != valueSize || mUndersampledMapImage.height() != keySize))
                mUndersampledMapImage = QImage(QSize(valueSize, keySize), format);
            localMapImage = &mUndersampledMapImage; // make the colorization run on the undersampled image
        } else if (!mUndersampledMapImage.isNull())
            mUndersampledMapImage = QImage(); // don't need oversampling mechanism anymore (map size has changed) but mUndersampledMapImage still has nonzero size, free it

        const double *rawData = mMapData->mData;
        const unsigned char *rawAlpha = mMapData->mAlpha;
        if (keyAxis->orientation() == Qt::Horizontal)
        {
            const int lineCount = valueSize;
            const int rowCount = keySize;
            for (int line=0; line<lineCount; ++line)
            {
                QRgb* pixels = reinterpret_cast<QRgb*>(localMapImage->scanLine(lineCount-1-line)); // invert scanline index because QImage counts scanlines from top, but our vertical index counts from bottom (mathematical coordinate system)
                if (rawAlpha)
                    mGradient.colorize(rawData+line*rowCount, rawAlpha+line*rowCount, mDataRange, pixels, rowCount, 1, mDataScaleType==QCPAxis::stLogarithmic);
                else
                    mGradient.colorize(rawData+line*rowCount, mDataRange, pixels, rowCount, 1, mDataScaleType==QCPAxis::stLogarithmic);
            }
        } else // keyAxis->orientation() == Qt::Vertical
        {
            const int lineCount = keySize;
            const int rowCount = valueSize;
            for (int line=0; line<lineCount; ++line)
            {
                QRgb* pixels = reinterpret_cast<QRgb*>(localMapImage->scanLine(lineCount-1-line)); // invert scanline index because QImage counts scanlines from top, but our vertical index counts from bottom (mathematical coordinate system)
                if (rawAlpha)
                    mGradient.colorize(rawData+line, rawAlpha+line, mDataRange, pixels, rowCount, lineCount, mDataScaleType==QCPAxis::stLogarithmic);
                else
                    mGradient.colorize(rawData+line, mDataRange, pixels, rowCount, lineCount, mDataScaleType==QCPAxis::stLogarithmic);
            }
        }

        if (keyOversamplingFactor > 1 || valueOversamplingFactor > 1)
        {
            if (keyAxis->orientation() == Qt::Horizontal)
                mMapImage = mUndersampledMapImage.scaled(keySize*keyOversamplingFactor, valueSize*valueOversamplingFactor, Qt::IgnoreAspectRatio, Qt::FastTransformation);
            else
                mMapImage = mUndersampledMapImage.scaled(valueSize*valueOversamplingFactor, keySize*keyOversamplingFactor, Qt::IgnoreAspectRatio, Qt::FastTransformation);
        }
    }
    mMapData->mDataModified = false;
    mMapImageInvalidated = false;
}

/* inherits documentation from base class */
void QCPColorMap::draw(QCPPainter *painter)
{
    if (mMapData->isEmpty()) return;
    if (!mKeyAxis || !mValueAxis) return;
    applyDefaultAntialiasingHint(painter);

    if (mMapData->mDataModified || mMapImageInvalidated)
        updateMapImage();

// use buffer if painting vectorized (PDF):
    const bool useBuffer = painter->modes().testFlag(QCPPainter::pmVectorized);
    QCPPainter *localPainter = painter; // will be redirected to paint on mapBuffer if painting vectorized
    QRectF mapBufferTarget; // the rect in absolute widget coordinates where the visible map portion/buffer will end up in
    QPixmap mapBuffer;
    if (useBuffer)
    {
        const double mapBufferPixelRatio = 3; // factor by which DPI is increased in embedded bitmaps
        mapBufferTarget = painter->clipRegion().boundingRect();
        mapBuffer = QPixmap((mapBufferTarget.size()*mapBufferPixelRatio).toSize());
        mapBuffer.fill(Qt::transparent);
        localPainter = new QCPPainter(&mapBuffer);
        localPainter->scale(mapBufferPixelRatio, mapBufferPixelRatio);
        localPainter->translate(-mapBufferTarget.topLeft());
    }

    QRectF imageRect = QRectF(coordsToPixels(mMapData->keyRange().lower, mMapData->valueRange().lower),
                              coordsToPixels(mMapData->keyRange().upper, mMapData->valueRange().upper)).normalized();
// extend imageRect to contain outer halves/quarters of bordering/cornering pixels (cells are centered on map range boundary):
    double halfCellWidth = 0; // in pixels
    double halfCellHeight = 0; // in pixels
    if (keyAxis()->orientation() == Qt::Horizontal)
    {
        if (mMapData->keySize() > 1)
            halfCellWidth = 0.5*imageRect.width()/double(mMapData->keySize()-1);
        if (mMapData->valueSize() > 1)
            halfCellHeight = 0.5*imageRect.height()/double(mMapData->valueSize()-1);
    } else // keyAxis orientation is Qt::Vertical
    {
        if (mMapData->keySize() > 1)
            halfCellHeight = 0.5*imageRect.height()/double(mMapData->keySize()-1);
        if (mMapData->valueSize() > 1)
            halfCellWidth = 0.5*imageRect.width()/double(mMapData->valueSize()-1);
    }
    imageRect.adjust(-halfCellWidth, -halfCellHeight, halfCellWidth, halfCellHeight);
    const bool mirrorX = (keyAxis()->orientation() == Qt::Horizontal ? keyAxis() : valueAxis())->rangeReversed();
    const bool mirrorY = (valueAxis()->orientation() == Qt::Vertical ? valueAxis() : keyAxis())->rangeReversed();
    const bool smoothBackup = localPainter->renderHints().testFlag(QPainter::SmoothPixmapTransform);
    localPainter->setRenderHint(QPainter::SmoothPixmapTransform, mInterpolate);
    QRegion clipBackup;
    if (mTightBoundary)
    {
        clipBackup = localPainter->clipRegion();
        QRectF tightClipRect = QRectF(coordsToPixels(mMapData->keyRange().lower, mMapData->valueRange().lower),
                                      coordsToPixels(mMapData->keyRange().upper, mMapData->valueRange().upper)).normalized();
        localPainter->setClipRect(tightClipRect, Qt::IntersectClip);
    }
    localPainter->drawImage(imageRect, mMapImage.mirrored(mirrorX, mirrorY));
    if (mTightBoundary)
        localPainter->setClipRegion(clipBackup);
    localPainter->setRenderHint(QPainter::SmoothPixmapTransform, smoothBackup);

    if (useBuffer) // localPainter painted to mapBuffer, so now draw buffer with original painter
    {
        delete localPainter;
        painter->drawPixmap(mapBufferTarget.toRect(), mapBuffer);
    }
}

/* inherits documentation from base class */
void QCPColorMap::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
    applyDefaultAntialiasingHint(painter);
// draw map thumbnail:
    if (!mLegendIcon.isNull())
    {
        QPixmap scaledIcon = mLegendIcon.scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::FastTransformation);
        QRectF iconRect = QRectF(0, 0, scaledIcon.width(), scaledIcon.height());
        iconRect.moveCenter(rect.center());
        painter->drawPixmap(iconRect.topLeft(), scaledIcon);
    }
/*
  // draw frame:
  painter->setBrush(Qt::NoBrush);
  painter->setPen(Qt::black);
  painter->drawRect(rect.adjusted(1, 1, 0, 0));
  */
}