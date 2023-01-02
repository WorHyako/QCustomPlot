#include "include/QCPPaintBufferGlFbo.hpp"

namespace QCP {

#ifdef QCP_OPENGL_FBO
    ////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPaintBufferGlFbo
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPaintBufferGlFbo
  \brief A paint buffer based on OpenGL frame buffers objects, using hardware accelerated rendering

  This paint buffer is one of the OpenGL paint buffers which facilitate hardware accelerated plot
  rendering. It is based on OpenGL frame buffer objects (fbo) and is used in Qt versions 5.0 and
  higher. (See \ref QCPPaintBufferGlPbuffer used in older Qt versions.)

  The OpenGL paint buffers are used if \ref QCustomPlot::setOpenGl is set to true, and if they are
  supported by the system.
*/

/*!
  Creates a \ref QCPPaintBufferGlFbo instance with the specified \a size and \a devicePixelRatio,
  if applicable.

  All frame buffer objects shall share one OpenGL context and paint device, which need to be set up
  externally and passed via \a glContext and \a glPaintDevice. The set-up is done in \ref
  QCustomPlot::setupOpenGl and the context and paint device are managed by the parent QCustomPlot
  instance.
*/
QCPPaintBufferGlFbo::QCPPaintBufferGlFbo(const QSize &size, double devicePixelRatio, QWeakPointer<QOpenGLContext> glContext, QWeakPointer<QOpenGLPaintDevice> glPaintDevice) :
QCPAbstractPaintBuffer(size, devicePixelRatio),
mGlContext(glContext),
mGlPaintDevice(glPaintDevice),
mGlFrameBuffer(0)
{
QCPPaintBufferGlFbo::reallocateBuffer();
}

QCPPaintBufferGlFbo::~QCPPaintBufferGlFbo()
{
if (mGlFrameBuffer)
delete mGlFrameBuffer;
}

/* inherits documentation from base class */
QCPPainter *QCPPaintBufferGlFbo::startPainting()
{
QSharedPointer<QOpenGLPaintDevice> paintDevice = mGlPaintDevice.toStrongRef();
QSharedPointer<QOpenGLContext> context = mGlContext.toStrongRef();
if (!paintDevice)
{
qDebug() << Q_FUNC_INFO << "OpenGL paint device doesn't exist";
return 0;
}
if (!context)
{
qDebug() << Q_FUNC_INFO << "OpenGL context doesn't exist";
return 0;
}
if (!mGlFrameBuffer)
{
qDebug() << Q_FUNC_INFO << "OpenGL frame buffer object doesn't exist, reallocateBuffer was not called?";
return 0;
}

if (QOpenGLContext::currentContext() != context.data())
context->makeCurrent(context->surface());
mGlFrameBuffer->bind();
QCPPainter *result = new QCPPainter(paintDevice.data());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
result->setRenderHint(QPainter::HighQualityAntialiasing);
#endif
return result;
}

/* inherits documentation from base class */
void QCPPaintBufferGlFbo::donePainting()
{
if (mGlFrameBuffer && mGlFrameBuffer->isBound())
mGlFrameBuffer->release();
else
qDebug() << Q_FUNC_INFO << "Either OpenGL frame buffer not valid or was not bound";
}

/* inherits documentation from base class */
void QCPPaintBufferGlFbo::draw(QCPPainter *painter) const
{
if (!painter || !painter->isActive())
{
qDebug() << Q_FUNC_INFO << "invalid or inactive painter passed";
return;
}
if (!mGlFrameBuffer)
{
qDebug() << Q_FUNC_INFO << "OpenGL frame buffer object doesn't exist, reallocateBuffer was not called?";
return;
}
painter->drawImage(0, 0, mGlFrameBuffer->toImage());
}

/* inherits documentation from base class */
void QCPPaintBufferGlFbo::clear(const QColor &color)
{
QSharedPointer<QOpenGLContext> context = mGlContext.toStrongRef();
if (!context)
{
qDebug() << Q_FUNC_INFO << "OpenGL context doesn't exist";
return;
}
if (!mGlFrameBuffer)
{
qDebug() << Q_FUNC_INFO << "OpenGL frame buffer object doesn't exist, reallocateBuffer was not called?";
return;
}

if (QOpenGLContext::currentContext() != context.data())
context->makeCurrent(context->surface());
mGlFrameBuffer->bind();
glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
mGlFrameBuffer->release();
}

/* inherits documentation from base class */
void QCPPaintBufferGlFbo::reallocateBuffer()
{
// release and delete possibly existing framebuffer:
if (mGlFrameBuffer)
{
if (mGlFrameBuffer->isBound())
mGlFrameBuffer->release();
delete mGlFrameBuffer;
mGlFrameBuffer = 0;
}

QSharedPointer<QOpenGLPaintDevice> paintDevice = mGlPaintDevice.toStrongRef();
QSharedPointer<QOpenGLContext> context = mGlContext.toStrongRef();
if (!paintDevice)
{
qDebug() << Q_FUNC_INFO << "OpenGL paint device doesn't exist";
return;
}
if (!context)
{
qDebug() << Q_FUNC_INFO << "OpenGL context doesn't exist";
return;
}

// create new fbo with appropriate size:
context->makeCurrent(context->surface());
QOpenGLFramebufferObjectFormat frameBufferFormat;
frameBufferFormat.setSamples(context->format().samples());
frameBufferFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
mGlFrameBuffer = new QOpenGLFramebufferObject(mSize*mDevicePixelRatio, frameBufferFormat);
if (paintDevice->size() != mSize*mDevicePixelRatio)
paintDevice->setSize(mSize*mDevicePixelRatio);
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
paintDevice->setDevicePixelRatio(mDevicePixelRatio);
#endif
}
#endif // QCP_OPENGL_FBO
/* end of 'src/paintbuffer.cpp' */

} // QCP