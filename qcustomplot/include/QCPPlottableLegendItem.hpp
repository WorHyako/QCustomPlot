#ifndef QCUSTOMPLOT_QCPPLOTTABLELEGENDITEM_HPP
#define QCUSTOMPLOT_QCPPLOTTABLELEGENDITEM_HPP

#include "QCPAbstractLegendItem.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPPlottableLegendItem : public QCPAbstractLegendItem {
    Q_OBJECT
    public:
        QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable);

        QCPAbstractPlottable *plottable() { return mPlottable; }

    protected:
        QCPAbstractPlottable *mPlottable;

        void draw(QCPPainter *painter) override;

        QSize minimumOuterSizeHint() const override;

        QPen getIconBorderPen() const;

        QColor getTextColor() const;

        QFont getFont() const;
    };
}
#endif
