#ifndef QCUSTOMPLOT_QCPPAINTBUFFERGLFBO_HPP
#define QCUSTOMPLOT_QCPPAINTBUFFERGLFBO_HPP

#include "QCPAbstractPaintBuffer.hpp"

#include <QtOpenGL/QOpenGLFramebufferObject>

namespace QCP {

#ifdef QCP_OPENGL_FBO
    class QCP_LIB_DECL QCPPaintBufferGlFbo : public QCPAbstractPaintBuffer {
    public:
        explicit QCPPaintBufferGlFbo(const QSize &size, double devicePixelRatio, QWeakPointer<QOpenGLContext> glContext,
                                     QWeakPointer<QOpenGLPaintDevice> glPaintDevice);

        ~QCPPaintBufferGlFbo() override;

        QCPPainter *startPainting() override;

        void donePainting() override;

        void draw(QCPPainter *painter) const override;

        void clear(const QColor &color) override;

    protected:
        QWeakPointer<QOpenGLContext> mGlContext;
        QWeakPointer<QOpenGLPaintDevice> mGlPaintDevice;
        QOpenGLFramebufferObject *mGlFrameBuffer;

        void reallocateBuffer() override;
    };
#endif
}
#endif
