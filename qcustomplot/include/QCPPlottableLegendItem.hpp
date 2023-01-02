#ifndef QCUSTOMPLOT_QCPPLOTTABLELEGENDITEM_H
#define QCUSTOMPLOT_QCPPLOTTABLELEGENDITEM_H

namespace QCP {


    class QCP_LIB_DECL QCPPlottableLegendItem : public QCPAbstractLegendItem
{
    Q_OBJECT
    public:
    QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable);

    // getters:
    QCPAbstractPlottable *plottable() { return mPlottable; }

    protected:
    // property members:
    QCPAbstractPlottable *mPlottable;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    virtual QSize minimumOuterSizeHint() const Q_DECL_OVERRIDE;

    // non-virtual methods:
    QPen getIconBorderPen() const;
    QColor getTextColor() const;
    QFont getFont() const;
};


} // QCP

#endif //QCUSTOMPLOT_QCPPLOTTABLELEGENDITEM_H
