#ifndef QCUSTOMPLOT_QCPPAINTBUFFERGLPBUFFER_HPP
#define QCUSTOMPLOT_QCPPAINTBUFFERGLPBUFFER_HPP

#include "QCPAbstractPaintBuffer.hpp"

#include <QOpenGLContext>

namespace QCP {
//#ifdef QCP_OPENGL_PBUFFER
    class QCP_LIB_DECL QCPPaintBufferGlPbuffer : public QCPAbstractPaintBuffer {
    public:
        explicit QCPPaintBufferGlPbuffer(const QSize &size, double devicePixelRatio, int multisamples);

        ~QCPPaintBufferGlPbuffer() override;

        QCPPainter *startPainting() override;

        void draw(QCPPainter *painter) const override;

        void clear(const QColor &color) override;

    protected:
        QGLPixelBuffer *mGlPBuffer;
        int mMultisamples;

        void reallocateBuffer() override;
    };
//#endif
}
#endif
