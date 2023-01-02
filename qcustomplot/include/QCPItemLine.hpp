#ifndef QCUSTOMPLOT_QCPITEMLINE_H
#define QCUSTOMPLOT_QCPITEMLINE_H

namespace QCP {

    class QCP_LIB_DECL QCPItemLine : public QCPAbstractItem
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
    Q_PROPERTY(QCPLineEnding head READ head WRITE setHead)
    Q_PROPERTY(QCPLineEnding tail READ tail WRITE setTail)
    /// \endcond
    public:
    explicit QCPItemLine(QCustomPlot *parentPlot);
    virtual ~QCPItemLine() Q_DECL_OVERRIDE;

    // getters:
    QPen pen() const { return mPen; }
    QPen selectedPen() const { return mSelectedPen; }
    QCPLineEnding head() const { return mHead; }
    QCPLineEnding tail() const { return mTail; }

    // setters;
    void setPen(const QPen &pen);
    void setSelectedPen(const QPen &pen);
    void setHead(const QCPLineEnding &head);
    void setTail(const QCPLineEnding &tail);

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;

    QCPItemPosition * const start;
    QCPItemPosition * const end;

    protected:
    // property members:
    QPen mPen, mSelectedPen;
    QCPLineEnding mHead, mTail;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;

    // non-virtual methods:
    QLineF getRectClippedLine(const QCPVector2D &start, const QCPVector2D &end, const QRect &rect) const;
    QPen mainPen() const;
};

} // QCP

#endif //QCUSTOMPLOT_QCPITEMLINE_H
