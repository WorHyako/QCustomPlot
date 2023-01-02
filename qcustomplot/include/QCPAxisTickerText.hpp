#ifndef QCUSTOMPLOT_QCPAXISTICKERTEXT_HPP
#define QCUSTOMPLOT_QCPAXISTICKERTEXT_HPP

#include "QCPAxisTicker.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerText : public QCPAxisTicker {
    public:
        QCPAxisTickerText();

        QMap<double, QString> &ticks() { return mTicks; }

        int subTickCount() const { return mSubTickCount; }

        void setTicks(const QMap<double, QString> &ticks);

        void setTicks(const QVector<double> &positions, const QVector<QString> &labels);

        void setSubTickCount(int subTicks);

        void clear();

        void addTick(double position, const QString &label);

        void addTicks(const QMap<double, QString> &ticks);

        void addTicks(const QVector<double> &positions, const QVector<QString> &labels);

    protected:
        QMap<double, QString> mTicks;
        int mSubTickCount;

        double getTickStep(const QCPRange &range) override;

        int getSubTickCount(double tickStep) override;

        QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) override;

        QVector<double> createTickVector(double tickStep, const QCPRange &range) override;
    };
}

#endif
