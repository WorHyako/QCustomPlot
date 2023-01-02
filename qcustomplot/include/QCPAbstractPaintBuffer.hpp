#ifndef QCUSTOMPLOT_QCPABSTRACTPAINTBUFFER_HPP
#define QCUSTOMPLOT_QCPABSTRACTPAINTBUFFER_HPP

#include "defs.hpp"
#include "QCPPainter.hpp"

#include <QColor>
#include <QSize>

namespace QCP {

    class QCP_LIB_DECL QCPAbstractPaintBuffer {
    public:
        QCPAbstractPaintBuffer(const QSize &size, double devicePixelRatio);

        virtual ~QCPAbstractPaintBuffer() = default;

        QSize size() const { return mSize; }

        bool invalidated() const { return mInvalidated; }

        double devicePixelRatio() const { return mDevicePixelRatio; }

        void setSize(const QSize &size);

        void setInvalidated(bool invalidated = true);

        void setDevicePixelRatio(double ratio);

        virtual QCPPainter *startPainting() = 0;

        virtual void donePainting() {}

        virtual void draw(QCPPainter *painter) const = 0;

        virtual void clear(const QColor &color) = 0;

    protected:
        QSize mSize;
        double mDevicePixelRatio;

        bool mInvalidated;

        virtual void reallocateBuffer() = 0;
    };
}
#endif
