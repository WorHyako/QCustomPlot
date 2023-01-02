#ifndef QCUSTOMPLOT_QCPITEMANCHOR_H
#define QCUSTOMPLOT_QCPITEMANCHOR_H

namespace QCP {

    class QCP_LIB_DECL QCPItemAnchor
            {
                    Q_GADGET
                    public:
                    QCPItemAnchor(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString &name, int anchorId=-1);
                    virtual ~QCPItemAnchor();

                    // getters:
                    QString name() const { return mName; }
                    virtual QPointF pixelPosition() const;

                    protected:
                    // property members:
                    QString mName;

                    // non-property members:
                    QCustomPlot *mParentPlot;
                    QCPAbstractItem *mParentItem;
                    int mAnchorId;
                    QSet<QCPItemPosition*> mChildrenX, mChildrenY;

                    // introduced virtual methods:
                    virtual QCPItemPosition *toQCPItemPosition() { return nullptr; }

                    // non-virtual methods:
                    void addChildX(QCPItemPosition* pos); // called from pos when this anchor is set as parent
                    void removeChildX(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted
                    void addChildY(QCPItemPosition* pos); // called from pos when this anchor is set as parent
                    void removeChildY(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted

                    private:
                    Q_DISABLE_COPY(QCPItemAnchor)

                    friend class QCPItemPosition;
            };



} // QCP

#endif //QCUSTOMPLOT_QCPITEMANCHOR_H
