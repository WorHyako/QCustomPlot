#ifndef QCUSTOMPLOT_QCPAXISTICKERTIME_HPP
#define QCUSTOMPLOT_QCPAXISTICKERTIME_HPP

#include "QCPAxisTicker.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerTime : public QCPAxisTicker {
    Q_GADGET
    public:
        /*!
        Defines the logical units in which fractions of time spans can be expressed.

        \see setFieldWidth, setTimeFormat
      */
        enum TimeUnit {
            tuMilliseconds,///< Milliseconds, one thousandth of a second (%%z in \ref setTimeFormat)
            tuSeconds,  ///< Seconds (%%s in \ref setTimeFormat)
            tuMinutes,    ///< Minutes (%%m in \ref setTimeFormat)
            tuHours,    ///< Hours (%%h in \ref setTimeFormat)
            tuDays,   ///< Days (%%d in \ref setTimeFormat)
        };

        Q_ENUM(TimeUnit)

        QCPAxisTickerTime();

        QString timeFormat() const { return mTimeFormat; }

        int fieldWidth(TimeUnit unit) const { return mFieldWidth.value(unit); }

        void setTimeFormat(const QString &format);

        void setFieldWidth(TimeUnit unit, int width);

    protected:
        QString mTimeFormat;
        QHash<TimeUnit, int> mFieldWidth;

        TimeUnit mSmallestUnit, mBiggestUnit;
        QHash<TimeUnit, QString> mFormatPattern;

        double getTickStep(const QCPRange &range) override;

        int getSubTickCount(double tickStep) override;

        QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) override;

        void replaceUnit(QString &text, TimeUnit unit, int value) const;
    };
}
Q_DECLARE_METATYPE(QCP::QCPAxisTickerTime::TimeUnit)

#endif
