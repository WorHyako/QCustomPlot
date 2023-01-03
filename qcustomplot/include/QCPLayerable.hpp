#ifndef QCUSTOMPLOT_QCPLAYERABLE_HPP
#define QCUSTOMPLOT_QCPLAYERABLE_HPP

#include "defs.hpp"
#include "enums.hpp"
#include "QCustomPlot.hpp"
#include "QCPLayer.hpp"
#include "QCPPainter.hpp"

#include <QObject>
#include <QString>
#include <QPointer>
#include <QMouseEvent>
#include <QWheelEvent>

namespace QCP {

    class QCP_LIB_DECL QCPLayerable : public QObject {
    Q_OBJECT
        Q_PROPERTY(bool visible READ visible WRITE setVisible)
        Q_PROPERTY(QCustomPlot *parentPlot READ parentPlot)
        Q_PROPERTY(QCPLayerable *parentLayerable READ parentLayerable)
        Q_PROPERTY(QCPLayer *layer READ layer WRITE setLayer NOTIFY layerChanged)
        Q_PROPERTY(bool antialiased READ antialiased WRITE setAntialiased)
    public:
        Q_DISABLE_COPY(QCPLayerable)

        explicit QCPLayerable(QCustomPlot *plot, QString targetLayer = QString(),
                              QCPLayerable *parentLayerable = nullptr);

        ~QCPLayerable() override;

        bool visible() const { return mVisible; }

        QCustomPlot *parentPlot() const { return mParentPlot; }

        QCPLayerable *parentLayerable() const { return mParentLayerable.data(); }

        QCPLayer *layer() const { return mLayer; }

        bool antialiased() const { return mAntialiased; }

        void setVisible(bool on);

        Q_SLOT bool setLayer(QCPLayer *layer);

        bool setLayer(const QString &layerName);

        void setAntialiased(bool enabled);

        virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const;

        bool realVisibility() const;

    signals:

        void layerChanged(QCPLayer *newLayer);

    protected:
        bool mVisible;
        QCustomPlot *mParentPlot;
        QPointer<QCPLayerable> mParentLayerable;
        QCPLayer *mLayer;
        bool mAntialiased;

        virtual void parentPlotInitialized(QCustomPlot *parentPlot);

        virtual Interaction selectionCategory() const;

        virtual QRect clipRect() const;

        virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const = 0;

        virtual void draw(QCPPainter *painter) = 0;

        virtual void
        selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);

        virtual void deselectEvent(bool *selectionStateChanged);

        virtual void mousePressEvent(QMouseEvent *event, const QVariant &details);

        virtual void mouseMoveEvent(QMouseEvent *event, const QPointF &startPos);

        virtual void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos);

        virtual void mouseDoubleClickEvent(QMouseEvent *event, const QVariant &details);

        virtual void wheelEvent(QWheelEvent *event);

        void initializeParentPlot(QCustomPlot *parentPlot);

        void setParentLayerable(QCPLayerable *parentLayerable);

        bool moveToLayer(QCPLayer *layer, bool prepend);

        void
        applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, AntialiasedElement overrideElement) const;
    };
}
#endif
