#include "include/QCPLayoutGrid.hpp"

namespace QCP {
/*! \class QCPLayoutGrid
  \brief A layout that arranges child elements in a grid

  Elements are laid out in a grid with configurable stretch factors (\ref setColumnStretchFactor,
  \ref setRowStretchFactor) and spacing (\ref setColumnSpacing, \ref setRowSpacing).

  Elements can be added to cells via \ref addElement. The grid is expanded if the specified row or
  column doesn't exist yet. Whether a cell contains a valid layout element can be checked with \ref
  hasElement, that element can be retrieved with \ref element. If rows and columns that only have
  empty cells shall be removed, call \ref simplify. Removal of elements is either done by just
  adding the element to a different layout or by using the QCPLayout interface \ref take or \ref
  remove.

  If you use \ref addElement(QCPLayoutElement*) without explicit parameters for \a row and \a
  column, the grid layout will choose the position according to the current \ref setFillOrder and
  the wrapping (\ref setWrap).

  Row and column insertion can be performed with \ref insertRow and \ref insertColumn.
*/

/* start documentation of inline functions */

/*! \fn int QCPLayoutGrid::rowCount() const

  Returns the number of rows in the layout.

  \see columnCount
*/

/*! \fn int QCPLayoutGrid::columnCount() const

  Returns the number of columns in the layout.

  \see rowCount
*/

/* end documentation of inline functions */

/*!
  Creates an instance of QCPLayoutGrid and sets default values.
*/
    QCPLayoutGrid::QCPLayoutGrid() :
            mColumnSpacing(5),
            mRowSpacing(5),
            mWrap(0),
            mFillOrder(foColumnsFirst)
    {
    }

    QCPLayoutGrid::~QCPLayoutGrid()
    {
// clear all child layout elements. This is important because only the specific layouts know how
// to handle removing elements (clear calls virtual removeAt method to do that).
        clear();
    }

/*!
  Returns the element in the cell in \a row and \a column.

  Returns \c nullptr if either the row/column is invalid or if the cell is empty. In those cases, a
  qDebug message is printed. To check whether a cell exists and isn't empty, use \ref hasElement.

  \see addElement, hasElement
*/
    QCPLayoutElement *QCPLayoutGrid::element(int row, int column) const
    {
        if (row >= 0 && row < mElements.size())
        {
            if (column >= 0 && column < mElements.first().size())
            {
                if (QCPLayoutElement *result = mElements.at(row).at(column))
                    return result;
                else
                    qDebug() << Q_FUNC_INFO << "Requested cell is empty. Row:" << row << "Column:" << column;
            } else
                qDebug() << Q_FUNC_INFO << "Invalid column. Row:" << row << "Column:" << column;
        } else
            qDebug() << Q_FUNC_INFO << "Invalid row. Row:" << row << "Column:" << column;
        return nullptr;
    }


/*! \overload

  Adds the \a element to cell with \a row and \a column. If \a element is already in a layout, it
  is first removed from there. If \a row or \a column don't exist yet, the layout is expanded
  accordingly.

  Returns true if the element was added successfully, i.e. if the cell at \a row and \a column
  didn't already have an element.

  Use the overload of this method without explicit row/column index to place the element according
  to the configured fill order and wrapping settings.

  \see element, hasElement, take, remove
*/
    bool QCPLayoutGrid::addElement(int row, int column, QCPLayoutElement *element)
    {
        if (!hasElement(row, column))
        {
            if (element && element->layout()) // remove from old layout first
                element->layout()->take(element);
            expandTo(row+1, column+1);
            mElements[row][column] = element;
            if (element)
                adoptElement(element);
            return true;
        } else
            qDebug() << Q_FUNC_INFO << "There is already an element in the specified row/column:" << row << column;
        return false;
    }

/*! \overload

  Adds the \a element to the next empty cell according to the current fill order (\ref
  setFillOrder) and wrapping (\ref setWrap). If \a element is already in a layout, it is first
  removed from there. If necessary, the layout is expanded to hold the new element.

  Returns true if the element was added successfully.

  \see setFillOrder, setWrap, element, hasElement, take, remove
*/
    bool QCPLayoutGrid::addElement(QCPLayoutElement *element)
    {
        int rowIndex = 0;
        int colIndex = 0;
        if (mFillOrder == foColumnsFirst)
        {
            while (hasElement(rowIndex, colIndex))
            {
                ++colIndex;
                if (colIndex >= mWrap && mWrap > 0)
                {
                    colIndex = 0;
                    ++rowIndex;
                }
            }
        } else
        {
            while (hasElement(rowIndex, colIndex))
            {
                ++rowIndex;
                if (rowIndex >= mWrap && mWrap > 0)
                {
                    rowIndex = 0;
                    ++colIndex;
                }
            }
        }
        return addElement(rowIndex, colIndex, element);
    }

/*!
  Returns whether the cell at \a row and \a column exists and contains a valid element, i.e. isn't
  empty.

  \see element
*/
    bool QCPLayoutGrid::hasElement(int row, int column)
    {
        if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount())
            return mElements.at(row).at(column);
        else
            return false;
    }

/*!
  Sets the stretch \a factor of \a column.

  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights, regardless of the stretch factor. (see \ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize, \ref
  QCPLayoutElement::setSizeConstraintRect.)

  The default stretch factor of newly created rows/columns is 1.

  \see setColumnStretchFactors, setRowStretchFactor
*/
    void QCPLayoutGrid::setColumnStretchFactor(int column, double factor)
    {
        if (column >= 0 && column < columnCount())
        {
            if (factor > 0)
                mColumnStretchFactors[column] = factor;
            else
                qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << factor;
        } else
            qDebug() << Q_FUNC_INFO << "Invalid column:" << column;
    }

/*!
  Sets the stretch \a factors of all columns. \a factors must have the size \ref columnCount.

  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights, regardless of the stretch factor. (see \ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize, \ref
  QCPLayoutElement::setSizeConstraintRect.)

  The default stretch factor of newly created rows/columns is 1.

  \see setColumnStretchFactor, setRowStretchFactors
*/
    void QCPLayoutGrid::setColumnStretchFactors(const QList<double> &factors)
    {
        if (factors.size() == mColumnStretchFactors.size())
        {
            mColumnStretchFactors = factors;
            for (int i=0; i<mColumnStretchFactors.size(); ++i)
            {
                if (mColumnStretchFactors.at(i) <= 0)
                {
                    qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << mColumnStretchFactors.at(i);
                    mColumnStretchFactors[i] = 1;
                }
            }
        } else
            qDebug() << Q_FUNC_INFO << "Column count not equal to passed stretch factor count:" << factors;
    }

/*!
  Sets the stretch \a factor of \a row.

  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights, regardless of the stretch factor. (see \ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize, \ref
  QCPLayoutElement::setSizeConstraintRect.)

  The default stretch factor of newly created rows/columns is 1.

  \see setColumnStretchFactors, setRowStretchFactor
*/
    void QCPLayoutGrid::setRowStretchFactor(int row, double factor)
    {
        if (row >= 0 && row < rowCount())
        {
            if (factor > 0)
                mRowStretchFactors[row] = factor;
            else
                qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << factor;
        } else
            qDebug() << Q_FUNC_INFO << "Invalid row:" << row;
    }

/*!
  Sets the stretch \a factors of all rows. \a factors must have the size \ref rowCount.

  Stretch factors control the relative sizes of rows and columns. Cells will not be resized beyond
  their minimum and maximum widths/heights, regardless of the stretch factor. (see \ref
  QCPLayoutElement::setMinimumSize, \ref QCPLayoutElement::setMaximumSize, \ref
  QCPLayoutElement::setSizeConstraintRect.)

  The default stretch factor of newly created rows/columns is 1.

  \see setRowStretchFactor, setColumnStretchFactors
*/
    void QCPLayoutGrid::setRowStretchFactors(const QList<double> &factors)
    {
        if (factors.size() == mRowStretchFactors.size())
        {
            mRowStretchFactors = factors;
            for (int i=0; i<mRowStretchFactors.size(); ++i)
            {
                if (mRowStretchFactors.at(i) <= 0)
                {
                    qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << mRowStretchFactors.at(i);
                    mRowStretchFactors[i] = 1;
                }
            }
        } else
            qDebug() << Q_FUNC_INFO << "Row count not equal to passed stretch factor count:" << factors;
    }

/*!
  Sets the gap that is left blank between columns to \a pixels.

  \see setRowSpacing
*/
    void QCPLayoutGrid::setColumnSpacing(int pixels)
    {
        mColumnSpacing = pixels;
    }

/*!
  Sets the gap that is left blank between rows to \a pixels.

  \see setColumnSpacing
*/
    void QCPLayoutGrid::setRowSpacing(int pixels)
    {
        mRowSpacing = pixels;
    }

/*!
  Sets the maximum number of columns or rows that are used, before new elements added with \ref
  addElement(QCPLayoutElement*) will start to fill the next row or column, respectively. It depends
  on \ref setFillOrder, whether rows or columns are wrapped.

  If \a count is set to zero, no wrapping will ever occur.

  If you wish to re-wrap the elements currently in the layout, call \ref setFillOrder with \a
  rearrange set to true (the actual fill order doesn't need to be changed for the rearranging to be
  done).

  Note that the method \ref addElement(int row, int column, QCPLayoutElement *element) with
  explicitly stated row and column is not subject to wrapping and can place elements even beyond
  the specified wrapping point.

  \see setFillOrder
*/
    void QCPLayoutGrid::setWrap(int count)
    {
        mWrap = qMax(0, count);
    }

/*!
  Sets the filling order and wrapping behaviour that is used when adding new elements with the
  method \ref addElement(QCPLayoutElement*).

  The specified \a order defines whether rows or columns are filled first. Using \ref setWrap, you
  can control at which row/column count wrapping into the next column/row will occur. If you set it
  to zero, no wrapping will ever occur. Changing the fill order also changes the meaning of the
  linear index used e.g. in \ref elementAt and \ref takeAt. The default fill order for \ref
  QCPLayoutGrid is \ref foColumnsFirst.

  If you want to have all current elements arranged in the new order, set \a rearrange to true. The
  elements will be rearranged in a way that tries to preserve their linear index. However, empty
  cells are skipped during build-up of the new cell order, which shifts the succeeding element's
  index. The rearranging is performed even if the specified \a order is already the current fill
  order. Thus this method can be used to re-wrap the current elements.

  If \a rearrange is false, the current element arrangement is not changed, which means the
  linear indexes change (because the linear index is dependent on the fill order).

  Note that the method \ref addElement(int row, int column, QCPLayoutElement *element) with
  explicitly stated row and column is not subject to wrapping and can place elements even beyond
  the specified wrapping point.

  \see setWrap, addElement(QCPLayoutElement*)
*/
    void QCPLayoutGrid::setFillOrder(FillOrder order, bool rearrange)
    {
// if rearranging, take all elements via linear index of old fill order:
        const int elCount = elementCount();
        QVector<QCPLayoutElement*> tempElements;
        if (rearrange)
        {
            tempElements.reserve(elCount);
            for (int i=0; i<elCount; ++i)
            {
                if (elementAt(i))
                    tempElements.append(takeAt(i));
            }
            simplify();
        }
// change fill order as requested:
        mFillOrder = order;
// if rearranging, re-insert via linear index according to new fill order:
        if (rearrange)
        {
            foreach (QCPLayoutElement *tempElement, tempElements)
            addElement(tempElement);
        }
    }

/*!
  Expands the layout to have \a newRowCount rows and \a newColumnCount columns. So the last valid
  row index will be \a newRowCount-1, the last valid column index will be \a newColumnCount-1.

  If the current column/row count is already larger or equal to \a newColumnCount/\a newRowCount,
  this function does nothing in that dimension.

  Newly created cells are empty, new rows and columns have the stretch factor 1.

  Note that upon a call to \ref addElement, the layout is expanded automatically to contain the
  specified row and column, using this function.

  \see simplify
*/
    void QCPLayoutGrid::expandTo(int newRowCount, int newColumnCount)
    {
// add rows as necessary:
        while (rowCount() < newRowCount)
        {
            mElements.append(QList<QCPLayoutElement*>());
            mRowStretchFactors.append(1);
        }
// go through rows and expand columns as necessary:
        int newColCount = qMax(columnCount(), newColumnCount);
        for (int i=0; i<rowCount(); ++i)
        {
            while (mElements.at(i).size() < newColCount)
                mElements[i].append(nullptr);
        }
        while (mColumnStretchFactors.size() < newColCount)
            mColumnStretchFactors.append(1);
    }

/*!
  Inserts a new row with empty cells at the row index \a newIndex. Valid values for \a newIndex
  range from 0 (inserts a row at the top) to \a rowCount (appends a row at the bottom).

  \see insertColumn
*/
    void QCPLayoutGrid::insertRow(int newIndex)
    {
        if (mElements.isEmpty() || mElements.first().isEmpty()) // if grid is completely empty, add first cell
        {
            expandTo(1, 1);
            return;
        }

        if (newIndex < 0)
            newIndex = 0;
        if (newIndex > rowCount())
            newIndex = rowCount();

        mRowStretchFactors.insert(newIndex, 1);
        QList<QCPLayoutElement*> newRow;
        for (int col=0; col<columnCount(); ++col)
            newRow.append(nullptr);
        mElements.insert(newIndex, newRow);
    }

/*!
  Inserts a new column with empty cells at the column index \a newIndex. Valid values for \a
  newIndex range from 0 (inserts a column at the left) to \a columnCount (appends a column at the
  right).

  \see insertRow
*/
    void QCPLayoutGrid::insertColumn(int newIndex)
    {
        if (mElements.isEmpty() || mElements.first().isEmpty()) // if grid is completely empty, add first cell
        {
            expandTo(1, 1);
            return;
        }

        if (newIndex < 0)
            newIndex = 0;
        if (newIndex > columnCount())
            newIndex = columnCount();

        mColumnStretchFactors.insert(newIndex, 1);
        for (int row=0; row<rowCount(); ++row)
            mElements[row].insert(newIndex, nullptr);
    }

/*!
  Converts the given \a row and \a column to the linear index used by some methods of \ref
  QCPLayoutGrid and \ref QCPLayout.

  The way the cells are indexed depends on \ref setFillOrder. If it is \ref foRowsFirst, the
  indices increase left to right and then top to bottom. If it is \ref foColumnsFirst, the indices
  increase top to bottom and then left to right.

  For the returned index to be valid, \a row and \a column must be valid indices themselves, i.e.
  greater or equal to zero and smaller than the current \ref rowCount/\ref columnCount.

  \see indexToRowCol
*/
    int QCPLayoutGrid::rowColToIndex(int row, int column) const
    {
        if (row >= 0 && row < rowCount())
        {
            if (column >= 0 && column < columnCount())
            {
                switch (mFillOrder)
                {
                    case foRowsFirst: return column*rowCount() + row;
                    case foColumnsFirst: return row*columnCount() + column;
                }
            } else
                qDebug() << Q_FUNC_INFO << "row index out of bounds:" << row;
        } else
            qDebug() << Q_FUNC_INFO << "column index out of bounds:" << column;
        return 0;
    }

/*!
  Converts the linear index to row and column indices and writes the result to \a row and \a
  column.

  The way the cells are indexed depends on \ref setFillOrder. If it is \ref foRowsFirst, the
  indices increase left to right and then top to bottom. If it is \ref foColumnsFirst, the indices
  increase top to bottom and then left to right.

  If there are no cells (i.e. column or row count is zero), sets \a row and \a column to -1.

  For the retrieved \a row and \a column to be valid, the passed \a index must be valid itself,
  i.e. greater or equal to zero and smaller than the current \ref elementCount.

  \see rowColToIndex
*/
    void QCPLayoutGrid::indexToRowCol(int index, int &row, int &column) const
    {
        row = -1;
        column = -1;
        const int nCols = columnCount();
        const int nRows = rowCount();
        if (nCols == 0 || nRows == 0)
            return;
        if (index < 0 || index >= elementCount())
        {
            qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
            return;
        }

        switch (mFillOrder)
        {
            case foRowsFirst:
            {
                column = index / nRows;
                row = index % nRows;
                break;
            }
            case foColumnsFirst:
            {
                row = index / nCols;
                column = index % nCols;
                break;
            }
        }
    }

/* inherits documentation from base class */
    void QCPLayoutGrid::updateLayout()
    {
        QVector<int> minColWidths, minRowHeights, maxColWidths, maxRowHeights;
        getMinimumRowColSizes(&minColWidths, &minRowHeights);
        getMaximumRowColSizes(&maxColWidths, &maxRowHeights);

        int totalRowSpacing = (rowCount()-1) * mRowSpacing;
        int totalColSpacing = (columnCount()-1) * mColumnSpacing;
        QVector<int> colWidths = getSectionSizes(maxColWidths, minColWidths, mColumnStretchFactors.toVector(), mRect.width()-totalColSpacing);
        QVector<int> rowHeights = getSectionSizes(maxRowHeights, minRowHeights, mRowStretchFactors.toVector(), mRect.height()-totalRowSpacing);

// go through cells and set rects accordingly:
        int yOffset = mRect.top();
        for (int row=0; row<rowCount(); ++row)
        {
            if (row > 0)
                yOffset += rowHeights.at(row-1)+mRowSpacing;
            int xOffset = mRect.left();
            for (int col=0; col<columnCount(); ++col)
            {
                if (col > 0)
                    xOffset += colWidths.at(col-1)+mColumnSpacing;
                if (mElements.at(row).at(col))
                    mElements.at(row).at(col)->setOuterRect(QRect(xOffset, yOffset, colWidths.at(col), rowHeights.at(row)));
            }
        }
    }

/*!
  \seebaseclassmethod

  Note that the association of the linear \a index to the row/column based cells depends on the
  current setting of \ref setFillOrder.

  \see rowColToIndex
*/
    QCPLayoutElement *QCPLayoutGrid::elementAt(int index) const
    {
        if (index >= 0 && index < elementCount())
        {
            int row, col;
            indexToRowCol(index, row, col);
            return mElements.at(row).at(col);
        } else
            return nullptr;
    }

/*!
  \seebaseclassmethod

  Note that the association of the linear \a index to the row/column based cells depends on the
  current setting of \ref setFillOrder.

  \see rowColToIndex
*/
    QCPLayoutElement *QCPLayoutGrid::takeAt(int index)
    {
        if (QCPLayoutElement *el = elementAt(index))
        {
            releaseElement(el);
            int row, col;
            indexToRowCol(index, row, col);
            mElements[row][col] = nullptr;
            return el;
        } else
        {
            qDebug() << Q_FUNC_INFO << "Attempt to take invalid index:" << index;
            return nullptr;
        }
    }

/* inherits documentation from base class */
    bool QCPLayoutGrid::take(QCPLayoutElement *element)
    {
        if (element)
        {
            for (int i=0; i<elementCount(); ++i)
            {
                if (elementAt(i) == element)
                {
                    takeAt(i);
                    return true;
                }
            }
            qDebug() << Q_FUNC_INFO << "Element not in this layout, couldn't take";
        } else
            qDebug() << Q_FUNC_INFO << "Can't take nullptr element";
        return false;
    }

/* inherits documentation from base class */
    QList<QCPLayoutElement*> QCPLayoutGrid::elements(bool recursive) const
    {
        QList<QCPLayoutElement*> result;
        const int elCount = elementCount();
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
        result.reserve(elCount);
#endif
        for (int i=0; i<elCount; ++i)
            result.append(elementAt(i));
        if (recursive)
        {
            for (int i=0; i<elCount; ++i)
            {
                if (result.at(i))
                    result << result.at(i)->elements(recursive);
            }
        }
        return result;
    }

/*!
  Simplifies the layout by collapsing rows and columns which only contain empty cells.
*/
    void QCPLayoutGrid::simplify()
    {
// remove rows with only empty cells:
        for (int row=rowCount()-1; row>=0; --row)
        {
            bool hasElements = false;
            for (int col=0; col<columnCount(); ++col)
            {
                if (mElements.at(row).at(col))
                {
                    hasElements = true;
                    break;
                }
            }
            if (!hasElements)
            {
                mRowStretchFactors.removeAt(row);
                mElements.removeAt(row);
                if (mElements.isEmpty()) // removed last element, also remove stretch factor (wouldn't happen below because also columnCount changed to 0 now)
                    mColumnStretchFactors.clear();
            }
        }

// remove columns with only empty cells:
        for (int col=columnCount()-1; col>=0; --col)
        {
            bool hasElements = false;
            for (int row=0; row<rowCount(); ++row)
            {
                if (mElements.at(row).at(col))
                {
                    hasElements = true;
                    break;
                }
            }
            if (!hasElements)
            {
                mColumnStretchFactors.removeAt(col);
                for (int row=0; row<rowCount(); ++row)
                    mElements[row].removeAt(col);
            }
        }
    }

/* inherits documentation from base class */
    QSize QCPLayoutGrid::minimumOuterSizeHint() const
    {
        QVector<int> minColWidths, minRowHeights;
        getMinimumRowColSizes(&minColWidths, &minRowHeights);
        QSize result(0, 0);
        foreach (int w, minColWidths)
        result.rwidth() += w;
        foreach (int h, minRowHeights)
        result.rheight() += h;
        result.rwidth() += qMax(0, columnCount()-1) * mColumnSpacing;
        result.rheight() += qMax(0, rowCount()-1) * mRowSpacing;
        result.rwidth() += mMargins.left()+mMargins.right();
        result.rheight() += mMargins.top()+mMargins.bottom();
        return result;
    }

/* inherits documentation from base class */
    QSize QCPLayoutGrid::maximumOuterSizeHint() const
    {
        QVector<int> maxColWidths, maxRowHeights;
        getMaximumRowColSizes(&maxColWidths, &maxRowHeights);

        QSize result(0, 0);
        foreach (int w, maxColWidths)
        result.setWidth(qMin(result.width()+w, QWIDGETSIZE_MAX));
        foreach (int h, maxRowHeights)
        result.setHeight(qMin(result.height()+h, QWIDGETSIZE_MAX));
        result.rwidth() += qMax(0, columnCount()-1) * mColumnSpacing;
        result.rheight() += qMax(0, rowCount()-1) * mRowSpacing;
        result.rwidth() += mMargins.left()+mMargins.right();
        result.rheight() += mMargins.top()+mMargins.bottom();
        if (result.height() > QWIDGETSIZE_MAX)
            result.setHeight(QWIDGETSIZE_MAX);
        if (result.width() > QWIDGETSIZE_MAX)
            result.setWidth(QWIDGETSIZE_MAX);
        return result;
    }

/*! \internal

  Places the minimum column widths and row heights into \a minColWidths and \a minRowHeights
  respectively.

  The minimum height of a row is the largest minimum height of any element's outer rect in that
  row. The minimum width of a column is the largest minimum width of any element's outer rect in
  that column.

  This is a helper function for \ref updateLayout.

  \see getMaximumRowColSizes
*/
    void QCPLayoutGrid::getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const
    {
        *minColWidths = QVector<int>(columnCount(), 0);
        *minRowHeights = QVector<int>(rowCount(), 0);
        for (int row=0; row<rowCount(); ++row)
        {
            for (int col=0; col<columnCount(); ++col)
            {
                if (QCPLayoutElement *el = mElements.at(row).at(col))
                {
                    QSize minSize = getFinalMinimumOuterSize(el);
                    if (minColWidths->at(col) < minSize.width())
                        (*minColWidths)[col] = minSize.width();
                    if (minRowHeights->at(row) < minSize.height())
                        (*minRowHeights)[row] = minSize.height();
                }
            }
        }
    }

/*! \internal

  Places the maximum column widths and row heights into \a maxColWidths and \a maxRowHeights
  respectively.

  The maximum height of a row is the smallest maximum height of any element's outer rect in that
  row. The maximum width of a column is the smallest maximum width of any element's outer rect in
  that column.

  This is a helper function for \ref updateLayout.

  \see getMinimumRowColSizes
*/
    void QCPLayoutGrid::getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const
    {
        *maxColWidths = QVector<int>(columnCount(), QWIDGETSIZE_MAX);
        *maxRowHeights = QVector<int>(rowCount(), QWIDGETSIZE_MAX);
        for (int row=0; row<rowCount(); ++row)
        {
            for (int col=0; col<columnCount(); ++col)
            {
                if (QCPLayoutElement *el = mElements.at(row).at(col))
                {
                    QSize maxSize = getFinalMaximumOuterSize(el);
                    if (maxColWidths->at(col) > maxSize.width())
                        (*maxColWidths)[col] = maxSize.width();
                    if (maxRowHeights->at(row) > maxSize.height())
                        (*maxRowHeights)[row] = maxSize.height();
                }
            }
        }
    }

} // QCP