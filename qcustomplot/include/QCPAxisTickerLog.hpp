#ifndef QCUSTOMPLOT_QCPAXISTICKERLOG_H
#define QCUSTOMPLOT_QCPAXISTICKERLOG_H

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerLog : public QCPAxisTicker
{
    public:
    QCPAxisTickerLog();

    // getters:
    double logBase() const { return mLogBase; }
    int subTickCount() const { return mSubTickCount; }

    // setters:
    void setLogBase(double base);
    void setSubTickCount(int subTicks);

    protected:
    // property members:
    double mLogBase;
    int mSubTickCount;

    // non-property members:
    double mLogBaseLnInv;

    // reimplemented virtual methods:
    virtual int getSubTickCount(double tickStep) Q_DECL_OVERRIDE;
    virtual QVector<double> createTickVector(double tickStep, const QCPRange &range) Q_DECL_OVERRIDE;
};


} // QCP

#endif //QCUSTOMPLOT_QCPAXISTICKERLOG_H
