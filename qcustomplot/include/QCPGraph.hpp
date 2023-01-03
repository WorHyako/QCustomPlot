#ifndef QCUSTOMPLOT_QCPGRAPH_HPP
#define QCUSTOMPLOT_QCPGRAPH_HPP

#include "defs.hpp"
#include "QCPAbstractPlottable1D.hpp"
#include "QCPGraphData.hpp"
#include "QCPScatterStyle.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPGraph : public QCPAbstractPlottable1D<QCPGraphData> {
    Q_OBJECT
        Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle)
        Q_PROPERTY(QCPScatterStyle scatterStyle READ scatterStyle WRITE setScatterStyle)
        Q_PROPERTY(int scatterSkip READ scatterSkip WRITE setScatterSkip)
        Q_PROPERTY(QCPGraph *channelFillGraph READ channelFillGraph WRITE setChannelFillGraph)
        Q_PROPERTY(bool adaptiveSampling READ adaptiveSampling WRITE setAdaptiveSampling)
    public:
        /*!
        Defines how the graph's line is represented visually in the plot. The line is drawn with the
        current pen of the graph (\ref setPen).
        \see setLineStyle
      */
        enum LineStyle {
            lsNone,   ///< data points are not connected with any lines (e.g. data only represented
            ///< with symbols according to the scatter style, see \ref setScatterStyle)
            lsLine,    ///< data points are connected by a straight line
            lsStepLeft,  ///< line is drawn as steps where the step height is the value of the left data point
            lsStepRight, ///< line is drawn as steps where the step height is the value of the right data point
            lsStepCenter, ///< line is drawn as steps where the step is in between two data points
            lsImpulse    ///< each data point is represented by a line parallel to the value axis, which reaches from the data point to the zero-value-line
        };

        Q_ENUM(LineStyle)

        Q_DISABLE_COPY(QCPGraph)

        explicit QCPGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);

        ~QCPGraph() override;

        QSharedPointer<QCPGraphDataContainer> data() const { return mDataContainer; }

        LineStyle lineStyle() const { return mLineStyle; }

        QCPScatterStyle scatterStyle() const { return mScatterStyle; }

        int scatterSkip() const { return mScatterSkip; }

        QCPGraph *channelFillGraph() const { return mChannelFillGraph.data(); }

        bool adaptiveSampling() const { return mAdaptiveSampling; }

        void setData(QSharedPointer<QCPGraphDataContainer> data);

        void setData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted = false);

        void setLineStyle(LineStyle ls);

        void setScatterStyle(const QCPScatterStyle &style);

        void setScatterSkip(int skip);

        void setChannelFillGraph(QCPGraph *targetGraph);

        void setAdaptiveSampling(bool enabled);

        void addData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted = false);

        void addData(double key, double value);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth) const override;

        QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth,
                               const QCPRange &inKeyRange = QCPRange()) const override;

    protected:
        LineStyle mLineStyle;
        QCPScatterStyle mScatterStyle;
        int mScatterSkip;
        QPointer<QCPGraph> mChannelFillGraph;
        bool mAdaptiveSampling;

        void draw(QCPPainter *painter) override;

        void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const override;

        virtual void drawFill(QCPPainter *painter, QVector<QPointF> *lines) const;

        virtual void
        drawScatterPlot(QCPPainter *painter, const QVector<QPointF> &scatters, const QCPScatterStyle &style) const;

        virtual void drawLinePlot(QCPPainter *painter, const QVector<QPointF> &lines) const;

        virtual void drawImpulsePlot(QCPPainter *painter, const QVector<QPointF> &lines) const;

        virtual void
        getOptimizedLineData(QVector<QCPGraphData> *lineData, const QCPGraphDataContainer::const_iterator &begin,
                             const QCPGraphDataContainer::const_iterator &end) const;

        virtual void
        getOptimizedScatterData(QVector<QCPGraphData> *scatterData, QCPGraphDataContainer::const_iterator begin,
                                QCPGraphDataContainer::const_iterator end) const;

        void
        getVisibleDataBounds(QCPGraphDataContainer::const_iterator &begin, QCPGraphDataContainer::const_iterator &end,
                             const QCPDataRange &rangeRestriction) const;

        void getLines(QVector<QPointF> *lines, const QCPDataRange &dataRange) const;

        void getScatters(QVector<QPointF> *scatters, const QCPDataRange &dataRange) const;

        QVector<QPointF> dataToLines(const QVector<QCPGraphData> &data) const;

        QVector<QPointF> dataToStepLeftLines(const QVector<QCPGraphData> &data) const;

        QVector<QPointF> dataToStepRightLines(const QVector<QCPGraphData> &data) const;

        QVector<QPointF> dataToStepCenterLines(const QVector<QCPGraphData> &data) const;

        QVector<QPointF> dataToImpulseLines(const QVector<QCPGraphData> &data) const;

        QVector<QCPDataRange> getNonNanSegments(const QVector<QPointF> *lineData, Qt::Orientation keyOrientation) const;

        QVector<QPair<QCPDataRange, QCPDataRange> >
        getOverlappingSegments(QVector<QCPDataRange> thisSegments, const QVector<QPointF> *thisData,
                               QVector<QCPDataRange> otherSegments, const QVector<QPointF> *otherData) const;

        bool segmentsIntersect(double aLower, double aUpper, double bLower, double bUpper, int &bPrecedence) const;

        QPointF getFillBasePoint(QPointF matchingDataPoint) const;

        const QPolygonF getFillPolygon(const QVector<QPointF> *lineData, QCPDataRange segment) const;

        const QPolygonF getChannelFillPolygon(const QVector<QPointF> *thisData, QCPDataRange thisSegment,
                                              const QVector<QPointF> *otherData, QCPDataRange otherSegment) const;

        int findIndexBelowX(const QVector<QPointF> *data, double x) const;

        int findIndexAboveX(const QVector<QPointF> *data, double x) const;

        int findIndexBelowY(const QVector<QPointF> *data, double y) const;

        int findIndexAboveY(const QVector<QPointF> *data, double y) const;

        double pointDistance(const QPointF &pixelPoint, QCPGraphDataContainer::const_iterator &closestData) const;
    };
}
Q_DECLARE_METATYPE(QCP::QCPGraph::LineStyle)

#endif
