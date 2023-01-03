#ifndef QCUSTOMPLOT_QCPAXISRECT_HPP
#define QCUSTOMPLOT_QCPAXISRECT_HPP

#include "QCPLayoutElement.hpp"
#include "QCPLayoutInset.hpp"
#include "QCustomPlot.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPAxisRect : public QCPLayoutElement {
    Q_OBJECT
        Q_PROPERTY(QPixmap background READ background WRITE setBackground)
        Q_PROPERTY(bool backgroundScaled READ backgroundScaled WRITE setBackgroundScaled)
        Q_PROPERTY(Qt::AspectRatioMode backgroundScaledMode READ backgroundScaledMode WRITE setBackgroundScaledMode)
        Q_PROPERTY(Qt::Orientations rangeDrag READ rangeDrag WRITE setRangeDrag)
        Q_PROPERTY(Qt::Orientations rangeZoom READ rangeZoom WRITE setRangeZoom)
    public:
        explicit QCPAxisRect(QCustomPlot *parentPlot, bool setupDefaultAxes = true);

        ~QCPAxisRect() override;

        Q_DISABLE_COPY(QCPAxisRect)

        QPixmap background() const { return mBackgroundPixmap; }

        QBrush backgroundBrush() const { return mBackgroundBrush; }

        bool backgroundScaled() const { return mBackgroundScaled; }

        Qt::AspectRatioMode backgroundScaledMode() const { return mBackgroundScaledMode; }

        Qt::Orientations rangeDrag() const { return mRangeDrag; }

        Qt::Orientations rangeZoom() const { return mRangeZoom; }

        QCPAxis *rangeDragAxis(Qt::Orientation orientation);

        QCPAxis *rangeZoomAxis(Qt::Orientation orientation);

        QList<QCPAxis *> rangeDragAxes(Qt::Orientation orientation);

        QList<QCPAxis *> rangeZoomAxes(Qt::Orientation orientation);

        double rangeZoomFactor(Qt::Orientation orientation);

        void setBackground(const QPixmap &pm);

        void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode = Qt::KeepAspectRatioByExpanding);

        void setBackground(const QBrush &brush);

        void setBackgroundScaled(bool scaled);

        void setBackgroundScaledMode(Qt::AspectRatioMode mode);

        void setRangeDrag(Qt::Orientations orientations);

        void setRangeZoom(Qt::Orientations orientations);

        void setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical);

        void setRangeDragAxes(QList<QCPAxis *> axes);

        void setRangeDragAxes(QList<QCPAxis *> horizontal, QList<QCPAxis *> vertical);

        void setRangeZoomAxes(QCPAxis *horizontal, QCPAxis *vertical);

        void setRangeZoomAxes(QList<QCPAxis *> axes);

        void setRangeZoomAxes(QList<QCPAxis *> horizontal, QList<QCPAxis *> vertical);

        void setRangeZoomFactor(double horizontalFactor, double verticalFactor);

        void setRangeZoomFactor(double factor);

        int axisCount(QCPAxis::AxisType type) const;

        QCPAxis *axis(QCPAxis::AxisType type, int index = 0) const;

        QList<QCPAxis *> axes(QCPAxis::AxisTypes types) const;

        QList<QCPAxis *> axes() const;

        QCPAxis *addAxis(QCPAxis::AxisType type, QCPAxis *axis = nullptr);

        QList<QCPAxis *> addAxes(QCPAxis::AxisTypes types);

        bool removeAxis(QCPAxis *axis);

        QCPLayoutInset *insetLayout() const { return mInsetLayout; }

        void zoom(const QRectF &pixelRect);

        void zoom(const QRectF &pixelRect, const QList<QCPAxis *> &affectedAxes);

        void setupFullAxesBox(bool connectRanges = false);

        QList<QCPAbstractPlottable *> plottables() const;

        QList<QCPGraph *> graphs() const;

        QList<QCPAbstractItem *> items() const;

        // read-only interface imitating a QRect:
        int left() const { return mRect.left(); }

        int right() const { return mRect.right(); }

        int top() const { return mRect.top(); }

        int bottom() const { return mRect.bottom(); }

        int width() const { return mRect.width(); }

        int height() const { return mRect.height(); }

        QSize size() const { return mRect.size(); }

        QPoint topLeft() const { return mRect.topLeft(); }

        QPoint topRight() const { return mRect.topRight(); }

        QPoint bottomLeft() const { return mRect.bottomLeft(); }

        QPoint bottomRight() const { return mRect.bottomRight(); }

        QPoint center() const { return mRect.center(); }

        void update(UpdatePhase phase) override;

        QList<QCPLayoutElement *> elements(bool recursive) const override;

    protected:
        QBrush mBackgroundBrush;
        QPixmap mBackgroundPixmap;
        QPixmap mScaledBackgroundPixmap;
        bool mBackgroundScaled;
        Qt::AspectRatioMode mBackgroundScaledMode;
        QCPLayoutInset *mInsetLayout;
        Qt::Orientations mRangeDrag, mRangeZoom;
        QList<QPointer<QCPAxis> > mRangeDragHorzAxis, mRangeDragVertAxis;
        QList<QPointer<QCPAxis> > mRangeZoomHorzAxis, mRangeZoomVertAxis;
        double mRangeZoomFactorHorz, mRangeZoomFactorVert;

        QList<QCPRange> mDragStartHorzRange, mDragStartVertRange;
        QCP::AntialiasedElements mAADragBackup, mNotAADragBackup;
        bool mDragging;
        QHash<QCPAxis::AxisType, QList<QCPAxis *> > mAxes;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void draw(QCPPainter *painter) override;

        int calculateAutoMargin(QCP::MarginSide side) override;

        void layoutChanged() override;

        void mousePressEvent(QMouseEvent *event, const QVariant &details) override;

        void mouseMoveEvent(QMouseEvent *event, const QPointF &startPos) override;

        void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos) override;

        void wheelEvent(QWheelEvent *event) override;

        void drawBackground(QCPPainter *painter);

        void updateAxesOffset(QCPAxis::AxisType type);
    };
}
#endif
