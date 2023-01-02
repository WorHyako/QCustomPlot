#include "include/QCPAxisPainterPrivate.hpp"

namespace QCP {
/*! \class QCPAxisPainterPrivate

  \internal
  \brief (Private)

  This is a private class and not part of the public QCustomPlot interface.

  It is used by QCPAxis to do the low-level drawing of axis backbone, tick marks, tick labels and
  axis label. It also buffers the labels to reduce replot times. The parameters are configured by
  directly accessing the public member variables.
*/

/*!
  Constructs a QCPAxisPainterPrivate instance. Make sure to not create a new instance on every
  redraw, to utilize the caching mechanisms.
*/
    QCPAxisPainterPrivate::QCPAxisPainterPrivate(QCustomPlot *parentPlot) :
            type(QCPAxis::atLeft),
            basePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
            lowerEnding(QCPLineEnding::esNone),
            upperEnding(QCPLineEnding::esNone),
            labelPadding(0),
            tickLabelPadding(0),
            tickLabelRotation(0),
            tickLabelSide(QCPAxis::lsOutside),
            substituteExponent(true),
            numberMultiplyCross(false),
            tickLengthIn(5),
            tickLengthOut(0),
            subTickLengthIn(2),
            subTickLengthOut(0),
            tickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
            subTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
            offset(0),
            abbreviateDecimalPowers(false),
            reversedEndings(false),
            mParentPlot(parentPlot),
            mLabelCache(16) // cache at most 16 (tick) labels
    {
    }

    QCPAxisPainterPrivate::~QCPAxisPainterPrivate()
    {
    }

/*! \internal

  Draws the axis with the specified \a painter.

  The selection boxes (mAxisSelectionBox, mTickLabelsSelectionBox, mLabelSelectionBox) are set
  here, too.
*/
    void QCPAxisPainterPrivate::draw(QCPPainter *painter)
    {
        QByteArray newHash = generateLabelParameterHash();
        if (newHash != mLabelParameterHash)
        {
            mLabelCache.clear();
            mLabelParameterHash = newHash;
        }

        QPoint origin;
        switch (type)
        {
            case QCPAxis::atLeft:   origin = axisRect.bottomLeft() +QPoint(-offset, 0); break;
            case QCPAxis::atRight:  origin = axisRect.bottomRight()+QPoint(+offset, 0); break;
            case QCPAxis::atTop:    origin = axisRect.topLeft()    +QPoint(0, -offset); break;
            case QCPAxis::atBottom: origin = axisRect.bottomLeft() +QPoint(0, +offset); break;
        }

        double xCor = 0, yCor = 0; // paint system correction, for pixel exact matches (affects baselines and ticks of top/right axes)
        switch (type)
        {
            case QCPAxis::atTop: yCor = -1; break;
            case QCPAxis::atRight: xCor = 1; break;
            default: break;
        }
        int margin = 0;
// draw baseline:
        QLineF baseLine;
        painter->setPen(basePen);
        if (QCPAxis::orientation(type) == Qt::Horizontal)
            baseLine.setPoints(origin+QPointF(xCor, yCor), origin+QPointF(axisRect.width()+xCor, yCor));
        else
            baseLine.setPoints(origin+QPointF(xCor, yCor), origin+QPointF(xCor, -axisRect.height()+yCor));
        if (reversedEndings)
            baseLine = QLineF(baseLine.p2(), baseLine.p1()); // won't make a difference for line itself, but for line endings later
        painter->drawLine(baseLine);

// draw ticks:
        if (!tickPositions.isEmpty())
        {
            painter->setPen(tickPen);
            int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1; // direction of ticks ("inward" is right for left axis and left for right axis)
            if (QCPAxis::orientation(type) == Qt::Horizontal)
            {
                foreach (double tickPos, tickPositions)
                painter->drawLine(QLineF(tickPos+xCor, origin.y()-tickLengthOut*tickDir+yCor, tickPos+xCor, origin.y()+tickLengthIn*tickDir+yCor));
            } else
            {
                foreach (double tickPos, tickPositions)
                painter->drawLine(QLineF(origin.x()-tickLengthOut*tickDir+xCor, tickPos+yCor, origin.x()+tickLengthIn*tickDir+xCor, tickPos+yCor));
            }
        }

// draw subticks:
        if (!subTickPositions.isEmpty())
        {
            painter->setPen(subTickPen);
// direction of ticks ("inward" is right for left axis and left for right axis)
            int tickDir = (type == QCPAxis::atBottom || type == QCPAxis::atRight) ? -1 : 1;
            if (QCPAxis::orientation(type) == Qt::Horizontal)
            {
                foreach (double subTickPos, subTickPositions)
                painter->drawLine(QLineF(subTickPos+xCor, origin.y()-subTickLengthOut*tickDir+yCor, subTickPos+xCor, origin.y()+subTickLengthIn*tickDir+yCor));
            } else
            {
                foreach (double subTickPos, subTickPositions)
                painter->drawLine(QLineF(origin.x()-subTickLengthOut*tickDir+xCor, subTickPos+yCor, origin.x()+subTickLengthIn*tickDir+xCor, subTickPos+yCor));
            }
        }
        margin += qMax(0, qMax(tickLengthOut, subTickLengthOut));

// draw axis base endings:
        bool antialiasingBackup = painter->antialiasing();
        painter->setAntialiasing(true); // always want endings to be antialiased, even if base and ticks themselves aren't
        painter->setBrush(QBrush(basePen.color()));
        QCPVector2D baseLineVector(baseLine.dx(), baseLine.dy());
        if (lowerEnding.style() != QCPLineEnding::esNone)
            lowerEnding.draw(painter, QCPVector2D(baseLine.p1())-baseLineVector.normalized()*lowerEnding.realLength()*(lowerEnding.inverted()?-1:1), -baseLineVector);
        if (upperEnding.style() != QCPLineEnding::esNone)
            upperEnding.draw(painter, QCPVector2D(baseLine.p2())+baseLineVector.normalized()*upperEnding.realLength()*(upperEnding.inverted()?-1:1), baseLineVector);
        painter->setAntialiasing(antialiasingBackup);

// tick labels:
        QRect oldClipRect;
        if (tickLabelSide == QCPAxis::lsInside) // if using inside labels, clip them to the axis rect
        {
            oldClipRect = painter->clipRegion().boundingRect();
            painter->setClipRect(axisRect);
        }
        QSize tickLabelsSize(0, 0); // size of largest tick label, for offset calculation of axis label
        if (!tickLabels.isEmpty())
        {
            if (tickLabelSide == QCPAxis::lsOutside)
                margin += tickLabelPadding;
            painter->setFont(tickLabelFont);
            painter->setPen(QPen(tickLabelColor));
            const int maxLabelIndex = qMin(tickPositions.size(), tickLabels.size());
            int distanceToAxis = margin;
            if (tickLabelSide == QCPAxis::lsInside)
                distanceToAxis = -(qMax(tickLengthIn, subTickLengthIn)+tickLabelPadding);
            for (int i=0; i<maxLabelIndex; ++i)
                placeTickLabel(painter, tickPositions.at(i), distanceToAxis, tickLabels.at(i), &tickLabelsSize);
            if (tickLabelSide == QCPAxis::lsOutside)
                margin += (QCPAxis::orientation(type) == Qt::Horizontal) ? tickLabelsSize.height() : tickLabelsSize.width();
        }
        if (tickLabelSide == QCPAxis::lsInside)
            painter->setClipRect(oldClipRect);

// axis label:
        QRect labelBounds;
        if (!label.isEmpty())
        {
            margin += labelPadding;
            painter->setFont(labelFont);
            painter->setPen(QPen(labelColor));
            labelBounds = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip, label);
            if (type == QCPAxis::atLeft)
            {
                QTransform oldTransform = painter->transform();
                painter->translate((origin.x()-margin-labelBounds.height()), origin.y());
                painter->rotate(-90);
                painter->drawText(0, 0, axisRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
                painter->setTransform(oldTransform);
            }
            else if (type == QCPAxis::atRight)
            {
                QTransform oldTransform = painter->transform();
                painter->translate((origin.x()+margin+labelBounds.height()), origin.y()-axisRect.height());
                painter->rotate(90);
                painter->drawText(0, 0, axisRect.height(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
                painter->setTransform(oldTransform);
            }
            else if (type == QCPAxis::atTop)
                painter->drawText(origin.x(), origin.y()-margin-labelBounds.height(), axisRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
            else if (type == QCPAxis::atBottom)
                painter->drawText(origin.x(), origin.y()+margin, axisRect.width(), labelBounds.height(), Qt::TextDontClip | Qt::AlignCenter, label);
        }

// set selection boxes:
        int selectionTolerance = 0;
        if (mParentPlot)
            selectionTolerance = mParentPlot->selectionTolerance();
        else
            qDebug() << Q_FUNC_INFO << "mParentPlot is null";
        int selAxisOutSize = qMax(qMax(tickLengthOut, subTickLengthOut), selectionTolerance);
        int selAxisInSize = selectionTolerance;
        int selTickLabelSize;
        int selTickLabelOffset;
        if (tickLabelSide == QCPAxis::lsOutside)
        {
            selTickLabelSize = (QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
            selTickLabelOffset = qMax(tickLengthOut, subTickLengthOut)+tickLabelPadding;
        } else
        {
            selTickLabelSize = -(QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width());
            selTickLabelOffset = -(qMax(tickLengthIn, subTickLengthIn)+tickLabelPadding);
        }
        int selLabelSize = labelBounds.height();
        int selLabelOffset = qMax(tickLengthOut, subTickLengthOut)+(!tickLabels.isEmpty() && tickLabelSide == QCPAxis::lsOutside ? tickLabelPadding+selTickLabelSize : 0)+labelPadding;
        if (type == QCPAxis::atLeft)
        {
            mAxisSelectionBox.setCoords(origin.x()-selAxisOutSize, axisRect.top(), origin.x()+selAxisInSize, axisRect.bottom());
            mTickLabelsSelectionBox.setCoords(origin.x()-selTickLabelOffset-selTickLabelSize, axisRect.top(), origin.x()-selTickLabelOffset, axisRect.bottom());
            mLabelSelectionBox.setCoords(origin.x()-selLabelOffset-selLabelSize, axisRect.top(), origin.x()-selLabelOffset, axisRect.bottom());
        } else if (type == QCPAxis::atRight)
        {
            mAxisSelectionBox.setCoords(origin.x()-selAxisInSize, axisRect.top(), origin.x()+selAxisOutSize, axisRect.bottom());
            mTickLabelsSelectionBox.setCoords(origin.x()+selTickLabelOffset+selTickLabelSize, axisRect.top(), origin.x()+selTickLabelOffset, axisRect.bottom());
            mLabelSelectionBox.setCoords(origin.x()+selLabelOffset+selLabelSize, axisRect.top(), origin.x()+selLabelOffset, axisRect.bottom());
        } else if (type == QCPAxis::atTop)
        {
            mAxisSelectionBox.setCoords(axisRect.left(), origin.y()-selAxisOutSize, axisRect.right(), origin.y()+selAxisInSize);
            mTickLabelsSelectionBox.setCoords(axisRect.left(), origin.y()-selTickLabelOffset-selTickLabelSize, axisRect.right(), origin.y()-selTickLabelOffset);
            mLabelSelectionBox.setCoords(axisRect.left(), origin.y()-selLabelOffset-selLabelSize, axisRect.right(), origin.y()-selLabelOffset);
        } else if (type == QCPAxis::atBottom)
        {
            mAxisSelectionBox.setCoords(axisRect.left(), origin.y()-selAxisInSize, axisRect.right(), origin.y()+selAxisOutSize);
            mTickLabelsSelectionBox.setCoords(axisRect.left(), origin.y()+selTickLabelOffset+selTickLabelSize, axisRect.right(), origin.y()+selTickLabelOffset);
            mLabelSelectionBox.setCoords(axisRect.left(), origin.y()+selLabelOffset+selLabelSize, axisRect.right(), origin.y()+selLabelOffset);
        }
        mAxisSelectionBox = mAxisSelectionBox.normalized();
        mTickLabelsSelectionBox = mTickLabelsSelectionBox.normalized();
        mLabelSelectionBox = mLabelSelectionBox.normalized();
// draw hitboxes for debug purposes:
//painter->setBrush(Qt::NoBrush);
//painter->drawRects(QVector<QRect>() << mAxisSelectionBox << mTickLabelsSelectionBox << mLabelSelectionBox);
    }

/*! \internal

  Returns the size ("margin" in QCPAxisRect context, so measured perpendicular to the axis backbone
  direction) needed to fit the axis.
*/
    int QCPAxisPainterPrivate::size()
    {
        int result = 0;

        QByteArray newHash = generateLabelParameterHash();
        if (newHash != mLabelParameterHash)
        {
            mLabelCache.clear();
            mLabelParameterHash = newHash;
        }

// get length of tick marks pointing outwards:
        if (!tickPositions.isEmpty())
            result += qMax(0, qMax(tickLengthOut, subTickLengthOut));

// calculate size of tick labels:
        if (tickLabelSide == QCPAxis::lsOutside)
        {
            QSize tickLabelsSize(0, 0);
            if (!tickLabels.isEmpty())
            {
                foreach (const QString &tickLabel, tickLabels)
                getMaxTickLabelSize(tickLabelFont, tickLabel, &tickLabelsSize);
                result += QCPAxis::orientation(type) == Qt::Horizontal ? tickLabelsSize.height() : tickLabelsSize.width();
                result += tickLabelPadding;
            }
        }

// calculate size of axis label (only height needed, because left/right labels are rotated by 90 degrees):
        if (!label.isEmpty())
        {
            QFontMetrics fontMetrics(labelFont);
            QRect bounds;
            bounds = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | Qt::AlignHCenter | Qt::AlignVCenter, label);
            result += bounds.height() + labelPadding;
        }

        return result;
    }

/*! \internal

  Clears the internal label cache. Upon the next \ref draw, all labels will be created new. This
  method is called automatically in \ref draw, if any parameters have changed that invalidate the
  cached labels, such as font, color, etc.
*/
    void QCPAxisPainterPrivate::clearCache()
    {
        mLabelCache.clear();
    }

/*! \internal

  Returns a hash that allows uniquely identifying whether the label parameters have changed such
  that the cached labels must be refreshed (\ref clearCache). It is used in \ref draw. If the
  return value of this method hasn't changed since the last redraw, the respective label parameters
  haven't changed and cached labels may be used.
*/
    QByteArray QCPAxisPainterPrivate::generateLabelParameterHash() const
    {
        QByteArray result;
        result.append(QByteArray::number(mParentPlot->bufferDevicePixelRatio()));
        result.append(QByteArray::number(tickLabelRotation));
        result.append(QByteArray::number(int(tickLabelSide)));
        result.append(QByteArray::number(int(substituteExponent)));
        result.append(QByteArray::number(int(numberMultiplyCross)));
        result.append(tickLabelColor.name().toLatin1()+QByteArray::number(tickLabelColor.alpha(), 16));
        result.append(tickLabelFont.toString().toLatin1());
        return result;
    }

/*! \internal

  Draws a single tick label with the provided \a painter, utilizing the internal label cache to
  significantly speed up drawing of labels that were drawn in previous calls. The tick label is
  always bound to an axis, the distance to the axis is controllable via \a distanceToAxis in
  pixels. The pixel position in the axis direction is passed in the \a position parameter. Hence
  for the bottom axis, \a position would indicate the horizontal pixel position (not coordinate),
  at which the label should be drawn.

  In order to later draw the axis label in a place that doesn't overlap with the tick labels, the
  largest tick label size is needed. This is acquired by passing a \a tickLabelsSize to the \ref
  drawTickLabel calls during the process of drawing all tick labels of one axis. In every call, \a
  tickLabelsSize is expanded, if the drawn label exceeds the value \a tickLabelsSize currently
  holds.

  The label is drawn with the font and pen that are currently set on the \a painter. To draw
  superscripted powers, the font is temporarily made smaller by a fixed factor (see \ref
  getTickLabelData).
*/
    void QCPAxisPainterPrivate::placeTickLabel(QCPPainter *painter, double position, int distanceToAxis, const QString &text, QSize *tickLabelsSize)
    {
// warning: if you change anything here, also adapt getMaxTickLabelSize() accordingly!
        if (text.isEmpty()) return;
        QSize finalSize;
        QPointF labelAnchor;
        switch (type)
        {
            case QCPAxis::atLeft:   labelAnchor = QPointF(axisRect.left()-distanceToAxis-offset, position); break;
            case QCPAxis::atRight:  labelAnchor = QPointF(axisRect.right()+distanceToAxis+offset, position); break;
            case QCPAxis::atTop:    labelAnchor = QPointF(position, axisRect.top()-distanceToAxis-offset); break;
            case QCPAxis::atBottom: labelAnchor = QPointF(position, axisRect.bottom()+distanceToAxis+offset); break;
        }
        if (mParentPlot->plottingHints().testFlag(QCP::phCacheLabels) && !painter->modes().testFlag(QCPPainter::pmNoCaching)) // label caching enabled
        {
            CachedLabel *cachedLabel = mLabelCache.take(text); // attempt to get label from cache
            if (!cachedLabel)  // no cached label existed, create it
            {
                cachedLabel = new CachedLabel;
                TickLabelData labelData = getTickLabelData(painter->font(), text);
                cachedLabel->offset = getTickLabelDrawOffset(labelData)+labelData.rotatedTotalBounds.topLeft();
                if (!qFuzzyCompare(1.0, mParentPlot->bufferDevicePixelRatio()))
                {
                    cachedLabel->pixmap = QPixmap(labelData.rotatedTotalBounds.size()*mParentPlot->bufferDevicePixelRatio());
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
                    #  ifdef QCP_DEVICEPIXELRATIO_FLOAT
cachedLabel->pixmap.setDevicePixelRatio(mParentPlot->devicePixelRatioF());
#  else
cachedLabel->pixmap.setDevicePixelRatio(mParentPlot->devicePixelRatio());
#  endif
#endif
                } else
                    cachedLabel->pixmap = QPixmap(labelData.rotatedTotalBounds.size());
                cachedLabel->pixmap.fill(Qt::transparent);
                QCPPainter cachePainter(&cachedLabel->pixmap);
                cachePainter.setPen(painter->pen());
                drawTickLabel(&cachePainter, -labelData.rotatedTotalBounds.topLeft().x(), -labelData.rotatedTotalBounds.topLeft().y(), labelData);
            }
// if label would be partly clipped by widget border on sides, don't draw it (only for outside tick labels):
            bool labelClippedByBorder = false;
            if (tickLabelSide == QCPAxis::lsOutside)
            {
                if (QCPAxis::orientation(type) == Qt::Horizontal)
                    labelClippedByBorder = labelAnchor.x()+cachedLabel->offset.x()+cachedLabel->pixmap.width()/mParentPlot->bufferDevicePixelRatio() > viewportRect.right() || labelAnchor.x()+cachedLabel->offset.x() < viewportRect.left();
                else
                    labelClippedByBorder = labelAnchor.y()+cachedLabel->offset.y()+cachedLabel->pixmap.height()/mParentPlot->bufferDevicePixelRatio() > viewportRect.bottom() || labelAnchor.y()+cachedLabel->offset.y() < viewportRect.top();
            }
            if (!labelClippedByBorder)
            {
                painter->drawPixmap(labelAnchor+cachedLabel->offset, cachedLabel->pixmap);
                finalSize = cachedLabel->pixmap.size()/mParentPlot->bufferDevicePixelRatio();
            }
            mLabelCache.insert(text, cachedLabel); // return label to cache or insert for the first time if newly created
        } else // label caching disabled, draw text directly on surface:
        {
            TickLabelData labelData = getTickLabelData(painter->font(), text);
            QPointF finalPosition = labelAnchor + getTickLabelDrawOffset(labelData);
// if label would be partly clipped by widget border on sides, don't draw it (only for outside tick labels):
            bool labelClippedByBorder = false;
            if (tickLabelSide == QCPAxis::lsOutside)
            {
                if (QCPAxis::orientation(type) == Qt::Horizontal)
                    labelClippedByBorder = finalPosition.x()+(labelData.rotatedTotalBounds.width()+labelData.rotatedTotalBounds.left()) > viewportRect.right() || finalPosition.x()+labelData.rotatedTotalBounds.left() < viewportRect.left();
                else
                    labelClippedByBorder = finalPosition.y()+(labelData.rotatedTotalBounds.height()+labelData.rotatedTotalBounds.top()) > viewportRect.bottom() || finalPosition.y()+labelData.rotatedTotalBounds.top() < viewportRect.top();
            }
            if (!labelClippedByBorder)
            {
                drawTickLabel(painter, finalPosition.x(), finalPosition.y(), labelData);
                finalSize = labelData.rotatedTotalBounds.size();
            }
        }

// expand passed tickLabelsSize if current tick label is larger:
        if (finalSize.width() > tickLabelsSize->width())
            tickLabelsSize->setWidth(finalSize.width());
        if (finalSize.height() > tickLabelsSize->height())
            tickLabelsSize->setHeight(finalSize.height());
    }

/*! \internal

  This is a \ref placeTickLabel helper function.

  Draws the tick label specified in \a labelData with \a painter at the pixel positions \a x and \a
  y. This function is used by \ref placeTickLabel to create new tick labels for the cache, or to
  directly draw the labels on the QCustomPlot surface when label caching is disabled, i.e. when
  QCP::phCacheLabels plotting hint is not set.
*/
    void QCPAxisPainterPrivate::drawTickLabel(QCPPainter *painter, double x, double y, const TickLabelData &labelData) const
    {
// backup painter settings that we're about to change:
        QTransform oldTransform = painter->transform();
        QFont oldFont = painter->font();

// transform painter to position/rotation:
        painter->translate(x, y);
        if (!qFuzzyIsNull(tickLabelRotation))
            painter->rotate(tickLabelRotation);

// draw text:
        if (!labelData.expPart.isEmpty()) // indicator that beautiful powers must be used
        {
            painter->setFont(labelData.baseFont);
            painter->drawText(0, 0, 0, 0, Qt::TextDontClip, labelData.basePart);
            if (!labelData.suffixPart.isEmpty())
                painter->drawText(labelData.baseBounds.width()+1+labelData.expBounds.width(), 0, 0, 0, Qt::TextDontClip, labelData.suffixPart);
            painter->setFont(labelData.expFont);
            painter->drawText(labelData.baseBounds.width()+1, 0, labelData.expBounds.width(), labelData.expBounds.height(), Qt::TextDontClip,  labelData.expPart);
        } else
        {
            painter->setFont(labelData.baseFont);
            painter->drawText(0, 0, labelData.totalBounds.width(), labelData.totalBounds.height(), Qt::TextDontClip | Qt::AlignHCenter, labelData.basePart);
        }

// reset painter settings to what it was before:
        painter->setTransform(oldTransform);
        painter->setFont(oldFont);
    }

/*! \internal

  This is a \ref placeTickLabel helper function.

  Transforms the passed \a text and \a font to a tickLabelData structure that can then be further
  processed by \ref getTickLabelDrawOffset and \ref drawTickLabel. It splits the text into base and
  exponent if necessary (member substituteExponent) and calculates appropriate bounding boxes.
*/
    QCPAxisPainterPrivate::TickLabelData QCPAxisPainterPrivate::getTickLabelData(const QFont &font, const QString &text) const
    {
        TickLabelData result;

// determine whether beautiful decimal powers should be used
        bool useBeautifulPowers = false;
        int ePos = -1; // first index of exponent part, text before that will be basePart, text until eLast will be expPart
        int eLast = -1; // last index of exponent part, rest of text after this will be suffixPart
        if (substituteExponent)
        {
            ePos = text.indexOf(QString(mParentPlot->locale().exponential()));
            if (ePos > 0 && text.at(ePos-1).isDigit())
            {
                eLast = ePos;
                while (eLast+1 < text.size() && (text.at(eLast+1) == QLatin1Char('+') || text.at(eLast+1) == QLatin1Char('-') || text.at(eLast+1).isDigit()))
                    ++eLast;
                if (eLast > ePos) // only if also to right of 'e' is a digit/+/- interpret it as beautifiable power
                    useBeautifulPowers = true;
            }
        }

// calculate text bounding rects and do string preparation for beautiful decimal powers:
        result.baseFont = font;
        if (result.baseFont.pointSizeF() > 0) // might return -1 if specified with setPixelSize, in that case we can't do correction in next line
            result.baseFont.setPointSizeF(result.baseFont.pointSizeF()+0.05); // QFontMetrics.boundingRect has a bug for exact point sizes that make the results oscillate due to internal rounding
        if (useBeautifulPowers)
        {
// split text into parts of number/symbol that will be drawn normally and part that will be drawn as exponent:
            result.basePart = text.left(ePos);
            result.suffixPart = text.mid(eLast+1); // also drawn normally but after exponent
// in log scaling, we want to turn "1*10^n" into "10^n", else add multiplication sign and decimal base:
            if (abbreviateDecimalPowers && result.basePart == QLatin1String("1"))
                result.basePart = QLatin1String("10");
            else
                result.basePart += (numberMultiplyCross ? QString(QChar(215)) : QString(QChar(183))) + QLatin1String("10");
            result.expPart = text.mid(ePos+1, eLast-ePos);
// clip "+" and leading zeros off expPart:
            while (result.expPart.length() > 2 && result.expPart.at(1) == QLatin1Char('0')) // length > 2 so we leave one zero when numberFormatChar is 'e'
                result.expPart.remove(1, 1);
            if (!result.expPart.isEmpty() && result.expPart.at(0) == QLatin1Char('+'))
                result.expPart.remove(0, 1);
// prepare smaller font for exponent:
            result.expFont = font;
            if (result.expFont.pointSize() > 0)
                result.expFont.setPointSize(int(result.expFont.pointSize()*0.75));
            else
                result.expFont.setPixelSize(int(result.expFont.pixelSize()*0.75));
// calculate bounding rects of base part(s), exponent part and total one:
            result.baseBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.basePart);
            result.expBounds = QFontMetrics(result.expFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.expPart);
            if (!result.suffixPart.isEmpty())
                result.suffixBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip, result.suffixPart);
            result.totalBounds = result.baseBounds.adjusted(0, 0, result.expBounds.width()+result.suffixBounds.width()+2, 0); // +2 consists of the 1 pixel spacing between base and exponent (see drawTickLabel) and an extra pixel to include AA
        } else // useBeautifulPowers == false
        {
            result.basePart = text;
            result.totalBounds = QFontMetrics(result.baseFont).boundingRect(0, 0, 0, 0, Qt::TextDontClip | Qt::AlignHCenter, result.basePart);
        }
        result.totalBounds.moveTopLeft(QPoint(0, 0)); // want bounding box aligned top left at origin, independent of how it was created, to make further processing simpler

// calculate possibly different bounding rect after rotation:
        result.rotatedTotalBounds = result.totalBounds;
        if (!qFuzzyIsNull(tickLabelRotation))
        {
            QTransform transform;
            transform.rotate(tickLabelRotation);
            result.rotatedTotalBounds = transform.mapRect(result.rotatedTotalBounds);
        }

        return result;
    }

/*! \internal

  This is a \ref placeTickLabel helper function.

  Calculates the offset at which the top left corner of the specified tick label shall be drawn.
  The offset is relative to a point right next to the tick the label belongs to.

  This function is thus responsible for e.g. centering tick labels under ticks and positioning them
  appropriately when they are rotated.
*/
    QPointF QCPAxisPainterPrivate::getTickLabelDrawOffset(const TickLabelData &labelData) const
    {
/*
    calculate label offset from base point at tick (non-trivial, for best visual appearance): short
    explanation for bottom axis: The anchor, i.e. the point in the label that is placed
    horizontally under the corresponding tick is always on the label side that is closer to the
    axis (e.g. the left side of the text when we're rotating clockwise). On that side, the height
    is halved and the resulting point is defined the anchor. This way, a 90 degree rotated text
    will be centered under the tick (i.e. displaced horizontally by half its height). At the same
    time, a 45 degree rotated text will "point toward" its tick, as is typical for rotated tick
    labels.
  */
        bool doRotation = !qFuzzyIsNull(tickLabelRotation);
        bool flip = qFuzzyCompare(qAbs(tickLabelRotation), 90.0); // perfect +/-90 degree flip. Indicates vertical label centering on vertical axes.
        double radians = tickLabelRotation/180.0*M_PI;
        double x = 0;
        double y = 0;
        if ((type == QCPAxis::atLeft && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atRight && tickLabelSide == QCPAxis::lsInside)) // Anchor at right side of tick label
        {
            if (doRotation)
            {
                if (tickLabelRotation > 0)
                {
                    x = -qCos(radians)*labelData.totalBounds.width();
                    y = flip ? -labelData.totalBounds.width()/2.0 : -qSin(radians)*labelData.totalBounds.width()-qCos(radians)*labelData.totalBounds.height()/2.0;
                } else
                {
                    x = -qCos(-radians)*labelData.totalBounds.width()-qSin(-radians)*labelData.totalBounds.height();
                    y = flip ? +labelData.totalBounds.width()/2.0 : +qSin(-radians)*labelData.totalBounds.width()-qCos(-radians)*labelData.totalBounds.height()/2.0;
                }
            } else
            {
                x = -labelData.totalBounds.width();
                y = -labelData.totalBounds.height()/2.0;
            }
        } else if ((type == QCPAxis::atRight && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atLeft && tickLabelSide == QCPAxis::lsInside)) // Anchor at left side of tick label
        {
            if (doRotation)
            {
                if (tickLabelRotation > 0)
                {
                    x = +qSin(radians)*labelData.totalBounds.height();
                    y = flip ? -labelData.totalBounds.width()/2.0 : -qCos(radians)*labelData.totalBounds.height()/2.0;
                } else
                {
                    x = 0;
                    y = flip ? +labelData.totalBounds.width()/2.0 : -qCos(-radians)*labelData.totalBounds.height()/2.0;
                }
            } else
            {
                x = 0;
                y = -labelData.totalBounds.height()/2.0;
            }
        } else if ((type == QCPAxis::atTop && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atBottom && tickLabelSide == QCPAxis::lsInside)) // Anchor at bottom side of tick label
        {
            if (doRotation)
            {
                if (tickLabelRotation > 0)
                {
                    x = -qCos(radians)*labelData.totalBounds.width()+qSin(radians)*labelData.totalBounds.height()/2.0;
                    y = -qSin(radians)*labelData.totalBounds.width()-qCos(radians)*labelData.totalBounds.height();
                } else
                {
                    x = -qSin(-radians)*labelData.totalBounds.height()/2.0;
                    y = -qCos(-radians)*labelData.totalBounds.height();
                }
            } else
            {
                x = -labelData.totalBounds.width()/2.0;
                y = -labelData.totalBounds.height();
            }
        } else if ((type == QCPAxis::atBottom && tickLabelSide == QCPAxis::lsOutside) || (type == QCPAxis::atTop && tickLabelSide == QCPAxis::lsInside)) // Anchor at top side of tick label
        {
            if (doRotation)
            {
                if (tickLabelRotation > 0)
                {
                    x = +qSin(radians)*labelData.totalBounds.height()/2.0;
                    y = 0;
                } else
                {
                    x = -qCos(-radians)*labelData.totalBounds.width()-qSin(-radians)*labelData.totalBounds.height()/2.0;
                    y = +qSin(-radians)*labelData.totalBounds.width();
                }
            } else
            {
                x = -labelData.totalBounds.width()/2.0;
                y = 0;
            }
        }

        return {x, y};
    }

/*! \internal

  Simulates the steps done by \ref placeTickLabel by calculating bounding boxes of the text label
  to be drawn, depending on number format etc. Since only the largest tick label is wanted for the
  margin calculation, the passed \a tickLabelsSize is only expanded, if it's currently set to a
  smaller width/height.
*/
    void QCPAxisPainterPrivate::getMaxTickLabelSize(const QFont &font, const QString &text,  QSize *tickLabelsSize) const
    {
// note: this function must return the same tick label sizes as the placeTickLabel function.
        QSize finalSize;
        if (mParentPlot->plottingHints().testFlag(QCP::phCacheLabels) && mLabelCache.contains(text)) // label caching enabled and have cached label
        {
            const CachedLabel *cachedLabel = mLabelCache.object(text);
            finalSize = cachedLabel->pixmap.size()/mParentPlot->bufferDevicePixelRatio();
        } else // label caching disabled or no label with this text cached:
        {
            TickLabelData labelData = getTickLabelData(font, text);
            finalSize = labelData.rotatedTotalBounds.size();
        }

// expand passed tickLabelsSize if current tick label is larger:
        if (finalSize.width() > tickLabelsSize->width())
            tickLabelsSize->setWidth(finalSize.width());
        if (finalSize.height() > tickLabelsSize->height())
            tickLabelsSize->setHeight(finalSize.height());
    }
} // QCP