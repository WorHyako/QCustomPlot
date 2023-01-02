#ifndef QCUSTOMPLOT_QCPSTATISTICALBOX_H
#define QCUSTOMPLOT_QCPSTATISTICALBOX_H

namespace QCP {

    class QCP_LIB_DECL QCPStatisticalBox : public QCPAbstractPlottable1D<QCPStatisticalBoxData>
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(double width READ width WRITE setWidth)
    Q_PROPERTY(double whiskerWidth READ whiskerWidth WRITE setWhiskerWidth)
    Q_PROPERTY(QPen whiskerPen READ whiskerPen WRITE setWhiskerPen)
    Q_PROPERTY(QPen whiskerBarPen READ whiskerBarPen WRITE setWhiskerBarPen)
    Q_PROPERTY(bool whiskerAntialiased READ whiskerAntialiased WRITE setWhiskerAntialiased)
    Q_PROPERTY(QPen medianPen READ medianPen WRITE setMedianPen)
    Q_PROPERTY(QCPScatterStyle outlierStyle READ outlierStyle WRITE setOutlierStyle)
    /// \endcond
    public:
    explicit QCPStatisticalBox(QCPAxis *keyAxis, QCPAxis *valueAxis);

    // getters:
    QSharedPointer<QCPStatisticalBoxDataContainer> data() const { return mDataContainer; }
    double width() const { return mWidth; }
    double whiskerWidth() const { return mWhiskerWidth; }
    QPen whiskerPen() const { return mWhiskerPen; }
    QPen whiskerBarPen() const { return mWhiskerBarPen; }
    bool whiskerAntialiased() const { return mWhiskerAntialiased; }
    QPen medianPen() const { return mMedianPen; }
    QCPScatterStyle outlierStyle() const { return mOutlierStyle; }

    // setters:
    void setData(QSharedPointer<QCPStatisticalBoxDataContainer> data);
    void setData(const QVector<double> &keys, const QVector<double> &minimum, const QVector<double> &lowerQuartile, const QVector<double> &median, const QVector<double> &upperQuartile, const QVector<double> &maximum, bool alreadySorted=false);
    void setWidth(double width);
    void setWhiskerWidth(double width);
    void setWhiskerPen(const QPen &pen);
    void setWhiskerBarPen(const QPen &pen);
    void setWhiskerAntialiased(bool enabled);
    void setMedianPen(const QPen &pen);
    void setOutlierStyle(const QCPScatterStyle &style);

    // non-property methods:
    void addData(const QVector<double> &keys, const QVector<double> &minimum, const QVector<double> &lowerQuartile, const QVector<double> &median, const QVector<double> &upperQuartile, const QVector<double> &maximum, bool alreadySorted=false);
    void addData(double key, double minimum, double lowerQuartile, double median, double upperQuartile, double maximum, const QVector<double> &outliers=QVector<double>());

    // reimplemented virtual methods:
    virtual QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const Q_DECL_OVERRIDE;
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;
    virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const Q_DECL_OVERRIDE;
    virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const Q_DECL_OVERRIDE;

    protected:
    // property members:
    double mWidth;
    double mWhiskerWidth;
    QPen mWhiskerPen, mWhiskerBarPen;
    bool mWhiskerAntialiased;
    QPen mMedianPen;
    QCPScatterStyle mOutlierStyle;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const Q_DECL_OVERRIDE;

    // introduced virtual methods:
    virtual void drawStatisticalBox(QCPPainter *painter, QCPStatisticalBoxDataContainer::const_iterator it, const QCPScatterStyle &outlierStyle) const;

    // non-virtual methods:
    void getVisibleDataBounds(QCPStatisticalBoxDataContainer::const_iterator &begin, QCPStatisticalBoxDataContainer::const_iterator &end) const;
    QRectF getQuartileBox(QCPStatisticalBoxDataContainer::const_iterator it) const;
    QVector<QLineF> getWhiskerBackboneLines(QCPStatisticalBoxDataContainer::const_iterator it) const;
    QVector<QLineF> getWhiskerBarLines(QCPStatisticalBoxDataContainer::const_iterator it) const;

    friend class QCustomPlot;
    friend class QCPLegend;
};


} // QCP

#endif //QCUSTOMPLOT_QCPSTATISTICALBOX_H
