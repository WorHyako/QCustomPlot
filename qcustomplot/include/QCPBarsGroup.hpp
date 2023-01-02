#ifndef QCUSTOMPLOT_QCPBARSGROUP_H
#define QCUSTOMPLOT_QCPBARSGROUP_H

namespace QCP {

    class QCP_LIB_DECL QCPBarsGroup : public QObject
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(SpacingType spacingType READ spacingType WRITE setSpacingType)
    Q_PROPERTY(double spacing READ spacing WRITE setSpacing)
    /// \endcond
    public:
    /*!
    Defines the ways the spacing between bars in the group can be specified. Thus it defines what
    the number passed to \ref setSpacing actually means.

    \see setSpacingType, setSpacing
  */
    enum SpacingType { stAbsolute       ///< Bar spacing is in absolute pixels
        ,stAxisRectRatio ///< Bar spacing is given by a fraction of the axis rect size
        ,stPlotCoords    ///< Bar spacing is in key coordinates and thus scales with the key axis range
    };
    Q_ENUMS(SpacingType)

    explicit QCPBarsGroup(QCustomPlot *parentPlot);
    virtual ~QCPBarsGroup();

    // getters:
    SpacingType spacingType() const { return mSpacingType; }
    double spacing() const { return mSpacing; }

    // setters:
    void setSpacingType(SpacingType spacingType);
    void setSpacing(double spacing);

    // non-virtual methods:
    QList<QCPBars*> bars() const { return mBars; }
    QCPBars* bars(int index) const;
    int size() const { return mBars.size(); }
    bool isEmpty() const { return mBars.isEmpty(); }
    void clear();
    bool contains(QCPBars *bars) const { return mBars.contains(bars); }
    void append(QCPBars *bars);
    void insert(int i, QCPBars *bars);
    void remove(QCPBars *bars);

    protected:
    // non-property members:
    QCustomPlot *mParentPlot;
    SpacingType mSpacingType;
    double mSpacing;
    QList<QCPBars*> mBars;

    // non-virtual methods:
    void registerBars(QCPBars *bars);
    void unregisterBars(QCPBars *bars);

    // virtual methods:
    double keyPixelOffset(const QCPBars *bars, double keyCoord);
    double getPixelSpacing(const QCPBars *bars, double keyCoord);

    private:
    Q_DISABLE_COPY(QCPBarsGroup)

    friend class QCPBars;
};
Q_DECLARE_METATYPE(QCPBarsGroup::SpacingType)


} // QCP

#endif //QCUSTOMPLOT_QCPBARSGROUP_H
