#ifndef QCUSTOMPLOT_QCPFINANCIAL_H
#define QCUSTOMPLOT_QCPFINANCIAL_H

namespace QCP {


    class QCP_LIB_DECL QCPFinancial : public QCPAbstractPlottable1D<QCPFinancialData>
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(ChartStyle chartStyle READ chartStyle WRITE setChartStyle)
    Q_PROPERTY(double width READ width WRITE setWidth)
    Q_PROPERTY(WidthType widthType READ widthType WRITE setWidthType)
    Q_PROPERTY(bool twoColored READ twoColored WRITE setTwoColored)
    Q_PROPERTY(QBrush brushPositive READ brushPositive WRITE setBrushPositive)
    Q_PROPERTY(QBrush brushNegative READ brushNegative WRITE setBrushNegative)
    Q_PROPERTY(QPen penPositive READ penPositive WRITE setPenPositive)
    Q_PROPERTY(QPen penNegative READ penNegative WRITE setPenNegative)
    /// \endcond
    public:
    /*!
    Defines the ways the width of the financial bar can be specified. Thus it defines what the
    number passed to \ref setWidth actually means.

    \see setWidthType, setWidth
  */
    enum WidthType { wtAbsolute       ///< width is in absolute pixels
        ,wtAxisRectRatio ///< width is given by a fraction of the axis rect size
        ,wtPlotCoords    ///< width is in key coordinates and thus scales with the key axis range
    };
    Q_ENUMS(WidthType)

    /*!
    Defines the possible representations of OHLC data in the plot.

    \see setChartStyle
  */
    enum ChartStyle { csOhlc         ///< Open-High-Low-Close bar representation
        ,csCandlestick  ///< Candlestick representation
    };
    Q_ENUMS(ChartStyle)

    explicit QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis);
    virtual ~QCPFinancial() Q_DECL_OVERRIDE;

    // getters:
    QSharedPointer<QCPFinancialDataContainer> data() const { return mDataContainer; }
    ChartStyle chartStyle() const { return mChartStyle; }
    double width() const { return mWidth; }
    WidthType widthType() const { return mWidthType; }
    bool twoColored() const { return mTwoColored; }
    QBrush brushPositive() const { return mBrushPositive; }
    QBrush brushNegative() const { return mBrushNegative; }
    QPen penPositive() const { return mPenPositive; }
    QPen penNegative() const { return mPenNegative; }

    // setters:
    void setData(QSharedPointer<QCPFinancialDataContainer> data);
    void setData(const QVector<double> &keys, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close, bool alreadySorted=false);
    void setChartStyle(ChartStyle style);
    void setWidth(double width);
    void setWidthType(WidthType widthType);
    void setTwoColored(bool twoColored);
    void setBrushPositive(const QBrush &brush);
    void setBrushNegative(const QBrush &brush);
    void setPenPositive(const QPen &pen);
    void setPenNegative(const QPen &pen);

    // non-property methods:
    void addData(const QVector<double> &keys, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close, bool alreadySorted=false);
    void addData(double key, double open, double high, double low, double close);

    // reimplemented virtual methods:
    virtual QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const Q_DECL_OVERRIDE;
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;
    virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const Q_DECL_OVERRIDE;
    virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const Q_DECL_OVERRIDE;

    // static methods:
    static QCPFinancialDataContainer timeSeriesToOhlc(const QVector<double> &time, const QVector<double> &value, double timeBinSize, double timeBinOffset = 0);

    protected:
    // property members:
    ChartStyle mChartStyle;
    double mWidth;
    WidthType mWidthType;
    bool mTwoColored;
    QBrush mBrushPositive, mBrushNegative;
    QPen mPenPositive, mPenNegative;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const Q_DECL_OVERRIDE;

    // non-virtual methods:
    void drawOhlcPlot(QCPPainter *painter, const QCPFinancialDataContainer::const_iterator &begin, const QCPFinancialDataContainer::const_iterator &end, bool isSelected);
    void drawCandlestickPlot(QCPPainter *painter, const QCPFinancialDataContainer::const_iterator &begin, const QCPFinancialDataContainer::const_iterator &end, bool isSelected);
    double getPixelWidth(double key, double keyPixel) const;
    double ohlcSelectTest(const QPointF &pos, const QCPFinancialDataContainer::const_iterator &begin, const QCPFinancialDataContainer::const_iterator &end, QCPFinancialDataContainer::const_iterator &closestDataPoint) const;
    double candlestickSelectTest(const QPointF &pos, const QCPFinancialDataContainer::const_iterator &begin, const QCPFinancialDataContainer::const_iterator &end, QCPFinancialDataContainer::const_iterator &closestDataPoint) const;
    void getVisibleDataBounds(QCPFinancialDataContainer::const_iterator &begin, QCPFinancialDataContainer::const_iterator &end) const;
    QRectF selectionHitBox(QCPFinancialDataContainer::const_iterator it) const;

    friend class QCustomPlot;
    friend class QCPLegend;
};
Q_DECLARE_METATYPE(QCPFinancial::ChartStyle)

} // QCP

#endif //QCUSTOMPLOT_QCPFINANCIAL_H
