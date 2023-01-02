#ifndef QCUSTOMPLOT_QCPAXISTICKERDATETIME_HPP
#define QCUSTOMPLOT_QCPAXISTICKERDATETIME_HPP

#include "QCPAxisTicker.hpp"

#include <QTimeZone>

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerDateTime : public QCPAxisTicker {
    public:
        QCPAxisTickerDateTime();

        QString dateTimeFormat() const { return mDateTimeFormat; }

        Qt::TimeSpec dateTimeSpec() const { return mDateTimeSpec; }

# if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)

        QTimeZone timeZone() const { return mTimeZone; }

#endif

        void setDateTimeFormat(const QString &format);

        void setDateTimeSpec(Qt::TimeSpec spec);

# if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)

        void setTimeZone(const QTimeZone &zone);

# endif

        void setTickOrigin(
                double origin); // hides base class method but calls baseclass implementation ("using" throws off IDEs and doxygen)

        void setTickOrigin(const QDateTime &origin);

        static QDateTime keyToDateTime(double key);

        static double dateTimeToKey(const QDateTime &dateTime);

        static double dateTimeToKey(const QDate &date, Qt::TimeSpec timeSpec = Qt::LocalTime);

    protected:
        QString mDateTimeFormat;
        Qt::TimeSpec mDateTimeSpec;
# if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
        QTimeZone mTimeZone;
# endif
        enum DateStrategy {
            dsNone, dsUniformTimeInDay, dsUniformDayInMonth
        } mDateStrategy;

        double getTickStep(const QCPRange &range) override;

        int getSubTickCount(double tickStep) override;

        QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) override;

        QVector<double> createTickVector(double tickStep, const QCPRange &range) override;
    };
}

#endif
