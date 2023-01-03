#ifndef QCUSTOMPLOT_QCPCURVE_HPP
#define QCUSTOMPLOT_QCPCURVE_HPP

#include "defs.hpp"
#include "QCPAbstractPlottable1D.hpp"
#include "QCPCurveData.hpp"
#include "QCPScatterStyle.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPCurve : public QCPAbstractPlottable1D<QCPCurveData> {
    Q_OBJECT
        Q_PROPERTY(QCPScatterStyle scatterStyle READ scatterStyle WRITE setScatterStyle)
        Q_PROPERTY(int scatterSkip READ scatterSkip WRITE setScatterSkip)
        Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle)
    public:
        Q_DISABLE_COPY(QCPCurve)

        /*!
        Defines how the curve's line is represented visually in the plot. The line is drawn with the
        current pen of the curve (\ref setPen).
        \see setLineStyle
      */
        enum LineStyle {
            lsNone  ///< No line is drawn between data points (e.g. only scatters)
            , lsLine ///< Data points are connected with a straight line
        };
        Q_ENUMS(LineStyle)

        explicit QCPCurve(QCPAxis *keyAxis, QCPAxis *valueAxis);

        ~QCPCurve() override;

        QSharedPointer<QCPCurveDataContainer> data() const { return mDataContainer; }

        QCPScatterStyle scatterStyle() const { return mScatterStyle; }

        int scatterSkip() const { return mScatterSkip; }

        LineStyle lineStyle() const { return mLineStyle; }

        void setData(QSharedPointer<QCPCurveDataContainer> data);

        void setData(const QVector<double> &t, const QVector<double> &keys, const QVector<double> &values,
                     bool alreadySorted = false);

        void setData(const QVector<double> &keys, const QVector<double> &values);

        void setScatterStyle(const QCPScatterStyle &style);

        void setScatterSkip(int skip);

        void setLineStyle(LineStyle style);

        void addData(const QVector<double> &t, const QVector<double> &keys, const QVector<double> &values,
                     bool alreadySorted = false);

        void addData(const QVector<double> &keys, const QVector<double> &values);

        void addData(double t, double key, double value);

        void addData(double key, double value);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth) const override;

        QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth,
                               const QCPRange &inKeyRange = QCPRange()) const override;

    protected:
        QCPScatterStyle mScatterStyle;
        int mScatterSkip;
        LineStyle mLineStyle;

        void draw(QCPPainter *painter) override;

        void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const override;

        virtual void drawCurveLine(QCPPainter *painter, const QVector<QPointF> &lines) const;

        virtual void
        drawScatterPlot(QCPPainter *painter, const QVector<QPointF> &points, const QCPScatterStyle &style) const;

        void getCurveLines(QVector<QPointF> *lines, const QCPDataRange &dataRange, double penWidth) const;

        void getScatters(QVector<QPointF> *scatters, const QCPDataRange &dataRange, double scatterWidth) const;

        int getRegion(double key, double value, double keyMin, double valueMax, double keyMax, double valueMin) const;

        QPointF
        getOptimizedPoint(int otherRegion, double otherKey, double otherValue, double key, double value, double keyMin,
                          double valueMax, double keyMax, double valueMin) const;

        QVector<QPointF>
        getOptimizedCornerPoints(int prevRegion, int currentRegion, double prevKey, double prevValue, double key,
                                 double value, double keyMin, double valueMax, double keyMax, double valueMin) const;

        bool mayTraverse(int prevRegion, int currentRegion) const;

        bool getTraverse(double prevKey, double prevValue, double key, double value, double keyMin, double valueMax,
                         double keyMax, double valueMin, QPointF &crossA, QPointF &crossB) const;

        void getTraverseCornerPoints(int prevRegion, int currentRegion, double keyMin, double valueMax, double keyMax,
                                     double valueMin, QVector<QPointF> &beforeTraverse,
                                     QVector<QPointF> &afterTraverse) const;

        double pointDistance(const QPointF &pixelPoint, QCPCurveDataContainer::const_iterator &closestData) const;

    };
}
Q_DECLARE_METATYPE(QCP::QCPCurve::LineStyle)

#endif
