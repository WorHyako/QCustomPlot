#include "include/QCPColorScaleAxisRectPrivate.hpp"

namespace QCP {
/*! \class QCPColorScaleAxisRectPrivate

  \internal
  \brief An axis rect subclass for use in a QCPColorScale

  This is a private class and not part of the public QCustomPlot interface.

  It provides the axis rect functionality for the QCPColorScale class.
*/


/*!
  Creates a new instance, as a child of \a parentColorScale.
*/
    QCPColorScaleAxisRectPrivate::QCPColorScaleAxisRectPrivate(QCPColorScale *parentColorScale) :
            QCPAxisRect(parentColorScale->parentPlot(), true),
            mParentColorScale(parentColorScale),
            mGradientImageInvalidated(true)
    {
        setParentLayerable(parentColorScale);
        setMinimumMargins(QMargins(0, 0, 0, 0));
        const QList<QCPAxis::AxisType> allAxisTypes = QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight;
        foreach (QCPAxis::AxisType type, allAxisTypes)
        {
            axis(type)->setVisible(true);
            axis(type)->grid()->setVisible(false);
            axis(type)->setPadding(0);
            connect(axis(type), SIGNAL(selectionChanged(QCPAxis::SelectableParts)), this, SLOT(axisSelectionChanged(QCPAxis::SelectableParts)));
            connect(axis(type), SIGNAL(selectableChanged(QCPAxis::SelectableParts)), this, SLOT(axisSelectableChanged(QCPAxis::SelectableParts)));
        }

        connect(axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atRight), SLOT(setRange(QCPRange)));
        connect(axis(QCPAxis::atRight), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atLeft), SLOT(setRange(QCPRange)));
        connect(axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atTop), SLOT(setRange(QCPRange)));
        connect(axis(QCPAxis::atTop), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
        connect(axis(QCPAxis::atLeft), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atRight), SLOT(setScaleType(QCPAxis::ScaleType)));
        connect(axis(QCPAxis::atRight), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atLeft), SLOT(setScaleType(QCPAxis::ScaleType)));
        connect(axis(QCPAxis::atBottom), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atTop), SLOT(setScaleType(QCPAxis::ScaleType)));
        connect(axis(QCPAxis::atTop), SIGNAL(scaleTypeChanged(QCPAxis::ScaleType)), axis(QCPAxis::atBottom), SLOT(setScaleType(QCPAxis::ScaleType)));

// make layer transfers of color scale transfer to axis rect and axes
// the axes must be set after axis rect, such that they appear above color gradient drawn by axis rect:
        connect(parentColorScale, SIGNAL(layerChanged(QCPLayer*)), this, SLOT(setLayer(QCPLayer*)));
        foreach (QCPAxis::AxisType type, allAxisTypes)
        connect(parentColorScale, SIGNAL(layerChanged(QCPLayer*)), axis(type), SLOT(setLayer(QCPLayer*)));
    }

/*! \internal

  Updates the color gradient image if necessary, by calling \ref updateGradientImage, then draws
  it. Then the axes are drawn by calling the \ref QCPAxisRect::draw base class implementation.

  \seebaseclassmethod
*/
    void QCPColorScaleAxisRectPrivate::draw(QCPPainter *painter)
    {
        if (mGradientImageInvalidated)
            updateGradientImage();

        bool mirrorHorz = false;
        bool mirrorVert = false;
        if (mParentColorScale->mColorAxis)
        {
            mirrorHorz = mParentColorScale->mColorAxis.data()->rangeReversed() && (mParentColorScale->type() == QCPAxis::atBottom || mParentColorScale->type() == QCPAxis::atTop);
            mirrorVert = mParentColorScale->mColorAxis.data()->rangeReversed() && (mParentColorScale->type() == QCPAxis::atLeft || mParentColorScale->type() == QCPAxis::atRight);
        }

        painter->drawImage(rect().adjusted(0, -1, 0, -1), mGradientImage.mirrored(mirrorHorz, mirrorVert));
        QCPAxisRect::draw(painter);
    }

/*! \internal

  Uses the current gradient of the parent \ref QCPColorScale (specified in the constructor) to
  generate a gradient image. This gradient image will be used in the \ref draw method.
*/
    void QCPColorScaleAxisRectPrivate::updateGradientImage()
    {
        if (rect().isEmpty())
            return;

        const QImage::Format format = QImage::Format_ARGB32_Premultiplied;
        int n = mParentColorScale->mGradient.levelCount();
        int w, h;
        QVector<double> data(n);
        for (int i=0; i<n; ++i)
            data[i] = i;
        if (mParentColorScale->mType == QCPAxis::atBottom || mParentColorScale->mType == QCPAxis::atTop)
        {
            w = n;
            h = rect().height();
            mGradientImage = QImage(w, h, format);
            QVector<QRgb*> pixels;
            for (int y=0; y<h; ++y)
                pixels.append(reinterpret_cast<QRgb*>(mGradientImage.scanLine(y)));
            mParentColorScale->mGradient.colorize(data.constData(), QCPRange(0, n-1), pixels.first(), n);
            for (int y=1; y<h; ++y)
                memcpy(pixels.at(y), pixels.first(), size_t(n)*sizeof(QRgb));
        } else
        {
            w = rect().width();
            h = n;
            mGradientImage = QImage(w, h, format);
            for (int y=0; y<h; ++y)
            {
                QRgb *pixels = reinterpret_cast<QRgb*>(mGradientImage.scanLine(y));
                const QRgb lineColor = mParentColorScale->mGradient.color(data[h-1-y], QCPRange(0, n-1));
                for (int x=0; x<w; ++x)
                    pixels[x] = lineColor;
            }
        }
        mGradientImageInvalidated = false;
    }

/*! \internal

  This slot is connected to the selectionChanged signals of the four axes in the constructor. It
  synchronizes the selection state of the axes.
*/
    void QCPColorScaleAxisRectPrivate::axisSelectionChanged(QCPAxis::SelectableParts selectedParts)
    {
// axis bases of four axes shall always (de-)selected synchronously:
        const QList<QCPAxis::AxisType> allAxisTypes = QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight;
        foreach (QCPAxis::AxisType type, allAxisTypes)
        {
            if (QCPAxis *senderAxis = qobject_cast<QCPAxis*>(sender()))
                if (senderAxis->axisType() == type)
                    continue;

            if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis))
            {
                if (selectedParts.testFlag(QCPAxis::spAxis))
                    axis(type)->setSelectedParts(axis(type)->selectedParts() | QCPAxis::spAxis);
                else
                    axis(type)->setSelectedParts(axis(type)->selectedParts() & ~QCPAxis::spAxis);
            }
        }
    }

/*! \internal

  This slot is connected to the selectableChanged signals of the four axes in the constructor. It
  synchronizes the selectability of the axes.
*/
    void QCPColorScaleAxisRectPrivate::axisSelectableChanged(QCPAxis::SelectableParts selectableParts)
    {
// synchronize axis base selectability:
        const QList<QCPAxis::AxisType> allAxisTypes = QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight;
        foreach (QCPAxis::AxisType type, allAxisTypes)
        {
            if (QCPAxis *senderAxis = qobject_cast<QCPAxis*>(sender()))
                if (senderAxis->axisType() == type)
                    continue;

            if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis))
            {
                if (selectableParts.testFlag(QCPAxis::spAxis))
                    axis(type)->setSelectableParts(axis(type)->selectableParts() | QCPAxis::spAxis);
                else
                    axis(type)->setSelectableParts(axis(type)->selectableParts() & ~QCPAxis::spAxis);
            }
        }
    }
} // QCP