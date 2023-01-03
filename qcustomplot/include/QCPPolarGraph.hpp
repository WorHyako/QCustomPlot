#ifndef QCUSTOMPLOT_QCPPOLARGRAPH_HPP
#define QCUSTOMPLOT_QCPPOLARGRAPH_HPP

#include "QCPLayerable.hpp"
#include "QCPPolarAxisAngular.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPPolarGraph : public QCPLayerable {
    Q_OBJECT
    public:
        /*!
        Defines how the graph's line is represented visually in the plot. The line is drawn with the
        current pen of the graph (\ref setPen).
        \see setLineStyle
      */
        enum LineStyle {
            lsNone,     ///< data points are not connected with any lines (e.g. data only represented
            ///< with symbols according to the scatter style, see \ref setScatterStyle)
            lsLine       ///< data points are connected by a straight line
        };

        Q_ENUM(LineStyle)

        Q_DISABLE_COPY(QCPPolarGraph)

        QCPPolarGraph(QCPPolarAxisAngular *keyAxis, QCPPolarAxisRadial *valueAxis);

        virtual ~QCPPolarGraph();

        QString name() const { return mName; }

        bool antialiasedFill() const { return mAntialiasedFill; }

        bool antialiasedScatters() const { return mAntialiasedScatters; }

        QPen pen() const { return mPen; }

        QBrush brush() const { return mBrush; }

        bool periodic() const { return mPeriodic; }

        QCPPolarAxisAngular *keyAxis() const { return mKeyAxis.data(); }

        QCPPolarAxisRadial *valueAxis() const { return mValueAxis.data(); }

        QCP::SelectionType selectable() const { return mSelectable; }

        bool selected() const { return !mSelection.isEmpty(); }

        QCPDataSelection selection() const { return mSelection; }

        //QCPSelectionDecorator *selectionDecorator() const { return mSelectionDecorator; }
        QSharedPointer<QCPGraphDataContainer> data() const { return mDataContainer; }

        LineStyle lineStyle() const { return mLineStyle; }

        QCPScatterStyle scatterStyle() const { return mScatterStyle; }

        void setName(const QString &name);

        void setAntialiasedFill(bool enabled);

        void setAntialiasedScatters(bool enabled);

        void setPen(const QPen &pen);

        void setBrush(const QBrush &brush);

        void setPeriodic(bool enabled);

        void setKeyAxis(QCPPolarAxisAngular *axis);

        void setValueAxis(QCPPolarAxisRadial *axis);

        Q_SLOT void setSelectable(QCP::SelectionType selectable);

        Q_SLOT void setSelection(QCPDataSelection selection);

        //void setSelectionDecorator(QCPSelectionDecorator *decorator);
        void setData(QSharedPointer<QCPGraphDataContainer> data);

        void setData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted = false);

        void setLineStyle(LineStyle ls);

        void setScatterStyle(const QCPScatterStyle &style);

        void addData(const QVector<double> &keys, const QVector<double> &values, bool alreadySorted = false);

        void addData(double key, double value);

        void coordsToPixels(double key, double value, double &x, double &y) const;

        const QPointF coordsToPixels(double key, double value) const;

        void pixelsToCoords(double x, double y, double &key, double &value) const;

        void pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const;

        void rescaleAxes(bool onlyEnlarge = false) const;

        void rescaleKeyAxis(bool onlyEnlarge = false) const;

        void rescaleValueAxis(bool onlyEnlarge = false, bool inKeyRange = false) const;

        bool addToLegend(QCPLegend *legend);

        bool addToLegend();

        bool removeFromLegend(QCPLegend *legend) const;

        bool removeFromLegend() const;

        virtual double selectTest(const QPointF &pos, bool onlySelectable,
                                  QVariant *details = 0) const; // actually introduced in QCPLayerable as non-pure, but we want to force reimplementation for plottables
        virtual QCPPlottableInterface1D *
        interface1D() { return 0; } // TODO: return this later, when QCPAbstractPolarPlottable is created
        virtual QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth) const;

        virtual QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth,
                                       const QCPRange &inKeyRange = QCPRange()) const;

    signals:

        void selectionChanged(bool selected);

        void selectionChanged(const QCPDataSelection &selection);

        void selectableChanged(QCP::SelectionType selectable);

    protected:
        QSharedPointer<QCPGraphDataContainer> mDataContainer;
        LineStyle mLineStyle;
        QCPScatterStyle mScatterStyle;
        QString mName;
        bool mAntialiasedFill, mAntialiasedScatters;
        QPen mPen;
        QBrush mBrush;
        bool mPeriodic;
        QPointer<QCPPolarAxisAngular> mKeyAxis;
        QPointer<QCPPolarAxisRadial> mValueAxis;
        QCP::SelectionType mSelectable;
        QCPDataSelection mSelection;
        //QCPSelectionDecorator *mSelectionDecorator;

        // introduced virtual methods (later reimplemented TODO from QCPAbstractPolarPlottable):
        virtual QRect clipRect() const;

        virtual void draw(QCPPainter *painter);

        virtual QCP::Interaction selectionCategory() const;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const;

        virtual void
        selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);

        virtual void deselectEvent(bool *selectionStateChanged);

        virtual void drawLinePlot(QCPPainter *painter, const QVector<QPointF> &lines) const;

        virtual void drawFill(QCPPainter *painter, QVector<QPointF> *lines) const;

        virtual void
        drawScatterPlot(QCPPainter *painter, const QVector<QPointF> &scatters, const QCPScatterStyle &style) const;

        virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;

        void applyFillAntialiasingHint(QCPPainter *painter) const;

        void applyScattersAntialiasingHint(QCPPainter *painter) const;

        double pointDistance(const QPointF &pixelPoint, QCPGraphDataContainer::const_iterator &closestData) const;

        virtual int dataCount() const;

        void getDataSegments(QList<QCPDataRange> &selectedSegments, QList<QCPDataRange> &unselectedSegments) const;

        void drawPolyline(QCPPainter *painter, const QVector<QPointF> &lineData) const;

        void
        getVisibleDataBounds(QCPGraphDataContainer::const_iterator &begin, QCPGraphDataContainer::const_iterator &end,
                             const QCPDataRange &rangeRestriction) const;

        void getLines(QVector<QPointF> *lines, const QCPDataRange &dataRange) const;

        void getScatters(QVector<QPointF> *scatters, const QCPDataRange &dataRange) const;

        void getOptimizedLineData(QVector<QCPGraphData> *lineData, const QCPGraphDataContainer::const_iterator &begin,
                                  const QCPGraphDataContainer::const_iterator &end) const;

        void getOptimizedScatterData(QVector<QCPGraphData> *scatterData, QCPGraphDataContainer::const_iterator begin,
                                     QCPGraphDataContainer::const_iterator end) const;

        QVector<QPointF> dataToLines(const QVector<QCPGraphData> &data) const;
    };
}
#endif
