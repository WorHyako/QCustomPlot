#ifndef QCUSTOMPLOT_QCPCOLORSCALE_HPP
#define QCUSTOMPLOT_QCPCOLORSCALE_HPP

#include "QCPLayoutElement.hpp"
#include "QCPColorGradient.hpp"
#include "QCPColorMap.hpp"
#include "QCPColorScaleAxisRectPrivate.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPColorScale : public QCPLayoutElement {
    Q_OBJECT
        Q_PROPERTY(QCPAxis::AxisType type READ type WRITE setType)
        Q_PROPERTY(QCPRange dataRange READ dataRange WRITE setDataRange NOTIFY dataRangeChanged)
        Q_PROPERTY(
                QCPAxis::ScaleType dataScaleType READ dataScaleType WRITE setDataScaleType NOTIFY dataScaleTypeChanged)
        Q_PROPERTY(QCPColorGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged)
        Q_PROPERTY(QString label READ label WRITE setLabel)
        Q_PROPERTY(int barWidth READ barWidth WRITE setBarWidth)
        Q_PROPERTY(bool rangeDrag READ rangeDrag WRITE setRangeDrag)
        Q_PROPERTY(bool rangeZoom READ rangeZoom WRITE setRangeZoom)
    public:
        Q_DISABLE_COPY(QCPColorScale)

        explicit QCPColorScale(QCustomPlot *parentPlot);

        ~QCPColorScale() override;

        QCPAxis *axis() const { return mColorAxis.data(); }

        QCPAxis::AxisType type() const { return mType; }

        QCPRange dataRange() const { return mDataRange; }

        QCPAxis::ScaleType dataScaleType() const { return mDataScaleType; }

        QCPColorGradient gradient() const { return mGradient; }

        QString label() const;

        int barWidth() const { return mBarWidth; }

        bool rangeDrag() const;

        bool rangeZoom() const;

        void setType(QCPAxis::AxisType type);

        Q_SLOT void setDataRange(const QCPRange &dataRange);

        Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);

        Q_SLOT void setGradient(const QCPColorGradient &gradient);

        void setLabel(const QString &str);

        void setBarWidth(int width);

        void setRangeDrag(bool enabled);

        void setRangeZoom(bool enabled);

        QList<QCPColorMap *> colorMaps() const;

        void rescaleDataRange(bool onlyVisibleMaps);

        void update(UpdatePhase phase) override;

    signals:

        void dataRangeChanged(const QCPRange &newRange);

        void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);

        void gradientChanged(const QCPColorGradient &newGradient);

    protected:
        QCPAxis::AxisType mType;
        QCPRange mDataRange;
        QCPAxis::ScaleType mDataScaleType;
        QCPColorGradient mGradient;
        int mBarWidth;

        QPointer<QCPColorScaleAxisRectPrivate> mAxisRect;
        QPointer<QCPAxis> mColorAxis;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void mousePressEvent(QMouseEvent *event, const QVariant &details) override;

        void mouseMoveEvent(QMouseEvent *event, const QPointF &startPos) override;

        void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos) override;

        void wheelEvent(QWheelEvent *event) override;
    };
}
#endif
