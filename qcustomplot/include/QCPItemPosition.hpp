#ifndef QCUSTOMPLOT_QCPITEMPOSITION_H
#define QCUSTOMPLOT_QCPITEMPOSITION_H

namespace QCP {

    class QCP_LIB_DECL QCPItemPosition : public QCPItemAnchor
{
    Q_GADGET
    public:
    /*!
    Defines the ways an item position can be specified. Thus it defines what the numbers passed to
    \ref setCoords actually mean.

    \see setType
  */
    enum PositionType { ptAbsolute        ///< Static positioning in pixels, starting from the top left corner of the viewport/widget.
        ,ptViewportRatio  ///< Static positioning given by a fraction of the viewport size. For example, if you call setCoords(0, 0), the position will be at the top
        ///< left corner of the viewport/widget. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
        ///< vertically at the top of the viewport/widget, etc.
        ,ptAxisRectRatio  ///< Static positioning given by a fraction of the axis rect size (see \ref setAxisRect). For example, if you call setCoords(0, 0), the position will be at the top
        ///< left corner of the axis rect. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
        ///< vertically at the top of the axis rect, etc. You can also go beyond the axis rect by providing negative coordinates or coordinates larger than 1.
        ,ptPlotCoords     ///< Dynamic positioning at a plot coordinate defined by two axes (see \ref setAxes).
    };
    Q_ENUMS(PositionType)

    QCPItemPosition(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString &name);
    virtual ~QCPItemPosition() Q_DECL_OVERRIDE;

    // getters:
    PositionType type() const { return typeX(); }
    PositionType typeX() const { return mPositionTypeX; }
    PositionType typeY() const { return mPositionTypeY; }
    QCPItemAnchor *parentAnchor() const { return parentAnchorX(); }
    QCPItemAnchor *parentAnchorX() const { return mParentAnchorX; }
    QCPItemAnchor *parentAnchorY() const { return mParentAnchorY; }
    double key() const { return mKey; }
    double value() const { return mValue; }
    QPointF coords() const { return QPointF(mKey, mValue); }
    QCPAxis *keyAxis() const { return mKeyAxis.data(); }
    QCPAxis *valueAxis() const { return mValueAxis.data(); }
    QCPAxisRect *axisRect() const;
    virtual QPointF pixelPosition() const Q_DECL_OVERRIDE;

    // setters:
    void setType(PositionType type);
    void setTypeX(PositionType type);
    void setTypeY(PositionType type);
    bool setParentAnchor(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
    bool setParentAnchorX(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
    bool setParentAnchorY(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
    void setCoords(double key, double value);
    void setCoords(const QPointF &pos);
    void setAxes(QCPAxis* keyAxis, QCPAxis* valueAxis);
    void setAxisRect(QCPAxisRect *axisRect);
    void setPixelPosition(const QPointF &pixelPosition);

    protected:
    // property members:
    PositionType mPositionTypeX, mPositionTypeY;
    QPointer<QCPAxis> mKeyAxis, mValueAxis;
    QPointer<QCPAxisRect> mAxisRect;
    double mKey, mValue;
    QCPItemAnchor *mParentAnchorX, *mParentAnchorY;

    // reimplemented virtual methods:
    virtual QCPItemPosition *toQCPItemPosition() Q_DECL_OVERRIDE { return this; }

private:
Q_DISABLE_COPY(QCPItemPosition)

};
Q_DECLARE_METATYPE(QCPItemPosition::PositionType)


} // QCp

#endif //QCUSTOMPLOT_QCPITEMPOSITION_H
