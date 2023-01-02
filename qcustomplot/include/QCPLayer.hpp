#ifndef QCUSTOMPLOT_QCPLAYER_H
#define QCUSTOMPLOT_QCPLAYER_H

namespace QCP {

    class QCP_LIB_DECL QCPLayer : public QObject
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QCustomPlot* parentPlot READ parentPlot)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(int index READ index)
    Q_PROPERTY(QList<QCPLayerable*> children READ children)
    Q_PROPERTY(bool visible READ visible WRITE setVisible)
    Q_PROPERTY(LayerMode mode READ mode WRITE setMode)
    /// \endcond
    public:

    /*!
    Defines the different rendering modes of a layer. Depending on the mode, certain layers can be
    replotted individually, without the need to replot (possibly complex) layerables on other
    layers.

    \see setMode
  */
    enum LayerMode { lmLogical   ///< Layer is used only for rendering order, and shares paint buffer with all other adjacent logical layers.
        ,lmBuffered ///< Layer has its own paint buffer and may be replotted individually (see \ref replot).
    };
    Q_ENUMS(LayerMode)

    QCPLayer(QCustomPlot* parentPlot, const QString &layerName);
    virtual ~QCPLayer();

    // getters:
    QCustomPlot *parentPlot() const { return mParentPlot; }
    QString name() const { return mName; }
    int index() const { return mIndex; }
    QList<QCPLayerable*> children() const { return mChildren; }
    bool visible() const { return mVisible; }
    LayerMode mode() const { return mMode; }

    // setters:
    void setVisible(bool visible);
    void setMode(LayerMode mode);

    // non-virtual methods:
    void replot();

    protected:
    // property members:
    QCustomPlot *mParentPlot;
    QString mName;
    int mIndex;
    QList<QCPLayerable*> mChildren;
    bool mVisible;
    LayerMode mMode;

    // non-property members:
    QWeakPointer<QCPAbstractPaintBuffer> mPaintBuffer;

    // non-virtual methods:
    void draw(QCPPainter *painter);
    void drawToPaintBuffer();
    void addChild(QCPLayerable *layerable, bool prepend);
    void removeChild(QCPLayerable *layerable);

    private:
    Q_DISABLE_COPY(QCPLayer)

    friend class QCustomPlot;
    friend class QCPLayerable;
};
Q_DECLARE_METATYPE(QCPLayer::LayerMode)


} // QCP

#endif //QCUSTOMPLOT_QCPLAYER_H
