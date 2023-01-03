#ifndef QCUSTOMPLOT_QCPITEMPIXMAP_HPP
#define QCUSTOMPLOT_QCPITEMPIXMAP_HPP

#include "QCPAbstractItem.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPItemPixmap : public QCPAbstractItem {
    Q_OBJECT
        Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
        Q_PROPERTY(bool scaled READ scaled WRITE setScaled)
        Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode)
        Q_PROPERTY(Qt::TransformationMode transformationMode READ transformationMode)
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
    public:
        explicit QCPItemPixmap(QCustomPlot *parentPlot);

        ~QCPItemPixmap() override;

        QPixmap pixmap() const { return mPixmap; }

        bool scaled() const { return mScaled; }

        Qt::AspectRatioMode aspectRatioMode() const { return mAspectRatioMode; }

        Qt::TransformationMode transformationMode() const { return mTransformationMode; }

        QPen pen() const { return mPen; }

        QPen selectedPen() const { return mSelectedPen; }

        void setPixmap(const QPixmap &pixmap);

        void setScaled(bool scaled, Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio,
                       Qt::TransformationMode transformationMode = Qt::SmoothTransformation);

        void setPen(const QPen &pen);

        void setSelectedPen(const QPen &pen);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPItemPosition *const topLeft;
        QCPItemPosition *const bottomRight;
        QCPItemAnchor *const top;
        QCPItemAnchor *const topRight;
        QCPItemAnchor *const right;
        QCPItemAnchor *const bottom;
        QCPItemAnchor *const bottomLeft;
        QCPItemAnchor *const left;

    protected:
        enum AnchorIndex {
            aiTop,
            aiTopRight,
            aiRight,
            aiBottom,
            aiBottomLeft,
            aiLeft
        };

        QPixmap mPixmap;
        QPixmap mScaledPixmap;
        bool mScaled;
        bool mScaledPixmapInvalidated;
        Qt::AspectRatioMode mAspectRatioMode;
        Qt::TransformationMode mTransformationMode;
        QPen mPen, mSelectedPen;

        void draw(QCPPainter *painter) override;

        QPointF anchorPixelPosition(int anchorId) const override;

        void updateScaledPixmap(QRect finalRect = QRect(), bool flipHorz = false, bool flipVert = false);

        QRect getFinalRect(bool *flippedHorz = nullptr, bool *flippedVert = nullptr) const;

        QPen mainPen() const;
    };
}
#endif
