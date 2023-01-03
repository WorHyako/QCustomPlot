#ifndef QCUSTOMPLOT_QCPITEMELLIPSE_HPP
#define QCUSTOMPLOT_QCPITEMELLIPSE_HPP

#include "QCPAbstractItem.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPItemEllipse : public QCPAbstractItem {
    Q_OBJECT
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
        Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
        Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
    public:
        explicit QCPItemEllipse(QCustomPlot *parentPlot);

        ~QCPItemEllipse() override;

        QPen pen() const { return mPen; }

        QPen selectedPen() const { return mSelectedPen; }

        QBrush brush() const { return mBrush; }

        QBrush selectedBrush() const { return mSelectedBrush; }

        void setPen(const QPen &pen);

        void setSelectedPen(const QPen &pen);

        void setBrush(const QBrush &brush);

        void setSelectedBrush(const QBrush &brush);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPItemPosition *const topLeft;
        QCPItemPosition *const bottomRight;
        QCPItemAnchor *const topLeftRim;
        QCPItemAnchor *const top;
        QCPItemAnchor *const topRightRim;
        QCPItemAnchor *const right;
        QCPItemAnchor *const bottomRightRim;
        QCPItemAnchor *const bottom;
        QCPItemAnchor *const bottomLeftRim;
        QCPItemAnchor *const left;
        QCPItemAnchor *const center;

    protected:
        enum AnchorIndex {
            aiTopLeftRim,
            aiTop,
            aiTopRightRim,
            aiRight,
            aiBottomRightRim,
            aiBottom,
            aiBottomLeftRim,
            aiLeft,
            aiCenter
        };

        QPen mPen, mSelectedPen;
        QBrush mBrush, mSelectedBrush;

        void draw(QCPPainter *painter) override;

        QPointF anchorPixelPosition(int anchorId) const override;

        QPen mainPen() const;

        QBrush mainBrush() const;
    };
}
#endif
