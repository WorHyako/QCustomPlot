#ifndef QCUSTOMPLOT_QCPERRORBARS_HPP
#define QCUSTOMPLOT_QCPERRORBARS_HPP

#include "defs.hpp"
#include "QCPAbstractPlottable.hpp"
#include "QCPPlottableInterface1D.hpp"
#include "QCPErrorBarsData.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPErrorBars : public QCPAbstractPlottable, public QCPPlottableInterface1D {
    Q_OBJECT
        Q_PROPERTY(QSharedPointer<QCPErrorBarsDataContainer> data READ data WRITE setData)
        Q_PROPERTY(QCPAbstractPlottable *dataPlottable READ dataPlottable WRITE setDataPlottable)
        Q_PROPERTY(ErrorType errorType READ errorType WRITE setErrorType)
        Q_PROPERTY(double whiskerWidth READ whiskerWidth WRITE setWhiskerWidth)
        Q_PROPERTY(double symbolGap READ symbolGap WRITE setSymbolGap)
    public:
        Q_DISABLE_COPY(QCPErrorBars)

        /*!
        Defines in which orientation the error bars shall appear. If your data needs both error
        dimensions, create two \ref QCPErrorBars with different \ref ErrorType.

        \see setErrorType
      */
        enum ErrorType {
            etKeyError    ///< The errors are for the key dimension (bars appear parallel to the key axis)
            , etValueError ///< The errors are for the value dimension (bars appear parallel to the value axis)
        };

        Q_ENUM(ErrorType)

        explicit QCPErrorBars(QCPAxis *keyAxis, QCPAxis *valueAxis);

        ~QCPErrorBars() override;

        QSharedPointer<QCPErrorBarsDataContainer> data() const { return mDataContainer; }

        QCPAbstractPlottable *dataPlottable() const { return mDataPlottable.data(); }

        ErrorType errorType() const { return mErrorType; }

        double whiskerWidth() const { return mWhiskerWidth; }

        double symbolGap() const { return mSymbolGap; }

        void setData(QSharedPointer<QCPErrorBarsDataContainer> data);

        void setData(const QVector<double> &error);

        void setData(const QVector<double> &errorMinus, const QVector<double> &errorPlus);

        void setDataPlottable(QCPAbstractPlottable *plottable);

        void setErrorType(ErrorType type);

        void setWhiskerWidth(double pixels);

        void setSymbolGap(double pixels);

        void addData(const QVector<double> &error);

        void addData(const QVector<double> &errorMinus, const QVector<double> &errorPlus);

        void addData(double error);

        void addData(double errorMinus, double errorPlus);

        int dataCount() const override;

        double dataMainKey(int index) const override;

        double dataSortKey(int index) const override;

        double dataMainValue(int index) const override;

        QCPRange dataValueRange(int index) const override;

        QPointF dataPixelPosition(int index) const override;

        bool sortKeyIsMainKey() const override;

        QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const override;

        int findBegin(double sortKey, bool expandedRange = true) const override;

        int findEnd(double sortKey, bool expandedRange = true) const override;

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPPlottableInterface1D *interface1D() override { return this; }

    protected:
        QSharedPointer<QCPErrorBarsDataContainer> mDataContainer;
        QPointer<QCPAbstractPlottable> mDataPlottable;
        ErrorType mErrorType;
        double mWhiskerWidth;
        double mSymbolGap;

        void draw(QCPPainter *painter) override;

        void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const override;

        QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth) const override;

        QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth,
                               const QCPRange &inKeyRange = QCPRange()) const override;

        void getErrorBarLines(QCPErrorBarsDataContainer::const_iterator it, QVector<QLineF> &backbones,
                              QVector<QLineF> &whiskers) const;

        void getVisibleDataBounds(QCPErrorBarsDataContainer::const_iterator &begin,
                                  QCPErrorBarsDataContainer::const_iterator &end,
                                  const QCPDataRange &rangeRestriction) const;

        double pointDistance(const QPointF &pixelPoint, QCPErrorBarsDataContainer::const_iterator &closestData) const;

        void getDataSegments(QList<QCPDataRange> &selectedSegments, QList<QCPDataRange> &unselectedSegments) const;

        bool errorBarVisible(int index) const;

        bool rectIntersectsLine(const QRectF &pixelRect, const QLineF &line) const;
    };
}
#endif
