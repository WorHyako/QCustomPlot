#ifndef QCUSTOMPLOT_QCPITEMPIXMAP_H
#define QCUSTOMPLOT_QCPITEMPIXMAP_H

namespace QCP {

    class QCP_LIB_DECL QCPItemPixmap : public QCPAbstractItem
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
    Q_PROPERTY(bool scaled READ scaled WRITE setScaled)
    Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode)
    Q_PROPERTY(Qt::TransformationMode transformationMode READ transformationMode)
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
    /// \endcond
    public:
    explicit QCPItemPixmap(QCustomPlot *parentPlot);
    virtual ~QCPItemPixmap() Q_DECL_OVERRIDE;

    // getters:
    QPixmap pixmap() const { return mPixmap; }
    bool scaled() const { return mScaled; }
    Qt::AspectRatioMode aspectRatioMode() const { return mAspectRatioMode; }
    Qt::TransformationMode transformationMode() const { return mTransformationMode; }
    QPen pen() const { return mPen; }
    QPen selectedPen() const { return mSelectedPen; }

    // setters;
    void setPixmap(const QPixmap &pixmap);
    void setScaled(bool scaled, Qt::AspectRatioMode aspectRatioMode=Qt::KeepAspectRatio, Qt::TransformationMode transformationMode=Qt::SmoothTransformation);
    void setPen(const QPen &pen);
    void setSelectedPen(const QPen &pen);

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;

    QCPItemPosition * const topLeft;
    QCPItemPosition * const bottomRight;
    QCPItemAnchor * const top;
    QCPItemAnchor * const topRight;
    QCPItemAnchor * const right;
    QCPItemAnchor * const bottom;
    QCPItemAnchor * const bottomLeft;
    QCPItemAnchor * const left;

    protected:
    enum AnchorIndex {aiTop, aiTopRight, aiRight, aiBottom, aiBottomLeft, aiLeft};

    // property members:
    QPixmap mPixmap;
    QPixmap mScaledPixmap;
    bool mScaled;
    bool mScaledPixmapInvalidated;
    Qt::AspectRatioMode mAspectRatioMode;
    Qt::TransformationMode mTransformationMode;
    QPen mPen, mSelectedPen;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    virtual QPointF anchorPixelPosition(int anchorId) const Q_DECL_OVERRIDE;

    // non-virtual methods:
    void updateScaledPixmap(QRect finalRect=QRect(), bool flipHorz=false, bool flipVert=false);
    QRect getFinalRect(bool *flippedHorz=nullptr, bool *flippedVert=nullptr) const;
    QPen mainPen() const;
};


} // QCP

#endif //QCUSTOMPLOT_QCPITEMPIXMAP_H
