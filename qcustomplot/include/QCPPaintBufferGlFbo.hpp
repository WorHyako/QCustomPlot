#ifndef QCUSTOMPLOT_QCPPAINTBUFFERGLFBO_H
#define QCUSTOMPLOT_QCPPAINTBUFFERGLFBO_H

namespace QCP {

#ifdef QCP_OPENGL_FBO
    class QCP_LIB_DECL QCPPaintBufferGlFbo : public QCPAbstractPaintBuffer
{
public:
  explicit QCPPaintBufferGlFbo(const QSize &size, double devicePixelRatio, QWeakPointer<QOpenGLContext> glContext, QWeakPointer<QOpenGLPaintDevice> glPaintDevice);
  virtual ~QCPPaintBufferGlFbo() Q_DECL_OVERRIDE;

  // reimplemented virtual methods:
  virtual QCPPainter *startPainting() Q_DECL_OVERRIDE;
  virtual void donePainting() Q_DECL_OVERRIDE;
  virtual void draw(QCPPainter *painter) const Q_DECL_OVERRIDE;
  void clear(const QColor &color) Q_DECL_OVERRIDE;

protected:
  // non-property members:
  QWeakPointer<QOpenGLContext> mGlContext;
  QWeakPointer<QOpenGLPaintDevice> mGlPaintDevice;
  QOpenGLFramebufferObject *mGlFrameBuffer;

  // reimplemented virtual methods:
  virtual void reallocateBuffer() Q_DECL_OVERRIDE;
};
#endif // QCP_OPENGL_FBO


} // QCP

#endif //QCUSTOMPLOT_QCPPAINTBUFFERGLFBO_H
