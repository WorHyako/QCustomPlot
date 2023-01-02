#ifndef QCUSTOMPLOT_QCPERRORBARS_H
#define QCUSTOMPLOT_QCPERRORBARS_H

namespace QCP {

    class QCP_LIB_DECL QCPErrorBars : public QCPAbstractPlottable, public QCPPlottableInterface1D
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QSharedPointer<QCPErrorBarsDataContainer> data READ data WRITE setData)
    Q_PROPERTY(QCPAbstractPlottable* dataPlottable READ dataPlottable WRITE setDataPlottable)
    Q_PROPERTY(ErrorType errorType READ errorType WRITE setErrorType)
    Q_PROPERTY(double whiskerWidth READ whiskerWidth WRITE setWhiskerWidth)
    Q_PROPERTY(double symbolGap READ symbolGap WRITE setSymbolGap)
    /// \endcond
    public:

    /*!
    Defines in which orientation the error bars shall appear. If your data needs both error
    dimensions, create two \ref QCPErrorBars with different \ref ErrorType.

    \see setErrorType
  */
    enum ErrorType { etKeyError    ///< The errors are for the key dimension (bars appear parallel to the key axis)
        ,etValueError ///< The errors are for the value dimension (bars appear parallel to the value axis)
    };
    Q_ENUMS(ErrorType)

    explicit QCPErrorBars(QCPAxis *keyAxis, QCPAxis *valueAxis);
    virtual ~QCPErrorBars() Q_DECL_OVERRIDE;
    // getters:
    QSharedPointer<QCPErrorBarsDataContainer> data() const { return mDataContainer; }
    QCPAbstractPlottable *dataPlottable() const { return mDataPlottable.data(); }
    ErrorType errorType() const { return mErrorType; }
    double whiskerWidth() const { return mWhiskerWidth; }
    double symbolGap() const { return mSymbolGap; }

    // setters:
    void setData(QSharedPointer<QCPErrorBarsDataContainer> data);
    void setData(const QVector<double> &error);
    void setData(const QVector<double> &errorMinus, const QVector<double> &errorPlus);
    void setDataPlottable(QCPAbstractPlottable* plottable);
    void setErrorType(ErrorType type);
    void setWhiskerWidth(double pixels);
    void setSymbolGap(double pixels);

    // non-property methods:
    void addData(const QVector<double> &error);
    void addData(const QVector<double> &errorMinus, const QVector<double> &errorPlus);
    void addData(double error);
    void addData(double errorMinus, double errorPlus);

    // virtual methods of 1d plottable interface:
    virtual int dataCount() const Q_DECL_OVERRIDE;
    virtual double dataMainKey(int index) const Q_DECL_OVERRIDE;
    virtual double dataSortKey(int index) const Q_DECL_OVERRIDE;
    virtual double dataMainValue(int index) const Q_DECL_OVERRIDE;
    virtual QCPRange dataValueRange(int index) const Q_DECL_OVERRIDE;
    virtual QPointF dataPixelPosition(int index) const Q_DECL_OVERRIDE;
    virtual bool sortKeyIsMainKey() const Q_DECL_OVERRIDE;
    virtual QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const Q_DECL_OVERRIDE;
    virtual int findBegin(double sortKey, bool expandedRange=true) const Q_DECL_OVERRIDE;
    virtual int findEnd(double sortKey, bool expandedRange=true) const Q_DECL_OVERRIDE;

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;
    virtual QCPPlottableInterface1D *interface1D() Q_DECL_OVERRIDE { return this; }

protected:
// property members:
QSharedPointer<QCPErrorBarsDataContainer> mDataContainer;
QPointer<QCPAbstractPlottable> mDataPlottable;
ErrorType mErrorType;
double mWhiskerWidth;
double mSymbolGap;

// reimplemented virtual methods:
virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const Q_DECL_OVERRIDE;
virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth) const Q_DECL_OVERRIDE;
virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain=QCP::sdBoth, const QCPRange &inKeyRange=QCPRange()) const Q_DECL_OVERRIDE;

// non-virtual methods:
void getErrorBarLines(QCPErrorBarsDataContainer::const_iterator it, QVector<QLineF> &backbones, QVector<QLineF> &whiskers) const;
void getVisibleDataBounds(QCPErrorBarsDataContainer::const_iterator &begin, QCPErrorBarsDataContainer::const_iterator &end, const QCPDataRange &rangeRestriction) const;
double pointDistance(const QPointF &pixelPoint, QCPErrorBarsDataContainer::const_iterator &closestData) const;
// helpers:
void getDataSegments(QList<QCPDataRange> &selectedSegments, QList<QCPDataRange> &unselectedSegments) const;
bool errorBarVisible(int index) const;
bool rectIntersectsLine(const QRectF &pixelRect, const QLineF &line) const;

friend class QCustomPlot;
friend class QCPLegend;
};


} // QCP

#endif //QCUSTOMPLOT_QCPERRORBARS_H
