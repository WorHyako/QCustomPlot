#ifndef QCUSTOMPLOT_QCPABSTRACTLEGENDITEM_H
#define QCUSTOMPLOT_QCPABSTRACTLEGENDITEM_H

namespace QCP {


    class QCP_LIB_DECL QCPAbstractLegendItem : public QCPLayoutElement
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QCPLegend* parentLegend READ parentLegend)
    Q_PROPERTY(QFont font READ font WRITE setFont)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
    Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)
    Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectionChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectableChanged)
    /// \endcond
    public:
    explicit QCPAbstractLegendItem(QCPLegend *parent);

    // getters:
    QCPLegend *parentLegend() const { return mParentLegend; }
    QFont font() const { return mFont; }
    QColor textColor() const { return mTextColor; }
    QFont selectedFont() const { return mSelectedFont; }
    QColor selectedTextColor() const { return mSelectedTextColor; }
    bool selectable() const { return mSelectable; }
    bool selected() const { return mSelected; }

    // setters:
    void setFont(const QFont &font);
    void setTextColor(const QColor &color);
    void setSelectedFont(const QFont &font);
    void setSelectedTextColor(const QColor &color);
    Q_SLOT void setSelectable(bool selectable);
    Q_SLOT void setSelected(bool selected);

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;

    signals:
    void selectionChanged(bool selected);
    void selectableChanged(bool selectable);

    protected:
    // property members:
    QCPLegend *mParentLegend;
    QFont mFont;
    QColor mTextColor;
    QFont mSelectedFont;
    QColor mSelectedTextColor;
    bool mSelectable, mSelected;

    // reimplemented virtual methods:
    virtual QCP::Interaction selectionCategory() const Q_DECL_OVERRIDE;
    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const Q_DECL_OVERRIDE;
    virtual QRect clipRect() const Q_DECL_OVERRIDE;
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE = 0;
    // events:
    virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged) Q_DECL_OVERRIDE;
    virtual void deselectEvent(bool *selectionStateChanged) Q_DECL_OVERRIDE;

    private:
    Q_DISABLE_COPY(QCPAbstractLegendItem)

    friend class QCPLegend;
};


} // QCP

#endif //QCUSTOMPLOT_QCPABSTRACTLEGENDITEM_H
