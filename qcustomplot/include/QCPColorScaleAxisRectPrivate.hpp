#ifndef QCUSTOMPLOT_QCPCOLORSCALEAXISRECTPRIVATE_H
#define QCUSTOMPLOT_QCPCOLORSCALEAXISRECTPRIVATE_H

namespace QCP {

    class QCPColorScaleAxisRectPrivate : public QCPAxisRect
    {
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
        virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
        void updateGradientImage();
        Q_SLOT void axisSelectionChanged(QCPAxis::SelectableParts selectedParts);
        Q_SLOT void axisSelectableChanged(QCPAxis::SelectableParts selectableParts);
        friend class QCPColorScale;
    };


} // QCP

#endif //QCUSTOMPLOT_QCPCOLORSCALEAXISRECTPRIVATE_H
