#ifndef QCUSTOMPLOT_QCPLAYOUTGRID_H
#define QCUSTOMPLOT_QCPLAYOUTGRID_H

namespace QCP {

    class QCP_LIB_DECL QCPLayoutGrid : public QCPLayout
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(int rowCount READ rowCount)
    Q_PROPERTY(int columnCount READ columnCount)
    Q_PROPERTY(QList<double> columnStretchFactors READ columnStretchFactors WRITE setColumnStretchFactors)
    Q_PROPERTY(QList<double> rowStretchFactors READ rowStretchFactors WRITE setRowStretchFactors)
    Q_PROPERTY(int columnSpacing READ columnSpacing WRITE setColumnSpacing)
    Q_PROPERTY(int rowSpacing READ rowSpacing WRITE setRowSpacing)
    Q_PROPERTY(FillOrder fillOrder READ fillOrder WRITE setFillOrder)
    Q_PROPERTY(int wrap READ wrap WRITE setWrap)
    /// \endcond
    public:

    /*!
    Defines in which direction the grid is filled when using \ref addElement(QCPLayoutElement*).
    The column/row at which wrapping into the next row/column occurs can be specified with \ref
    setWrap.

    \see setFillOrder
  */
    enum FillOrder { foRowsFirst    ///< Rows are filled first, and a new element is wrapped to the next column if the row count would exceed \ref setWrap.
        ,foColumnsFirst ///< Columns are filled first, and a new element is wrapped to the next row if the column count would exceed \ref setWrap.
    };
    Q_ENUMS(FillOrder)

    explicit QCPLayoutGrid();
    virtual ~QCPLayoutGrid() Q_DECL_OVERRIDE;

    // getters:
    int rowCount() const { return mElements.size(); }
    int columnCount() const { return mElements.size() > 0 ? mElements.first().size() : 0; }
    QList<double> columnStretchFactors() const { return mColumnStretchFactors; }
    QList<double> rowStretchFactors() const { return mRowStretchFactors; }
    int columnSpacing() const { return mColumnSpacing; }
    int rowSpacing() const { return mRowSpacing; }
    int wrap() const { return mWrap; }
    FillOrder fillOrder() const { return mFillOrder; }

    // setters:
    void setColumnStretchFactor(int column, double factor);
    void setColumnStretchFactors(const QList<double> &factors);
    void setRowStretchFactor(int row, double factor);
    void setRowStretchFactors(const QList<double> &factors);
    void setColumnSpacing(int pixels);
    void setRowSpacing(int pixels);
    void setWrap(int count);
    void setFillOrder(FillOrder order, bool rearrange=true);

    // reimplemented virtual methods:
    virtual void updateLayout() Q_DECL_OVERRIDE;
    virtual int elementCount() const Q_DECL_OVERRIDE { return rowCount()*columnCount(); }
virtual QCPLayoutElement* elementAt(int index) const Q_DECL_OVERRIDE;
virtual QCPLayoutElement* takeAt(int index) Q_DECL_OVERRIDE;
virtual bool take(QCPLayoutElement* element) Q_DECL_OVERRIDE;
virtual QList<QCPLayoutElement*> elements(bool recursive) const Q_DECL_OVERRIDE;
virtual void simplify() Q_DECL_OVERRIDE;
virtual QSize minimumOuterSizeHint() const Q_DECL_OVERRIDE;
virtual QSize maximumOuterSizeHint() const Q_DECL_OVERRIDE;

// non-virtual methods:
QCPLayoutElement *element(int row, int column) const;
bool addElement(int row, int column, QCPLayoutElement *element);
bool addElement(QCPLayoutElement *element);
bool hasElement(int row, int column);
void expandTo(int newRowCount, int newColumnCount);
void insertRow(int newIndex);
void insertColumn(int newIndex);
int rowColToIndex(int row, int column) const;
void indexToRowCol(int index, int &row, int &column) const;

protected:
// property members:
QList<QList<QCPLayoutElement*> > mElements;
QList<double> mColumnStretchFactors;
QList<double> mRowStretchFactors;
int mColumnSpacing, mRowSpacing;
int mWrap;
FillOrder mFillOrder;

// non-virtual methods:
void getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const;
void getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const;

private:
Q_DISABLE_COPY(QCPLayoutGrid)
};
Q_DECLARE_METATYPE(QCPLayoutGrid::FillOrder)



} // QCP

#endif //QCUSTOMPLOT_QCPLAYOUTGRID_H
