#ifndef QCUSTOMPLOT_QCPSELECTIONRECT_HPP
#define QCUSTOMPLOT_QCPSELECTIONRECT_HPP

#include "QCPLayerable.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPSelectionRect : public QCPLayerable {
    Q_OBJECT
    public:
        explicit QCPSelectionRect(QCustomPlot *parentPlot);

        ~QCPSelectionRect() override;

        QRect rect() const { return mRect; }

        QCPRange range(const QCPAxis *axis) const;

        QPen pen() const { return mPen; }

        QBrush brush() const { return mBrush; }

        bool isActive() const { return mActive; }

        void setPen(const QPen &pen);

        void setBrush(const QBrush &brush);

        Q_SLOT void cancel();

    signals:

        void started(QMouseEvent *event);

        void changed(const QRect &rect, QMouseEvent *event);

        void canceled(const QRect &rect, QInputEvent *event);

        void accepted(const QRect &rect, QMouseEvent *event);

    protected:
        QRect mRect;
        QPen mPen;
        QBrush mBrush;
        bool mActive;

        virtual void startSelection(QMouseEvent *event);

        virtual void moveSelection(QMouseEvent *event);

        virtual void endSelection(QMouseEvent *event);

        virtual void keyPressEvent(QKeyEvent *event);

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void draw(QCPPainter *painter) override;
    };
}
#endif
