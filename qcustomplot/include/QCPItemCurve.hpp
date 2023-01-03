#ifndef QCUSTOMPLOT_QCPITEMCURVE_HPP
#define QCUSTOMPLOT_QCPITEMCURVE_HPP

#include "QCPAbstractItem.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPItemCurve : public QCPAbstractItem {
    Q_OBJECT
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
        Q_PROPERTY(QCPLineEnding head READ head WRITE setHead)
        Q_PROPERTY(QCPLineEnding tail READ tail WRITE setTail)
    public:
        explicit QCPItemCurve(QCustomPlot *parentPlot);

        ~QCPItemCurve() override;

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
        QCPItemPosition *const startDir;
        QCPItemPosition *const endDir;
        QCPItemPosition *const end;

    protected:
        QPen mPen, mSelectedPen;
        QCPLineEnding mHead, mTail;

        void draw(QCPPainter *painter) override;

        QPen mainPen() const;
    };
}
#endif
