#ifndef QCUSTOMPLOT_QCPCOLORMAP_H
#define QCUSTOMPLOT_QCPCOLORMAP_H

namespace QCP{

    class QCP_LIB_DECL QCPColorMap : public QCPAbstractPlottable
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QCPRange dataRange READ dataRange WRITE setDataRange NOTIFY dataRangeChanged)
    Q_PROPERTY(QCPAxis::ScaleType dataScaleType READ dataScaleType WRITE setDataScaleType NOTIFY dataScaleTypeChanged)
    Q_PROPERTY(QCPColorGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged)
    Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate)
    Q_PROPERTY(bool tightBoundary READ tightBoundary WRITE setTightBoundary)
    Q_PROPERTY(QCPColorScale* colorScale READ colorScale WRITE setColorScale)
    /// \endcond
    public:
    explicit QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis);
    virtual ~QCPColorMap() Q_DECL_OVERRIDE;

    // getters:
    QCPColorMapData *data() const { return mMapData; }
    QCPRange dataRange() const { return mDataRange; }
    QCPAxis::ScaleType dataScaleType() const { return mDataScaleType; }
    bool interpolate() const { return mInterpolate; }
    bool tightBoundary() const { return mTightBoundary; }
    QCPColorGradient gradient() const { return mGradient; }
    QCPColorScale *colorScale() const { return mColorScale.data(); }

    // setters:
    void setData(QCPColorMapData *data, bool copy=false);
    Q_SLOT void setDataRange(const QCPRange &dataRange);
    Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);
    Q_SLOT void setGradient(const QCPColorGradient &gradient);
    void setInterpolate(bool enabled);
    void setTightBoundary(bool enabled);
    void setColorScale(QCPColorScale *colorScale);

    // non-property methods:
    void rescaleDataRange(bool recalculateDataBounds=false);
    Q_SLOT void updateLegendIcon(Qt::TransformationMode transformMode=Qt::SmoothTransformation, const QSize &thumbSize=QSize(32, 18));

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;
    virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const Q_DECL_OVERRIDE;
    virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const Q_DECL_OVERRIDE;

    signals:
    void dataRangeChanged(const QCPRange &newRange);
    void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
    void gradientChanged(const QCPColorGradient &newGradient);

    protected:
    // property members:
    QCPRange mDataRange;
    QCPAxis::ScaleType mDataScaleType;
    QCPColorMapData *mMapData;
    QCPColorGradient mGradient;
    bool mInterpolate;
    bool mTightBoundary;
    QPointer<QCPColorScale> mColorScale;

    // non-property members:
    QImage mMapImage, mUndersampledMapImage;
    QPixmap mLegendIcon;
    bool mMapImageInvalidated;

    // introduced virtual methods:
    virtual void updateMapImage();

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const Q_DECL_OVERRIDE;

    friend class QCustomPlot;
    friend class QCPLegend;
};

}

#endif //QCUSTOMPLOT_QCPCOLORMAP_H
