#ifndef QCUSTOMPLOT_QCPSELECTIONDECORATOR_H
#define QCUSTOMPLOT_QCPSELECTIONDECORATOR_H

namespace QCP {

    class QCP_LIB_DECL QCPSelectionDecorator
            {
                    Q_GADGET
                    public:
                    QCPSelectionDecorator();
                    virtual ~QCPSelectionDecorator();

                    // getters:
                    QPen pen() const { return mPen; }
                    QBrush brush() const { return mBrush; }
                    QCPScatterStyle scatterStyle() const { return mScatterStyle; }
                    QCPScatterStyle::ScatterProperties usedScatterProperties() const { return mUsedScatterProperties; }

                    // setters:
                    void setPen(const QPen &pen);
                    void setBrush(const QBrush &brush);
                    void setScatterStyle(const QCPScatterStyle &scatterStyle, QCPScatterStyle::ScatterProperties usedProperties=QCPScatterStyle::spPen);
                    void setUsedScatterProperties(const QCPScatterStyle::ScatterProperties &properties);

                    // non-virtual methods:
                    void applyPen(QCPPainter *painter) const;
                    void applyBrush(QCPPainter *painter) const;
                    QCPScatterStyle getFinalScatterStyle(const QCPScatterStyle &unselectedStyle) const;

                    // introduced virtual methods:
                    virtual void copyFrom(const QCPSelectionDecorator *other);
                    virtual void drawDecoration(QCPPainter *painter, QCPDataSelection selection);

                    protected:
                    // property members:
                    QPen mPen;
                    QBrush mBrush;
                    QCPScatterStyle mScatterStyle;
                    QCPScatterStyle::ScatterProperties mUsedScatterProperties;
                    // non-property members:
                    QCPAbstractPlottable *mPlottable;

                    // introduced virtual methods:
                    virtual bool registerWithPlottable(QCPAbstractPlottable *plottable);

                    private:
                    Q_DISABLE_COPY(QCPSelectionDecorator)
                    friend class QCPAbstractPlottable;
            };
    Q_DECLARE_METATYPE(QCPSelectionDecorator*)


} // QCP

#endif //QCUSTOMPLOT_QCPSELECTIONDECORATOR_H
