#ifndef QCUSTOMPLOT_QCPCOLORSCALEAXISRECTPRIVATE_HPP
#define QCUSTOMPLOT_QCPCOLORSCALEAXISRECTPRIVATE_HPP

#include "QCPAxisRect.hpp"
#include "QCPColorScale.hpp"

namespace QCP {

    class QCPColorScaleAxisRectPrivate : public QCPAxisRect {
    Q_OBJECT
    public:
        explicit QCPColorScaleAxisRectPrivate(QCPColorScale *parentColorScale);

    protected:
        QCPColorScale *mParentColorScale;
        QImage mGradientImage;
        bool mGradientImageInvalidated;
        // re-using some methods of QCPAxisRect to make them available to friend class QCPColorScale
        using QCPAxisRect::calculateAutoMargin;
        using QCPAxisRect::mousePressEvent;
        using QCPAxisRect::mouseMoveEvent;
        using QCPAxisRect::mouseReleaseEvent;
        using QCPAxisRect::wheelEvent;
        using QCPAxisRect::update;

        void draw(QCPPainter *painter) override;

        void updateGradientImage();

        Q_SLOT void axisSelectionChanged(QCPAxis::SelectableParts selectedParts);

        Q_SLOT void axisSelectableChanged(QCPAxis::SelectableParts selectableParts);
    };
}
#endif
