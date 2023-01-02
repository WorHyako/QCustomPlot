#include "include/QCPPaintBufferPixmap.hpp"

namespace QCP {
/*! \class QCPPaintBufferPixmap
  \brief A paint buffer based on QPixmap, using software raster rendering

  This paint buffer is the default and fall-back paint buffer which uses software rendering and
  QPixmap as internal buffer. It is used if \ref QCustomPlot::setOpenGl is false.
*/

/*!
  Creates a pixmap paint buffer instancen with the specified \a size and \a devicePixelRatio, if
  applicable.
*/
    QCPPaintBufferPixmap::QCPPaintBufferPixmap(const QSize &size, double devicePixelRatio) :
            QCPAbstractPaintBuffer(size, devicePixelRatio)
    {
        QCPPaintBufferPixmap::reallocateBuffer();
    }

    QCPPaintBufferPixmap::~QCPPaintBufferPixmap()
    {
    }

/* inherits documentation from base class */
    QCPPainter *QCPPaintBufferPixmap::startPainting()
    {
        QCPPainter *result = new QCPPainter(&mBuffer);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        result->setRenderHint(QPainter::HighQualityAntialiasing);
#endif
        return result;
    }

/* inherits documentation from base class */
    void QCPPaintBufferPixmap::draw(QCPPainter *painter) const
    {
        if (painter && painter->isActive())
            painter->drawPixmap(0, 0, mBuffer);
        else
            qDebug() << Q_FUNC_INFO << "invalid or inactive painter passed";
    }

/* inherits documentation from base class */
    void QCPPaintBufferPixmap::clear(const QColor &color)
    {
        mBuffer.fill(color);
    }

/* inherits documentation from base class */
    void QCPPaintBufferPixmap::reallocateBuffer()
    {
        setInvalidated();
        if (!qFuzzyCompare(1.0, mDevicePixelRatio))
        {
#ifdef QCP_DEVICEPIXELRATIO_SUPPORTED
            mBuffer = QPixmap(mSize*mDevicePixelRatio);
mBuffer.setDevicePixelRatio(mDevicePixelRatio);
#else
            qDebug() << Q_FUNC_INFO << "Device pixel ratios not supported for Qt versions before 5.4";
            mDevicePixelRatio = 1.0;
            mBuffer = QPixmap(mSize);
#endif
        } else
        {
            mBuffer = QPixmap(mSize);
        }
    }

} // QCP