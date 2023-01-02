#ifndef QCUSTOMPLOT_QCPPAINTBUFFERGLPBUFFER_H
#define QCUSTOMPLOT_QCPPAINTBUFFERGLPBUFFER_H

namespace QCP {
#ifdef QCP_OPENGL_PBUFFER
    class QCP_LIB_DECL QCPPaintBufferGlPbuffer : public QCPAbstractPaintBuffer
{
    public:
    explicit QCPPaintBufferGlPbuffer(const QSize &size, double devicePixelRatio, int multisamples);
    virtual ~QCPPaintBufferGlPbuffer() Q_DECL_OVERRIDE;

    // reimplemented virtual methods:
    virtual QCPPainter *startPainting() Q_DECL_OVERRIDE;
    virtual void draw(QCPPainter *painter) const Q_DECL_OVERRIDE;
    void clear(const QColor &color) Q_DECL_OVERRIDE;

    protected:
    // non-property members:
    QGLPixelBuffer *mGlPBuffer;
    int mMultisamples;

    // reimplemented virtual methods:
    virtual void reallocateBuffer() Q_DECL_OVERRIDE;
};
#endif // QCP_OPENGL_PBUFFER

}

#endif //QCUSTOMPLOT_QCPPAINTBUFFERGLPBUFFER_H
