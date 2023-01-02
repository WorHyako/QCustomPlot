#ifndef QCUSTOMPLOT_QCPBARS_H
#define QCUSTOMPLOT_QCPBARS_H

namespace QCP {

    class QCP_LIB_DECL QCPBars : public QCPAbstractPlottable1D<QCPBarsData>
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(double width READ width WRITE setWidth)
    Q_PROPERTY(WidthType widthType READ widthType WRITE setWidthType)
    Q_PROPERTY(QCPBarsGroup* barsGroup READ barsGroup WRITE setBarsGroup)
    Q_PROPERTY(double baseValue READ baseValue WRITE setBaseValue)
    Q_PROPERTY(double stackingGap READ stackingGap WRITE setStackingGap)
    Q_PROPERTY(QCPBars* barBelow READ barBelow)
    Q_PROPERTY(QCPBars* barAbove READ barAbove)
    /// \endcond
    public:
    /*!
    Defines the ways the width of the bar can be specified. Thus it defines what the number passed
    to \ref setWidth actually means.

    \see setWidthType, setWidth
  */
    enum WidthType { wtAbsolute       ///< Bar width is in absolute pixels
        ,wtAxisRectRatio ///< Bar width is given by a fraction of the axis rect size
        ,wtPlotCoords    ///< Bar width is in key coordinates and thus scales with the key axis range
    };
    Q_ENUMS(WidthType)

    explicit QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis);
    virtual ~QCPBars() Q_DECL_OVERRIDE;

    // getters:
    double width() const { return mWidth; }
    WidthType widthType() const { return mWidthType; }
    QCPBarsGroup *barsGroup() const { return mBarsGroup; }
    double baseValue() const { return mBaseValue; }
    double stackingGap() const { return mStackingGap; }
    QCPBars *barBelow() const { return mBarBelow.data(); }
    QCPBars *barAbove() const { return mBarAbove.data(); }
    QSharedPointer<QCPBarsDataContainer> data() const { return mDataContainer; }

    // setters:
    void setData(QSharedPointer<QCPBarsDataContainer> data);
    void setData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted=false);
    void setWidth(double width);
    void setWidthType(WidthType widthType);
    void setBarsGroup(QCPBarsGroup *barsGroup);
    void setBaseValue(double baseValue);
    void setStackingGap(double pixels);

    // non-property methods:
    void addData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted=false);
    void addData(double key, double value);
    void moveBelow(QCPBars *bars);
    void moveAbove(QCPBars *bars);

    // reimplemented virtual methods:
    virtual QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const Q_DECL_OVERRIDE;
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;
    virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const Q_DECL_OVERRIDE;
    virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const Q_DECL_OVERRIDE;
    virtual QPointF dataPixelPosition(int index) const Q_DECL_OVERRIDE;

    protected:
    // property members:
    double mWidth;
    WidthType mWidthType;
    QCPBarsGroup *mBarsGroup;
    double mBaseValue;
    double mStackingGap;
    QPointer<QCPBars> mBarBelow, mBarAbove;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const Q_DECL_OVERRIDE;

    // non-virtual methods:
    void getVisibleDataBounds(QCPBarsDataContainer::const_iterator &begin, QCPBarsDataContainer::const_iterator &end) const;
    QRectF getBarRect(double key, double value) const;
    void getPixelWidth(double key, double &lower, double &upper) const;
    double getStackedBaseValue(double key, bool positive) const;
    static void connectBars(QCPBars* lower, QCPBars* upper);

    friend class QCustomPlot;
    friend class QCPLegend;
    friend class QCPBarsGroup;
};
Q_DECLARE_METATYPE(QCPBars::WidthType)


} // QCP

#endif //QCUSTOMPLOT_QCPBARS_H
