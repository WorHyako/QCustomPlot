#ifndef QCUSTOMPLOT_QCPAXISTICKER_HPP
#define QCUSTOMPLOT_QCPAXISTICKER_HPP

#include "defs.hpp"
#include "QCPRange.hpp"

#include <QObject>
#include <QVector>

namespace QCP {

    class QCP_LIB_DECL QCPAxisTicker {
    Q_GADGET
    public:
        /*!
        Defines the strategies that the axis ticker may follow when choosing the size of the tick step.

        \see setTickStepStrategy
      */
        enum TickStepStrategy {
            tssReadability    ///< A nicely readable tick step is prioritized over matching the requested number of ticks (see \ref setTickCount)
            ,
            tssMeetTickCount ///< Less readable tick steps are allowed which in turn facilitates getting closer to the requested tick count
        };
        Q_ENUMS(TickStepStrategy)

        QCPAxisTicker();

        virtual ~QCPAxisTicker();

        Q_DISABLE_COPY(QCPAxisTicker)

        TickStepStrategy tickStepStrategy() const { return mTickStepStrategy; }

        int tickCount() const { return mTickCount; }

        double tickOrigin() const { return mTickOrigin; }

        void setTickStepStrategy(TickStepStrategy strategy);

        void setTickCount(int count);

        void setTickOrigin(double origin);

        virtual void
        generate(const QCPRange &range, const QLocale &locale, QChar formatChar, int precision, QVector<double> &ticks,
                 QVector<double> *subTicks, QVector<QString> *tickLabels);

    protected:
        TickStepStrategy mTickStepStrategy;
        int mTickCount;
        double mTickOrigin;

        virtual double getTickStep(const QCPRange &range);

        virtual int getSubTickCount(double tickStep);

        virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision);

        virtual QVector<double> createTickVector(double tickStep, const QCPRange &range);

        virtual QVector<double> createSubTickVector(int subTickCount, const QVector<double> &ticks);

        virtual QVector<QString>
        createLabelVector(const QVector<double> &ticks, const QLocale &locale, QChar formatChar, int precision);

        void trimTicks(const QCPRange &range, QVector<double> &ticks, bool keepOneOutlier) const;

        double pickClosest(double target, const QVector<double> &candidates) const;

        double getMantissa(double input, double *magnitude = nullptr) const;

        double cleanMantissa(double input) const;
    };
}
Q_DECLARE_METATYPE(QCP::QCPAxisTicker::TickStepStrategy)

Q_DECLARE_METATYPE(QSharedPointer<QCP::QCPAxisTicker>)

#endif
