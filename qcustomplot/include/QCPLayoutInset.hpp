#ifndef QCUSTOMPLOT_QCPLAYOUTINSET_H
#define QCUSTOMPLOT_QCPLAYOUTINSET_H

namespace QCP {

    class QCP_LIB_DECL QCPLayoutInset : public QCPLayout
{
    Q_OBJECT
    public:
/*!
Defines how the placement and sizing is handled for a certain element in a QCPLayoutInset.
*/
    enum InsetPlacement { ipFree            ///< The element may be positioned/sized arbitrarily, see \ref setInsetRect
        ,ipBorderAligned  ///< The element is aligned to one of the layout sides, see \ref setInsetAlignment
    };
    Q_ENUMS(InsetPlacement)

    explicit QCPLayoutInset();
    virtual ~QCPLayoutInset() Q_DECL_OVERRIDE;

// getters:
    InsetPlacement insetPlacement(int index) const;
    Qt::Alignment insetAlignment(int index) const;
    QRectF insetRect(int index) const;

// setters:
    void setInsetPlacement(int index, InsetPlacement placement);
    void setInsetAlignment(int index, Qt::Alignment alignment);
    void setInsetRect(int index, const QRectF &rect);

// reimplemented virtual methods:
    virtual void updateLayout() Q_DECL_OVERRIDE;
    virtual int elementCount() const Q_DECL_OVERRIDE;
    virtual QCPLayoutElement* elementAt(int index) const Q_DECL_OVERRIDE;
    virtual QCPLayoutElement* takeAt(int index) Q_DECL_OVERRIDE;
    virtual bool take(QCPLayoutElement* element) Q_DECL_OVERRIDE;
    virtual void simplify() Q_DECL_OVERRIDE {}
virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;

// non-virtual methods:
void addElement(QCPLayoutElement *element, Qt::Alignment alignment);
void addElement(QCPLayoutElement *element, const QRectF &rect);

protected:
// property members:
QList<QCPLayoutElement*> mElements;
QList<InsetPlacement> mInsetPlacement;
QList<Qt::Alignment> mInsetAlignment;
QList<QRectF> mInsetRect;

private:
Q_DISABLE_COPY(QCPLayoutInset)
};
Q_DECLARE_METATYPE(QCPLayoutInset::InsetPlacement)


} // QCP

#endif //QCUSTOMPLOT_QCPLAYOUTINSET_H
