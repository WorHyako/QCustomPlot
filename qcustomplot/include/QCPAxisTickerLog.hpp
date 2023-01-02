#ifndef QCUSTOMPLOT_QCPAXISTICKERLOG_HPP
#define QCUSTOMPLOT_QCPAXISTICKERLOG_HPP

#include "QCPAxisTicker.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerLog : public QCPAxisTicker {
    public:
        QCPAxisTickerLog();

        double logBase() const { return mLogBase; }

        int subTickCount() const { return mSubTickCount; }

        void setLogBase(double base);

        void setSubTickCount(int subTicks);

    protected:
        double mLogBase;
        int mSubTickCount;

        double mLogBaseLnInv;

        int getSubTickCount(double tickStep) override;

        QVector<double> createTickVector(double tickStep, const QCPRange &range) override;
    };
}

#endif
