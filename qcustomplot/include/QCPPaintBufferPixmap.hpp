#ifndef QCUSTOMPLOT_QCPPAINTBUFFERPIXMAP_HPP
#define QCUSTOMPLOT_QCPPAINTBUFFERPIXMAP_HPP

#include "QCPAbstractPaintBuffer.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPPaintBufferPixmap : public QCPAbstractPaintBuffer {
    public:
        explicit QCPPaintBufferPixmap(const QSize &size, double devicePixelRatio);

        ~QCPPaintBufferPixmap() override;

        QCPPainter *startPainting() override;

        void draw(QCPPainter *painter) const override;

        void clear(const QColor &color) override;

    protected:
        QPixmap mBuffer;

        void reallocateBuffer() override;
    };
}
#endif
