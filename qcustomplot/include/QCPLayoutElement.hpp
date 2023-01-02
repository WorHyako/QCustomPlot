#ifndef QCUSTOMPLOT_QCPLAYOUTELEMENT_H
#define QCUSTOMPLOT_QCPLAYOUTELEMENT_H

namespace QCP {

    class QCP_LIB_DECL QCPLayoutElement : public QCPLayerable
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QCPLayout* layout READ layout)
    Q_PROPERTY(QRect rect READ rect)
    Q_PROPERTY(QRect outerRect READ outerRect WRITE setOuterRect)
    Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
    Q_PROPERTY(QMargins minimumMargins READ minimumMargins WRITE setMinimumMargins)
    Q_PROPERTY(QSize minimumSize READ minimumSize WRITE setMinimumSize)
    Q_PROPERTY(QSize maximumSize READ maximumSize WRITE setMaximumSize)
    Q_PROPERTY(SizeConstraintRect sizeConstraintRect READ sizeConstraintRect WRITE setSizeConstraintRect)
    /// \endcond
    public:
    /*!
    Defines the phases of the update process, that happens just before a replot. At each phase,
    \ref update is called with the according UpdatePhase value.
  */
    enum UpdatePhase { upPreparation ///< Phase used for any type of preparation that needs to be done before margin calculation and layout
        ,upMargins    ///< Phase in which the margins are calculated and set
        ,upLayout     ///< Final phase in which the layout system places the rects of the elements
    };
    Q_ENUMS(UpdatePhase)

    /*!
    Defines to which rect of a layout element the size constraints that can be set via \ref
    setMinimumSize and \ref setMaximumSize apply. The outer rect (\ref outerRect) includes the
    margins (e.g. in the case of a QCPAxisRect the axis labels), whereas the inner rect (\ref rect)
    does not.

    \see setSizeConstraintRect
  */
    enum SizeConstraintRect { scrInnerRect ///< Minimum/Maximum size constraints apply to inner rect
        , scrOuterRect ///< Minimum/Maximum size constraints apply to outer rect, thus include layout element margins
    };
    Q_ENUMS(SizeConstraintRect)

    explicit QCPLayoutElement(QCustomPlot *parentPlot=nullptr);
    virtual ~QCPLayoutElement() Q_DECL_OVERRIDE;

    // getters:
    QCPLayout *layout() const { return mParentLayout; }
    QRect rect() const { return mRect; }
    QRect outerRect() const { return mOuterRect; }
    QMargins margins() const { return mMargins; }
    QMargins minimumMargins() const { return mMinimumMargins; }
    QCP::MarginSides autoMargins() const { return mAutoMargins; }
    QSize minimumSize() const { return mMinimumSize; }
    QSize maximumSize() const { return mMaximumSize; }
    SizeConstraintRect sizeConstraintRect() const { return mSizeConstraintRect; }
    QCPMarginGroup *marginGroup(QCP::MarginSide side) const { return mMarginGroups.value(side, nullptr); }
    QHash<QCP::MarginSide, QCPMarginGroup*> marginGroups() const { return mMarginGroups; }

    // setters:
    void setOuterRect(const QRect &rect);
    void setMargins(const QMargins &margins);
    void setMinimumMargins(const QMargins &margins);
    void setAutoMargins(QCP::MarginSides sides);
    void setMinimumSize(const QSize &size);
    void setMinimumSize(int width, int height);
    void setMaximumSize(const QSize &size);
    void setMaximumSize(int width, int height);
    void setSizeConstraintRect(SizeConstraintRect constraintRect);
    void setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group);

    // introduced virtual methods:
    virtual void update(UpdatePhase phase);
    virtual QSize minimumOuterSizeHint() const;
    virtual QSize maximumOuterSizeHint() const;
    virtual QList<QCPLayoutElement*> elements(bool recursive) const;

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=nullptr) const Q_DECL_OVERRIDE;

    protected:
    // property members:
    QCPLayout *mParentLayout;
    QSize mMinimumSize, mMaximumSize;
    SizeConstraintRect mSizeConstraintRect;
    QRect mRect, mOuterRect;
    QMargins mMargins, mMinimumMargins;
    QCP::MarginSides mAutoMargins;
    QHash<QCP::MarginSide, QCPMarginGroup*> mMarginGroups;

    // introduced virtual methods:
    virtual int calculateAutoMargin(QCP::MarginSide side);
    virtual void layoutChanged();

    // reimplemented virtual methods:
    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const Q_DECL_OVERRIDE { Q_UNUSED(painter) }
virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE { Q_UNUSED(painter) }
virtual void parentPlotInitialized(QCustomPlot *parentPlot) Q_DECL_OVERRIDE;

private:
Q_DISABLE_COPY(QCPLayoutElement)

friend class QCustomPlot;
friend class QCPLayout;
friend class QCPMarginGroup;
};
Q_DECLARE_METATYPE(QCPLayoutElement::UpdatePhase)


} // QCP

#endif //QCUSTOMPLOT_QCPLAYOUTELEMENT_H
