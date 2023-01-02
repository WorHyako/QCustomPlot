#ifndef QCUSTOMPLOT_QCPPOLARLEGENDITEM_H
#define QCUSTOMPLOT_QCPPOLARLEGENDITEM_H

namespace QCP {

    class QCP_LIB_DECL QCPPolarLegendItem : public QCPAbstractLegendItem
{
    Q_OBJECT
    public:
    QCPPolarLegendItem(QCPLegend *parent, QCPPolarGraph *graph);

    // getters:
    QCPPolarGraph *polarGraph() { return mPolarGraph; }

    protected:
    // property members:
    QCPPolarGraph *mPolarGraph;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    virtual QSize minimumOuterSizeHint() const Q_DECL_OVERRIDE;

    // non-virtual methods:
    QPen getIconBorderPen() const;
    QColor getTextColor() const;
    QFont getFont() const;
};


} // QCP

#endif //QCUSTOMPLOT_QCPPOLARLEGENDITEM_H
