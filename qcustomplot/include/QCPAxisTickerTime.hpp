#ifndef QCUSTOMPLOT_QCPAXISTICKERTIME_H
#define QCUSTOMPLOT_QCPAXISTICKERTIME_H

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerTime : public QCPAxisTicker
{
    Q_GADGET
    public:
    /*!
    Defines the logical units in which fractions of time spans can be expressed.

    \see setFieldWidth, setTimeFormat
  */
    enum TimeUnit { tuMilliseconds ///< Milliseconds, one thousandth of a second (%%z in \ref setTimeFormat)
        ,tuSeconds     ///< Seconds (%%s in \ref setTimeFormat)
        ,tuMinutes     ///< Minutes (%%m in \ref setTimeFormat)
        ,tuHours       ///< Hours (%%h in \ref setTimeFormat)
        ,tuDays        ///< Days (%%d in \ref setTimeFormat)
    };
    Q_ENUMS(TimeUnit)

    QCPAxisTickerTime();

    // getters:
    QString timeFormat() const { return mTimeFormat; }
    int fieldWidth(TimeUnit unit) const { return mFieldWidth.value(unit); }

    // setters:
    void setTimeFormat(const QString &format);
    void setFieldWidth(TimeUnit unit, int width);

    protected:
    // property members:
    QString mTimeFormat;
    QHash<TimeUnit, int> mFieldWidth;

    // non-property members:
    TimeUnit mSmallestUnit, mBiggestUnit;
    QHash<TimeUnit, QString> mFormatPattern;

    // reimplemented virtual methods:
    virtual double getTickStep(const QCPRange &range) Q_DECL_OVERRIDE;
    virtual int getSubTickCount(double tickStep) Q_DECL_OVERRIDE;
    virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) Q_DECL_OVERRIDE;

    // non-virtual methods:
    void replaceUnit(QString &text, TimeUnit unit, int value) const;
};
Q_DECLARE_METATYPE(QCPAxisTickerTime::TimeUnit)


} // QCP

#endif //QCUSTOMPLOT_QCPAXISTICKERTIME_H
