#ifndef QCUSTOMPLOT_QCPAXISTICKERDATETIME_H
#define QCUSTOMPLOT_QCPAXISTICKERDATETIME_H

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerDateTime : public QCPAxisTicker
{
    public:
    QCPAxisTickerDateTime();

    // getters:
    QString dateTimeFormat() const { return mDateTimeFormat; }
    Qt::TimeSpec dateTimeSpec() const { return mDateTimeSpec; }
# if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    QTimeZone timeZone() const { return mTimeZone; }
#endif

    // setters:
    void setDateTimeFormat(const QString &format);
    void setDateTimeSpec(Qt::TimeSpec spec);
# if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    void setTimeZone(const QTimeZone &zone);
# endif
    void setTickOrigin(double origin); // hides base class method but calls baseclass implementation ("using" throws off IDEs and doxygen)
    void setTickOrigin(const QDateTime &origin);

    // static methods:
    static QDateTime keyToDateTime(double key);
    static double dateTimeToKey(const QDateTime &dateTime);
    static double dateTimeToKey(const QDate &date, Qt::TimeSpec timeSpec=Qt::LocalTime);

    protected:
    // property members:
    QString mDateTimeFormat;
    Qt::TimeSpec mDateTimeSpec;
# if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    QTimeZone mTimeZone;
# endif
    // non-property members:
    enum DateStrategy {dsNone, dsUniformTimeInDay, dsUniformDayInMonth} mDateStrategy;

    // reimplemented virtual methods:
    virtual double getTickStep(const QCPRange &range) Q_DECL_OVERRIDE;
    virtual int getSubTickCount(double tickStep) Q_DECL_OVERRIDE;
    virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) Q_DECL_OVERRIDE;
    virtual QVector<double> createTickVector(double tickStep, const QCPRange &range) Q_DECL_OVERRIDE;
};

/* end of 'src/axis/axistickerdatetime.h' */


} // QCP

#endif //QCUSTOMPLOT_QCPAXISTICKERDATETIME_H
