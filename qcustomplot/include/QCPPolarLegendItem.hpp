#ifndef QCUSTOMPLOT_QCPPOLARLEGENDITEM_HPP
#define QCUSTOMPLOT_QCPPOLARLEGENDITEM_HPP

#include "QCPAbstractLegendItem.hpp"
#include "QCPPolarGraph.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPPolarLegendItem : public QCPAbstractLegendItem {
    Q_OBJECT
    public:
        QCPPolarLegendItem(QCPLegend *parent, QCPPolarGraph *graph);

        QCPPolarGraph *polarGraph() { return mPolarGraph; }

    protected:
        QCPPolarGraph *mPolarGraph;

        void draw(QCPPainter *painter) override;

        QSize minimumOuterSizeHint() const override;

        QPen getIconBorderPen() const;

        QColor getTextColor() const;

        QFont getFont() const;
    };
}
#endif
