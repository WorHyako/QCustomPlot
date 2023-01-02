#ifndef QCUSTOMPLOT_QCPPLOTTABLEINTERFACE1D_H
#define QCUSTOMPLOT_QCPPLOTTABLEINTERFACE1D_H

namespace QCP {

    class QCPPlottableInterface1D
    {
    public:
        virtual ~QCPPlottableInterface1D() = default;
        // introduced pure virtual methods:
        virtual int dataCount() const = 0;
        virtual double dataMainKey(int index) const = 0;
        virtual double dataSortKey(int index) const = 0;
        virtual double dataMainValue(int index) const = 0;
        virtual QCPRange dataValueRange(int index) const = 0;
        virtual QPointF dataPixelPosition(int index) const = 0;
        virtual bool sortKeyIsMainKey() const = 0;
        virtual QCPDataSelection selectTestRect(const QRectF &rect, bool onlySelectable) const = 0;
        virtual int findBegin(double sortKey, bool expandedRange=true) const = 0;
        virtual int findEnd(double sortKey, bool expandedRange=true) const = 0;
    };


} // QCP

#endif //QCUSTOMPLOT_QCPPLOTTABLEINTERFACE1D_H
