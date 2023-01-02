#ifndef QCUSTOMPLOT_QCPAXISTICKERFIXED_HPP
#define QCUSTOMPLOT_QCPAXISTICKERFIXED_HPP

#include "QCPAxisTicker.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerFixed : public QCPAxisTicker {
    Q_GADGET
    public:
        /*!
        Defines how the axis ticker may modify the specified tick step (\ref setTickStep) in order to
        control the number of ticks in the axis range.

        \see setScaleStrategy
      */
        enum ScaleStrategy {
            ssNone,    ///< Modifications are not allowed, the specified tick step is absolutely fixed. This might cause a high tick density and overlapping labels if the axis range is zoomed out.
            ssMultiples, ///< An integer multiple of the specified tick step is allowed. The used factor follows the base class properties of \ref setTickStepStrategy and \ref setTickCount.
            ssPowers,  ///< An integer power of the specified tick step is allowed.
        };

        Q_ENUM(ScaleStrategy)

        QCPAxisTickerFixed();

        double tickStep() const { return mTickStep; }

        ScaleStrategy scaleStrategy() const { return mScaleStrategy; }

        void setTickStep(double step);

        void setScaleStrategy(ScaleStrategy strategy);

    protected:
        double mTickStep;
        ScaleStrategy mScaleStrategy;

        double getTickStep(const QCPRange &range) override;
    };
}
Q_DECLARE_METATYPE(QCP::QCPAxisTickerFixed::ScaleStrategy)

#endif
