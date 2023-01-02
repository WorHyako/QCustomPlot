#ifndef QCUSTOMPLOT_QCPPAINTBUFFERPIXMAP_H
#define QCUSTOMPLOT_QCPPAINTBUFFERPIXMAP_H

namespace QCP {


    class QCP_LIB_DECL QCPPaintBufferPixmap : public QCPAbstractPaintBuffer
{
    public:
    explicit QCPPaintBufferPixmap(const QSize &size, double devicePixelRatio);
    virtual ~QCPPaintBufferPixmap() Q_DECL_OVERRIDE;

    // reimplemented virtual methods:
    virtual QCPPainter *startPainting() Q_DECL_OVERRIDE;
    virtual void draw(QCPPainter *painter) const Q_DECL_OVERRIDE;
    void clear(const QColor &color) Q_DECL_OVERRIDE;

    protected:
    // non-property members:
    QPixmap mBuffer;

    // reimplemented virtual methods:
    virtual void reallocateBuffer() Q_DECL_OVERRIDE;
};

} // QCP

#endif //QCUSTOMPLOT_QCPPAINTBUFFERPIXMAP_H
