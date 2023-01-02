#ifndef QCUSTOMPLOT_QCPABSTRACTPAINTBUFFER_H
#define QCUSTOMPLOT_QCPABSTRACTPAINTBUFFER_H

namespace QCP {


    class QCP_LIB_DECL QCPAbstractPaintBuffer
            {
                    public:
                    explicit QCPAbstractPaintBuffer(const QSize &size, double devicePixelRatio);
                    virtual ~QCPAbstractPaintBuffer();

                    // getters:
                    QSize size() const { return mSize; }
                    bool invalidated() const { return mInvalidated; }
                    double devicePixelRatio() const { return mDevicePixelRatio; }

                    // setters:
                    void setSize(const QSize &size);
                    void setInvalidated(bool invalidated=true);
                    void setDevicePixelRatio(double ratio);

                    // introduced virtual methods:
                    virtual QCPPainter *startPainting() = 0;
                    virtual void donePainting() {}
                    virtual void draw(QCPPainter *painter) const = 0;
                    virtual void clear(const QColor &color) = 0;

                    protected:
                    // property members:
                    QSize mSize;
                    double mDevicePixelRatio;

                    // non-property members:
                    bool mInvalidated;

                    // introduced virtual methods:
                    virtual void reallocateBuffer() = 0;
            };

} // QCP

#endif //QCUSTOMPLOT_QCPABSTRACTPAINTBUFFER_H
