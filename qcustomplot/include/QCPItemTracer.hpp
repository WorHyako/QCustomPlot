#ifndef QCUSTOMPLOT_QCPITEMTRACER_HPP
#define QCUSTOMPLOT_QCPITEMTRACER_HPP

#include "defs.hpp"
#include "QCPAbstractItem.hpp"
#include "QCPItemPosition.hpp"
#include "QCPPainter.hpp"

#include <QPen>
#include <QColor>

namespace QCP {

    class QCP_LIB_DECL QCPItemTracer : public QCPAbstractItem {
    Q_OBJECT
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
        Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
        Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
        Q_PROPERTY(double size READ size WRITE setSize)
        Q_PROPERTY(TracerStyle style READ style WRITE setStyle)
        Q_PROPERTY(QCPGraph * graph READ graph WRITE setGraph)
        Q_PROPERTY(double graphKey READ graphKey WRITE setGraphKey)
        Q_PROPERTY(bool interpolating READ interpolating WRITE setInterpolating)
    public:
        /*!
        The different visual appearances a tracer item can have. Some styles size may be controlled with \ref setSize.

        \see setStyle
      */
        enum TracerStyle {
            tsNone,       ///< The tracer is not visible
            tsPlus,    ///< A plus shaped crosshair with limited size
            tsCrosshair, ///< A plus shaped crosshair which spans the complete axis rect
            tsCircle,   ///< A circle
            tsSquare,   ///< A square
        };

        Q_ENUM(TracerStyle)

        explicit QCPItemTracer(QCustomPlot *parentPlot);

        ~QCPItemTracer() override;

        QPen pen() const { return mPen; }

        QPen selectedPen() const { return mSelectedPen; }

        QBrush brush() const { return mBrush; }

        QBrush selectedBrush() const { return mSelectedBrush; }

        double size() const { return mSize; }

        TracerStyle style() const { return mStyle; }

        QCPGraph *graph() const { return mGraph; }

        double graphKey() const { return mGraphKey; }

        bool interpolating() const { return mInterpolating; }

        void setPen(const QPen &pen);

        void setSelectedPen(const QPen &pen);

        void setBrush(const QBrush &brush);

        void setSelectedBrush(const QBrush &brush);

        void setSize(double size);

        void setStyle(TracerStyle style);

        void setGraph(QCPGraph *graph);

        void setGraphKey(double key);

        void setInterpolating(bool enabled);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        void updatePosition();

        QCPItemPosition *const position;

    protected:
        QPen mPen, mSelectedPen;
        QBrush mBrush, mSelectedBrush;
        double mSize;
        TracerStyle mStyle;
        QCPGraph *mGraph;
        double mGraphKey;
        bool mInterpolating;

        void draw(QCPPainter *painter) override;

        QPen mainPen() const;

        QBrush mainBrush() const;
    };
}
Q_DECLARE_METATYPE(QCP::QCPItemTracer::TracerStyle)

#endif
