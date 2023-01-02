#ifndef QCUSTOMPLOT_QCPITEMBRACKET_H
#define QCUSTOMPLOT_QCPITEMBRACKET_H

namespace QCP {

    class QCP_LIB_DECL QCPItemBracket : public QCPAbstractItem
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
    Q_PROPERTY(double length READ length WRITE setLength)
    Q_PROPERTY(BracketStyle style READ style WRITE setStyle)
    /// \endcond
    public:
    /*!
    Defines the various visual shapes of the bracket item. The appearance can be further modified
    by \ref setLength and \ref setPen.

    \see setStyle
  */
    enum BracketStyle { bsSquare  ///< A brace with angled edges
        ,bsRound  ///< A brace with round edges
        ,bsCurly  ///< A curly brace
        ,bsCalligraphic ///< A curly brace with varying stroke width giving a calligraphic impression
    };
    Q_ENUMS(BracketStyle)

    explicit QCPItemBracket(QCustomPlot *parentPlot);
    virtual ~QCPItemBracket() Q_DECL_OVERRIDE;

    // getters:
    QPen pen() const { return mPen; }
    QPen selectedPen() const { return mSelectedPen; }
    double length() const { return mLength; }
    BracketStyle style() const { return mStyle; }

    // setters;
    void setPen(const QPen &pen);
    void setSelectedPen(const QPen &pen);
    void setLength(double length);
    void setStyle(BracketStyle style);

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;

    QCPItemPosition * const left;
    QCPItemPosition * const right;
    QCPItemAnchor * const center;

    protected:
    // property members:
    enum AnchorIndex {aiCenter};
    QPen mPen, mSelectedPen;
    double mLength;
    BracketStyle mStyle;

    // reimplemented virtual methods:
    virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;
    virtual QPointF anchorPixelPosition(int anchorId) const Q_DECL_OVERRIDE;

    // non-virtual methods:
    QPen mainPen() const;
};
Q_DECLARE_METATYPE(QCPItemBracket::BracketStyle)


} // QCP

#endif //QCUSTOMPLOT_QCPITEMBRACKET_H
