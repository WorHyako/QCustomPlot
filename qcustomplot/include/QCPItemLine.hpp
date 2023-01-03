#ifndef QCUSTOMPLOT_QCPITEMLINE_HPP
#define QCUSTOMPLOT_QCPITEMLINE_HPP

#include "QCPAbstractItem.hpp"
#include "QCPVector2D.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPItemLine : public QCPAbstractItem {
    Q_OBJECT
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
        Q_PROPERTY(QCPLineEnding head READ head WRITE setHead)
        Q_PROPERTY(QCPLineEnding tail READ tail WRITE setTail)
    public:
        explicit QCPItemLine(QCustomPlot *parentPlot);

        ~QCPItemLine() override;

        QPen pen() const { return mPen; }

        QPen selectedPen() const { return mSelectedPen; }

        QCPLineEnding head() const { return mHead; }

        QCPLineEnding tail() const { return mTail; }

        void setPen(const QPen &pen);

        void setSelectedPen(const QPen &pen);

        void setHead(const QCPLineEnding &head);

        void setTail(const QCPLineEnding &tail);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPItemPosition *const start;
        QCPItemPosition *const end;

    protected:
        QPen mPen, mSelectedPen;
        QCPLineEnding mHead, mTail;

        void draw(QCPPainter *painter) override;

        QLineF getRectClippedLine(const QCPVector2D &start, const QCPVector2D &end, const QRect &rect) const;

        QPen mainPen() const;
    };
}
#endif
