#ifndef QCUSTOMPLOT_QCPGRID_H
#define QCUSTOMPLOT_QCPGRID_H

namespace QCP {

    class QCP_LIB_DECL QCPGrid :public QCPLayerable
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(bool subGridVisible READ subGridVisible WRITE setSubGridVisible)
    Q_PROPERTY(bool antialiasedSubGrid READ antialiasedSubGrid WRITE setAntialiasedSubGrid)
    Q_PROPERTY(bool antialiasedZeroLine READ antialiasedZeroLine WRITE setAntialiasedZeroLine)
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(QPen subGridPen READ subGridPen WRITE setSubGridPen)
    Q_PROPERTY(QPen zeroLinePen READ zeroLinePen WRITE setZeroLinePen)
    /// \endcond
    public:
    explicit QCPGrid(QCPAxis *parentAxis);

    // getters:
    bool subGridVisible() const { return mSubGridVisible; }
    bool antialiasedSubGrid() const { return mAntialiasedSubGrid; }
    bool antialiasedZeroLine() const { return mAntialiasedZeroLine; }
    QPen pen() const { return mPen; }
    QPen subGridPen() const { return mSubGridPen; }
    QPen zeroLinePen() const { return mZeroLinePen; }

    // setters:
    void setSubGridVisible(bool visible);
    void setAntialiasedSubGrid(bool enabled);
    void setAntialiasedZeroLine(bool enabled);
    void setPen(const QPen &pen);
    void setSubGridPen(const QPen &pen);
    void setZeroLinePen(const QPen &pen);

    protected:
    // property members:
    bool mSubGridVisible;
    bool mAntialiasedSubGrid, mAntialiasedZeroLine;
    QPen mPen, mSubGridPen, mZeroLinePen;

    // non-property members:
    QCPAxis *mParentAxis;

    // reimplemented virtual methods:
    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const Q_DECL_OVERRIDE;
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;

    // non-virtual methods:
    void drawGridLines(QCPPainter *painter) const;
    void drawSubGridLines(QCPPainter *painter) const;

    friend class QCPAxis;
};



} // QCP

#endif //QCUSTOMPLOT_QCPGRID_H
