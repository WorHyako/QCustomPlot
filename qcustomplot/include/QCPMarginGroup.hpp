#ifndef QCUSTOMPLOT_QCPMARGINGROUP_H
#define QCUSTOMPLOT_QCPMARGINGROUP_H

namespace QCP {

    class QCP_LIB_DECL QCPMarginGroup : public QObject
{
    Q_OBJECT
    public:
    explicit QCPMarginGroup(QCustomPlot *parentPlot);
    virtual ~QCPMarginGroup();

    // non-virtual methods:
    QList<QCPLayoutElement*> elements(QCP::MarginSide side) const { return mChildren.value(side); }
    bool isEmpty() const;
    void clear();

    protected:
    // non-property members:
    QCustomPlot *mParentPlot;
    QHash<QCP::MarginSide, QList<QCPLayoutElement*> > mChildren;

    // introduced virtual methods:
    virtual int commonMargin(QCP::MarginSide side) const;

    // non-virtual methods:
    void addChild(QCP::MarginSide side, QCPLayoutElement *element);
    void removeChild(QCP::MarginSide side, QCPLayoutElement *element);

    private:
    Q_DISABLE_COPY(QCPMarginGroup)

    friend class QCPLayoutElement;
};

} // QCP

#endif //QCUSTOMPLOT_QCPMARGINGROUP_H
