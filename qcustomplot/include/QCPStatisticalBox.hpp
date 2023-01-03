#ifndef QCUSTOMPLOT_QCPSTATISTICALBOX_HPP
#define QCUSTOMPLOT_QCPSTATISTICALBOX_HPP

#include "QCPAbstractPlottable1D.hpp"
#include "QCPStatisticalBoxData.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPStatisticalBox : public QCPAbstractPlottable1D<QCPStatisticalBoxData> {
    Q_OBJECT
        Q_PROPERTY(double width READ width WRITE setWidth)
        Q_PROPERTY(double whiskerWidth READ whiskerWidth WRITE setWhiskerWidth)
        Q_PROPERTY(QPen whiskerPen READ whiskerPen WRITE setWhiskerPen)
        Q_PROPERTY(QPen whiskerBarPen READ whiskerBarPen WRITE setWhiskerBarPen)
        Q_PROPERTY(bool whiskerAntialiased READ whiskerAntialiased WRITE setWhiskerAntialiased)
        Q_PROPERTY(QPen medianPen READ medianPen WRITE setMedianPen)
        Q_PROPERTY(QCPScatterStyle outlierStyle READ outlierStyle WRITE setOutlierStyle)
    public:
        explicit QCPStatisticalBox(QCPAxis *keyAxis, QCPAxis *valueAxis);

        QSharedPointer<QCPStatisticalBoxDataContainer> data() const { return mDataContainer; }

        double width() const { return mWidth; }

        double whiskerWidth() const { return mWhiskerWidth; }

        QPen whiskerPen() const { return mWhiskerPen; }

        QPen whiskerBarPen() const { return mWhiskerBarPen; }

        bool whiskerAntialiased() const { return mWhiskerAntialiased; }

        QPen medianPen() const { return mMedianPen; }

        QCPScatterStyle outlierStyle() const { return mOutlierStyle; }

        void setData(QSharedPointer<QCPStatisticalBoxDataContainer> data);

        void setData(const QVector<double> &keys, const QVector<double> &minimum, const QVector<double> &lowerQuartile,
                     const QVector<double> &median, const QVector<double> &upperQuartile,
                     const QVector<double> &maximum, bool alreadySorted = false);

        void setWidth(double width);

        void setWhiskerWidth(double width);

        void setWhiskerPen(const QPen &pen);

        void setWhiskerBarPen(const QPen &pen);

        void setWhiskerAntialiased(bool enabled);

        void setMedianPen(const QPen &pen);

        void setOutlierStyle(const QCPScatterStyle &style);

        void addData(const QVector<double> &keys, const QVector<double> &minimum, const QVector<double> &lowerQuartile,
                     const QVector<double> &median, const QVector<double> &upperQuartile,
                     const QVector<double> &maximum, bool alreadySorted = false);

        void
        addData(double key, double minimum, double lowerQuartile, double median, double upperQuartile, double maximum,
                const QVector<double> &outliers = QVector<double>());

        QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const override;

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPRange getKeyRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth) const override;

        QCPRange getValueRange(bool &foundRange, QCP::SignDomain inSignDomain = QCP::sdBoth,
                               const QCPRange &inKeyRange = QCPRange()) const override;

    protected:
        double mWidth;
        double mWhiskerWidth;
        QPen mWhiskerPen, mWhiskerBarPen;
        bool mWhiskerAntialiased;
        QPen mMedianPen;
        QCPScatterStyle mOutlierStyle;

        void draw(QCPPainter *painter) override;

        void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const override;

        virtual void drawStatisticalBox(QCPPainter *painter, QCPStatisticalBoxDataContainer::const_iterator it,
                                        const QCPScatterStyle &outlierStyle) const;

        void getVisibleDataBounds(QCPStatisticalBoxDataContainer::const_iterator &begin,
                                  QCPStatisticalBoxDataContainer::const_iterator &end) const;

        QRectF getQuartileBox(QCPStatisticalBoxDataContainer::const_iterator it) const;

        QVector<QLineF> getWhiskerBackboneLines(QCPStatisticalBoxDataContainer::const_iterator it) const;

        QVector<QLineF> getWhiskerBarLines(QCPStatisticalBoxDataContainer::const_iterator it) const;
    };
}
#endif
