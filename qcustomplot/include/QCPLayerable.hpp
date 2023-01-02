#ifndef QCUSTOMPLOT_QCPLAYERABLE_H
#define QCUSTOMPLOT_QCPLAYERABLE_H

namespace QCP {

    class QCP_LIB_DECL QCPLayerable : public QObject
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(bool visible READ visible WRITE setVisible)
    Q_PROPERTY(QCustomPlot* parentPlot READ parentPlot)
    Q_PROPERTY(QCPLayerable* parentLayerable READ parentLayerable)
    Q_PROPERTY(QCPLayer* layer READ layer WRITE setLayer NOTIFY layerChanged)
    Q_PROPERTY(bool antialiased READ antialiased WRITE setAntialiased)
    /// \endcond
    public:
    QCPLayerable(QCustomPlot *plot, QString targetLayer=QString(), QCPLayerable *parentLayerable=nullptr);
    virtual ~QCPLayerable();

    // getters:
    bool visible() const { return mVisible; }
    QCustomPlot *parentPlot() const { return mParentPlot; }
    QCPLayerable *parentLayerable() const { return mParentLayerable.data(); }
    QCPLayer *layer() const { return mLayer; }
    bool antialiased() const { return mAntialiased; }

    // setters:
    void setVisible(bool on);
    Q_SLOT bool setLayer(QCPLayer *layer);
    bool setLayer(const QString &layerName);
    void setAntialiased(bool enabled);

    // introduced virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const;

    // non-property methods:
    bool realVisibility() const;

    signals:
    void layerChanged(QCPLayer *newLayer);

    protected:
    // property members:
    bool mVisible;
    QCustomPlot *mParentPlot;
    QPointer<QCPLayerable> mParentLayerable;
    QCPLayer *mLayer;
    bool mAntialiased;

    // introduced virtual methods:
    virtual void parentPlotInitialized(QCustomPlot *parentPlot);
    virtual QCP::Interaction selectionCategory() const;
    virtual QRect clipRect() const;
    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const = 0;
    virtual void draw(QCPPainter *painter) = 0;
    // selection events:
    virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
    virtual void deselectEvent(bool *selectionStateChanged);
    // low-level mouse events:
    virtual void mousePressEvent(QMouseEvent *event, const QVariant &details);
    virtual void mouseMoveEvent(QMouseEvent *event, const QPointF &startPos);
    virtual void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos);
    virtual void mouseDoubleClickEvent(QMouseEvent *event, const QVariant &details);
    virtual void wheelEvent(QWheelEvent *event);

    // non-property methods:
    void initializeParentPlot(QCustomPlot *parentPlot);
    void setParentLayerable(QCPLayerable* parentLayerable);
    bool moveToLayer(QCPLayer *layer, bool prepend);
    void applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const;

    private:
    Q_DISABLE_COPY(QCPLayerable)

    friend class QCustomPlot;
    friend class QCPLayer;
    friend class QCPAxisRect;
};


} // QCP

#endif //QCUSTOMPLOT_QCPLAYERABLE_H
