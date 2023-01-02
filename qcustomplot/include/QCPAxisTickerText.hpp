#ifndef QCUSTOMPLOT_QCPAXISTICKERTEXT_H
#define QCUSTOMPLOT_QCPAXISTICKERTEXT_H

namespace QCP {
    class QCP_LIB_DECL QCPAxisTickerText : public QCPAxisTicker
{
    public:
    QCPAxisTickerText();

    // getters:
    QMap<double, QString> &ticks() { return mTicks; }
    int subTickCount() const { return mSubTickCount; }

    // setters:
    void setTicks(const QMap<double, QString> &ticks);
    void setTicks(const QVector<double> &positions, const QVector<QString> &labels);
    void setSubTickCount(int subTicks);

    // non-virtual methods:
    void clear();
    void addTick(double position, const QString &label);
    void addTicks(const QMap<double, QString> &ticks);
    void addTicks(const QVector<double> &positions, const QVector<QString> &labels);

    protected:
    // property members:
    QMap<double, QString> mTicks;
    int mSubTickCount;

    // reimplemented virtual methods:
    virtual double getTickStep(const QCPRange &range) Q_DECL_OVERRIDE;
    virtual int getSubTickCount(double tickStep) Q_DECL_OVERRIDE;
    virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) Q_DECL_OVERRIDE;
    virtual QVector<double> createTickVector(double tickStep, const QCPRange &range) Q_DECL_OVERRIDE;
};


} // QCP

#endif //QCUSTOMPLOT_QCPAXISTICKERTEXT_H
