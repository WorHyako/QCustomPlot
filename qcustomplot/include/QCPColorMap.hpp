#ifndef QCUSTOMPLOT_QCPCOLORMAP_HPP
#define QCUSTOMPLOT_QCPCOLORMAP_HPP

#include "defs.hpp"
#include "QCPColorMapData.hpp"
#include "QCPColorGradient.hpp"
#include "QCPAbstractPlottable.hpp"
#include "QCPColorScale.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPColorMap : public QCPAbstractPlottable {
    Q_OBJECT
        Q_PROPERTY(QCPRange dataRange READ dataRange WRITE setDataRange NOTIFY dataRangeChanged)
        Q_PROPERTY(
                QCPAxis::ScaleType dataScaleType READ dataScaleType WRITE setDataScaleType NOTIFY dataScaleTypeChanged)
        Q_PROPERTY(QCPColorGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged)
        Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate)
        Q_PROPERTY(bool tightBoundary READ tightBoundary WRITE setTightBoundary)
        Q_PROPERTY(QCPColorScale * colorScale READ colorScale WRITE setColorScale)
    public:
        Q_DISABLE_COPY(QCPColorMap)

        explicit QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis);

        ~QCPColorMap() override;

        QCPColorMapData *data() const { return mMapData; }

        QCPRange dataRange() const { return mDataRange; }

        QCPAxis::ScaleType dataScaleType() const { return mDataScaleType; }

        bool interpolate() const { return mInterpolate; }

        bool tightBoundary() const { return mTightBoundary; }

        QCPColorGradient gradient() const { return mGradient; }

        QCPColorScale *colorScale() const { return mColorScale.data(); }

        void setData(QCPColorMapData *data, bool copy = false);

        Q_SLOT void setDataRange(const QCPRange &dataRange);

        Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);

        Q_SLOT void setGradient(const QCPColorGradient &gradient);

        void setInterpolate(bool enabled);

        void setTightBoundary(bool enabled);

        void setColorScale(QCPColorScale *colorScale);

        void rescaleDataRange(bool recalculateDataBounds = false);

        Q_SLOT void updateLegendIcon(Qt::TransformationMode transformMode = Qt::SmoothTransformation,
                                     const QSize &thumbSize = QSize(32, 18));

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth) const override;

        QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth,
                               const QCPRange &inKeyRange = QCPRange()) const override;

    signals:

        void dataRangeChanged(const QCPRange &newRange);

        void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);

        void gradientChanged(const QCPColorGradient &newGradient);

    protected:
        QCPRange mDataRange;
        QCPAxis::ScaleType mDataScaleType;
        QCPColorMapData *mMapData;
        QCPColorGradient mGradient;
        bool mInterpolate;
        bool mTightBoundary;
        QPointer<QCPColorScale> mColorScale;

        QImage mMapImage, mUndersampledMapImage;
        QPixmap mLegendIcon;
        bool mMapImageInvalidated;

        virtual void updateMapImage();

        void draw(QCPPainter *painter) override;

        void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const override;
    };
}
#endif
