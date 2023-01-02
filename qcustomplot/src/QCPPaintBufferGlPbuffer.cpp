//
// Created by worhy on 31.12.2022.
//

#include "include/QCPPaintBufferGlPbuffer.hpp"

using namespace QCP;


#ifdef QCP_OPENGL_PBUFFER
/*! \class QCPPaintBufferGlPbuffer
  \brief A paint buffer based on OpenGL pixel buffers, using hardware accelerated rendering

  This paint buffer is one of the OpenGL paint buffers which facilitate hardware accelerated plot
  rendering. It is based on OpenGL pixel buffers (pbuffer) and is used in Qt versions before 5.0.
  (See \ref QCPPaintBufferGlFbo used in newer Qt versions.)

  The OpenGL paint buffers are used if \ref QCustomPlot::setOpenGl is set to true, and if they are
  supported by the system.
*/

/*!
  Creates a \ref QCPPaintBufferGlPbuffer instance with the specified \a size and \a
  devicePixelRatio, if applicable.

  The parameter \a multisamples defines how many samples are used per pixel. Higher values thus
  result in higher quality antialiasing. If the specified \a multisamples value exceeds the
  capability of the graphics hardware, the highest supported multisampling is used.
*/
QCPPaintBufferGlPbuffer::QCPPaintBufferGlPbuffer(const QSize &size, double devicePixelRatio, int multisamples) :
QCPAbstractPaintBuffer(size, devicePixelRatio),
mGlPBuffer(0),
mMultisamples(qMax(0, multisamples))
{
QCPPaintBufferGlPbuffer::reallocateBuffer();
}

QCPPaintBufferGlPbuffer::~QCPPaintBufferGlPbuffer()
{
if (mGlPBuffer)
delete mGlPBuffer;
}

/* inherits documentation from base class */
QCPPainter *QCPPaintBufferGlPbuffer::startPainting()
{
if (!mGlPBuffer->isValid())
{
qDebug() << Q_FUNC_INFO << "OpenGL frame buffer object doesn't exist, reallocateBuffer was not called?";
return 0;
}

QCPPainter *result = new QCPPainter(mGlPBuffer);
result->setRenderHint(QPainter::HighQualityAntialiasing);
return result;
}

/* inherits documentation from base class */
void QCPPaintBufferGlPbuffer::draw(QCPPainter *painter) const
{
if (!painter || !painter->isActive())
{
qDebug() << Q_FUNC_INFO << "invalid or inactive painter passed";
return;
}
if (!mGlPBuffer->isValid())
{
qDebug() << Q_FUNC_INFO << "OpenGL pbuffer isn't valid, reallocateBuffer was not called?";
return;
}
painter->drawImage(0, 0, mGlPBuffer->toImage());
}

/* inherits documentation from base class */
void QCPPaintBufferGlPbuffer::clear(const QColor &color)
{
if (mGlPBuffer->isValid())
{
mGlPBuffer->makeCurrent();
glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
mGlPBuffer->doneCurrent();
} else
qDebug() << Q_FUNC_INFO << "OpenGL pbuffer invalid or context not current";
}

/* inherits documentation from base class */
void QCPPaintBufferGlPbuffer::reallocateBuffer()
{
if (mGlPBuffer)
delete mGlPBuffer;

QGLFormat format;
format.setAlpha(true);
format.setSamples(mMultisamples);
mGlPBuffer = new QGLPixelBuffer(mSize, format);
}
#endif // QCP_OPENGL_PBUFFER
