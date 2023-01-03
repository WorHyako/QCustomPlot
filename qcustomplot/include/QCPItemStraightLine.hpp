#ifndef QCUSTOMPLOT_QCPITEMSTRAIGHTLINE_HPP
#define QCUSTOMPLOT_QCPITEMSTRAIGHTLINE_HPP

#include "QCPAbstractItem.hpp"
#include "QCPVector2D.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPItemStraightLine : public QCPAbstractItem {
    Q_OBJECT
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
    public:
        explicit QCPItemStraightLine(QCustomPlot *parentPlot);

        ~QCPItemStraightLine() override;

        QPen pen() const { return mPen; }

        QPen selectedPen() const { return mSelectedPen; }

        void setPen(const QPen &pen);

        void setSelectedPen(const QPen &pen);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPItemPosition *const point1;
        QCPItemPosition *const point2;

    protected:
        QPen mPen, mSelectedPen;

        void draw(QCPPainter *painter) override;

        QLineF getRectClippedStraightLine(const QCPVector2D &base, const QCPVector2D &vec, const QRect &rect) const;

        QPen mainPen() const;
    };
}
#endif
