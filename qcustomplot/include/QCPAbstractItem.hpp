#ifndef QCUSTOMPLOT_QCPABSTRACTITEM_H
#define QCUSTOMPLOT_QCPABSTRACTITEM_H

namespace QCP {

    class QCP_LIB_DECL QCPAbstractItem : public QCPLayerable
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(bool clipToAxisRect READ clipToAxisRect WRITE setClipToAxisRect)
    Q_PROPERTY(QCPAxisRect* clipAxisRect READ clipAxisRect WRITE setClipAxisRect)
    Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectableChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectionChanged)
    /// \endcond
    public:
    explicit QCPAbstractItem(QCustomPlot *parentPlot);
    virtual ~QCPAbstractItem() Q_DECL_OVERRIDE;

    // getters:
    bool clipToAxisRect() const { return mClipToAxisRect; }
    QCPAxisRect *clipAxisRect() const;
    bool selectable() const { return mSelectable; }
    bool selected() const { return mSelected; }

    // setters:
    void setClipToAxisRect(bool clip);
    void setClipAxisRect(QCPAxisRect *rect);
    Q_SLOT void setSelectable(bool selectable);
    Q_SLOT void setSelected(bool selected);

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE = 0;

    // non-virtual methods:
    QList<QCPItemPosition*> positions() const { return mPositions; }
    QList<QCPItemAnchor*> anchors() const { return mAnchors; }
    QCPItemPosition *position(const QString &name) const;
    QCPItemAnchor *anchor(const QString &name) const;
    bool hasAnchor(const QString &name) const;

    signals:
    void selectionChanged(bool selected);
    void selectableChanged(bool selectable);

    protected:
    // property members:
    bool mClipToAxisRect;
    QPointer<QCPAxisRect> mClipAxisRect;
    QList<QCPItemPosition*> mPositions;
    QList<QCPItemAnchor*> mAnchors;
    bool mSelectable, mSelected;

    // reimplemented virtual methods:
    virtual QCP::Interaction selectionCategory() const Q_DECL_OVERRIDE;
    virtual QRect clipRect() const Q_DECL_OVERRIDE;
    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const Q_DECL_OVERRIDE;
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE = 0;
    // events:
    virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged) Q_DECL_OVERRIDE;
    virtual void deselectEvent(bool *selectionStateChanged) Q_DECL_OVERRIDE;

    // introduced virtual methods:
    virtual QPointF anchorPixelPosition(int anchorId) const;

    // non-virtual methods:
    double rectDistance(const QRectF &rect, const QPointF &pos, bool filledRect) const;
    QCPItemPosition *createPosition(const QString &name);
    QCPItemAnchor *createAnchor(const QString &name, int anchorId);

    private:
    Q_DISABLE_COPY(QCPAbstractItem)

    friend class QCustomPlot;
    friend class QCPItemAnchor;
};


} // QCP

#endif //QCUSTOMPLOT_QCPABSTRACTITEM_H
