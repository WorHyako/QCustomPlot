#include "include/QCPAbstractPaintBuffer.hpp"

namespace QCP {
/*! \class QCPAbstractPaintBuffer
  \brief The abstract base class for paint buffers, which define the rendering backend

  This abstract base class defines the basic interface that a paint buffer needs to provide in
  order to be usable by QCustomPlot.

  A paint buffer manages both a surface to draw onto, and the matching paint device. The size of
  the surface can be changed via \ref setSize. External classes (\ref QCustomPlot and \ref
  QCPLayer) request a painter via \ref startPainting and then perform the draw calls. Once the
  painting is complete, \ref donePainting is called, so the paint buffer implementation can do
  clean up if necessary. Before rendering a frame, each paint buffer is usually filled with a color
  using \ref clear (usually the color is \c Qt::transparent), to remove the contents of the
  previous frame.

  The simplest paint buffer implementation is \ref QCPPaintBufferPixmap which allows regular
  software rendering via the raster engine. Hardware accelerated rendering via pixel buffers and
  frame buffer objects is provided by \ref QCPPaintBufferGlPbuffer and \ref QCPPaintBufferGlFbo.
  They are used automatically if \ref QCustomPlot::setOpenGl is enabled.
*/

/* start documentation of pure virtual functions */

/*! \fn virtual QCPPainter *QCPAbstractPaintBuffer::startPainting() = 0

  Returns a \ref QCPPainter which is ready to draw to this buffer. The ownership and thus the
  responsibility to delete the painter after the painting operations are complete is given to the
  caller of this method.

  Once you are done using the painter, delete the painter and call \ref donePainting.

  While a painter generated with this method is active, you must not call \ref setSize, \ref
  setDevicePixelRatio or \ref clear.

  This method may return 0, if a painter couldn't be activated on the buffer. This usually
  indicates a problem with the respective painting backend.
*/

/*! \fn virtual void QCPAbstractPaintBuffer::draw(QCPPainter *painter) const = 0

  Draws the contents of this buffer with the provided \a painter. This is the method that is used
  to finally join all paint buffers and draw them onto the screen.
*/

/*! \fn virtual void QCPAbstractPaintBuffer::clear(const QColor &color) = 0

  Fills the entire buffer with the provided \a color. To have an empty transparent buffer, use the
  named color \c Qt::transparent.

  This method must not be called if there is currently a painter (acquired with \ref startPainting)
  active.
*/

/*! \fn virtual void QCPAbstractPaintBuffer::reallocateBuffer() = 0

  Reallocates the internal buffer with the currently configured size (\ref setSize) and device
  pixel ratio, if applicable (\ref setDevicePixelRatio). It is called as soon as any of those
  properties are changed on this paint buffer.

  \note Subclasses of \ref QCPAbstractPaintBuffer must call their reimplementation of this method
  in their constructor, to perform the first allocation (this can not be done by the base class
  because calling pure virtual methods in base class constructors is not possible).
*/

/* end documentation of pure virtual functions */
/* start documentation of inline functions */

/*! \fn virtual void QCPAbstractPaintBuffer::donePainting()

  If you have acquired a \ref QCPPainter to paint onto this paint buffer via \ref startPainting,
  call this method as soon as you are done with the painting operations and have deleted the
  painter.

  paint buffer subclasses may use this method to perform any type of cleanup that is necessary. The
  default implementation does nothing.
*/

/* end documentation of inline functions */

/*!
  Creates a paint buffer and initializes it with the provided \a size and \a devicePixelRatio.

  Subclasses must call their \ref reallocateBuffer implementation in their respective constructors.
*/
    QCPAbstractPaintBuffer::QCPAbstractPaintBuffer(const QSize &size, double devicePixelRatio) :
            mSize(size),
            mDevicePixelRatio(devicePixelRatio),
            mInvalidated(true)
    {
    }

    QCPAbstractPaintBuffer::~QCPAbstractPaintBuffer()
    {
    }

/*!
  Sets the paint buffer size.

  The buffer is reallocated (by calling \ref reallocateBuffer), so any painters that were obtained
  by \ref startPainting are invalidated and must not be used after calling this method.

  If \a size is already the current buffer size, this method does nothing.
*/
    void QCPAbstractPaintBuffer::setSize(const QSize &size)
    {
        if (mSize != size)
        {
            mSize = size;
            reallocateBuffer();
        }
    }

/*!
  Sets the invalidated flag to \a invalidated.

  This mechanism is used internally in conjunction with isolated replotting of \ref QCPLayer
  instances (in \ref QCPLayer::lmBuffered mode). If \ref QCPLayer::replot is called on a buffered
  layer, i.e. an isolated repaint of only that layer (and its dedicated paint buffer) is requested,
  QCustomPlot will decide depending on the invalidated flags of other paint buffers whether it also
  replots them, instead of only the layer on which the replot was called.

  The invalidated flag is set to true when \ref QCPLayer association has changed, i.e. if layers
  were added or removed from this buffer, or if they were reordered. It is set to false as soon as
  all associated \ref QCPLayer instances are drawn onto the buffer.

  Under normal circumstances, it is not necessary to manually call this method.
*/
    void QCPAbstractPaintBuffer::setInvalidated(bool invalidated)
    {
        mInvalidated = invalidated;
    }

/*!
  Sets the device pixel ratio to \a ratio. This is useful to render on high-DPI output devices.
  The ratio is automatically set to the device pixel ratio used by the parent QCustomPlot instance.

  The buffer is reallocated (by calling \ref reallocateBuffer), so any painters that were obtained
  by \ref startPainting are invalidated and must not be used after calling this method.

  \note This method is only available for Qt versions 5.4 and higher.
*/
    void QCPAbstractPaintBuffer::setDevicePixelRatio(double ratio)
    {
        if (!qFuzzyCompare(ratio, mDevicePixelRatio))
        {
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
            mDevicePixelRatio = ratio;
reallocateBuffer();
#else
            qDebug() << Q_FUNC_INFO << "Device pixel ratios not supported for Qt versions before 5.4";
            mDevicePixelRatio = 1.0;
#endif
        }
    }
} // QCP