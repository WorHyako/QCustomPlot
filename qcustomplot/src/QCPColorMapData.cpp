#include "include/QCPColorMapData.hpp"

namespace QCP {
/*! \class QCPColorMapData
  \brief Holds the two-dimensional data of a QCPColorMap plottable.

  This class is a data storage for \ref QCPColorMap. It holds a two-dimensional array, which \ref
  QCPColorMap then displays as a 2D image in the plot, where the array values are represented by a
  color, depending on the value.

  The size of the array can be controlled via \ref setSize (or \ref setKeySize, \ref setValueSize).
  Which plot coordinates these cells correspond to can be configured with \ref setRange (or \ref
  setKeyRange, \ref setValueRange).

  The data cells can be accessed in two ways: They can be directly addressed by an integer index
  with \ref setCell. This is the fastest method. Alternatively, they can be addressed by their plot
  coordinate with \ref setData. plot coordinate to cell index transformations and vice versa are
  provided by the functions \ref coordToCell and \ref cellToCoord.

  A \ref QCPColorMapData also holds an on-demand two-dimensional array of alpha values which (if
  allocated) has the same size as the data map. It can be accessed via \ref setAlpha, \ref
  fillAlpha and \ref clearAlpha. The memory for the alpha map is only allocated if needed, i.e. on
  the first call of \ref setAlpha. \ref clearAlpha restores full opacity and frees the alpha map.

  This class also buffers the minimum and maximum values that are in the data set, to provide
  QCPColorMap::rescaleDataRange with the necessary information quickly. Setting a cell to a value
  that is greater than the current maximum increases this maximum to the new value. However,
  setting the cell that currently holds the maximum value to a smaller value doesn't decrease the
  maximum again, because finding the true new maximum would require going through the entire data
  array, which might be time consuming. The same holds for the data minimum. This functionality is
  given by \ref recalculateDataBounds, such that you can decide when it is sensible to find the
  true current minimum and maximum. The method QCPColorMap::rescaleDataRange offers a convenience
  parameter \a recalculateDataBounds which may be set to true to automatically call \ref
  recalculateDataBounds internally.
*/

/* start of documentation of inline functions */

/*! \fn bool QCPColorMapData::isEmpty() const

  Returns whether this instance carries no data. This is equivalent to having a size where at least
  one of the dimensions is 0 (see \ref setSize).
*/

/* end of documentation of inline functions */

/*!
  Constructs a new QCPColorMapData instance. The instance has \a keySize cells in the key direction
  and \a valueSize cells in the value direction. These cells will be displayed by the \ref QCPColorMap
  at the coordinates \a keyRange and \a valueRange.

  \see setSize, setKeySize, setValueSize, setRange, setKeyRange, setValueRange
*/
    QCPColorMapData::QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange) :
            mKeySize(0),
            mValueSize(0),
            mKeyRange(keyRange),
            mValueRange(valueRange),
            mIsEmpty(true),
            mData(nullptr),
            mAlpha(nullptr),
            mDataModified(true)
    {
        setSize(keySize, valueSize);
        fill(0);
    }

    QCPColorMapData::~QCPColorMapData()
    {
        delete[] mData;
        delete[] mAlpha;
    }

/*!
  Constructs a new QCPColorMapData instance copying the data and range of \a other.
*/
    QCPColorMapData::QCPColorMapData(const QCPColorMapData &other) :
            mKeySize(0),
            mValueSize(0),
            mIsEmpty(true),
            mData(nullptr),
            mAlpha(nullptr),
            mDataModified(true)
    {
        *this = other;
    }

/*!
  Overwrites this color map data instance with the data stored in \a other. The alpha map state is
  transferred, too.
*/
    QCPColorMapData &QCPColorMapData::operator=(const QCPColorMapData &other)
    {
        if (&other != this)
        {
            const int keySize = other.keySize();
            const int valueSize = other.valueSize();
            if (!other.mAlpha && mAlpha)
                clearAlpha();
            setSize(keySize, valueSize);
            if (other.mAlpha && !mAlpha)
                createAlpha(false);
            setRange(other.keyRange(), other.valueRange());
            if (!isEmpty())
            {
                memcpy(mData, other.mData, sizeof(mData[0])*size_t(keySize*valueSize));
                if (mAlpha)
                    memcpy(mAlpha, other.mAlpha, sizeof(mAlpha[0])*size_t(keySize*valueSize));
            }
            mDataBounds = other.mDataBounds;
            mDataModified = true;
        }
        return *this;
    }

/* undocumented getter */
    double QCPColorMapData::data(double key, double value)
    {
        int keyCell = int( (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5 );
        int valueCell = int( (value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower)*(mValueSize-1)+0.5 );
        if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 && valueCell < mValueSize)
            return mData[valueCell*mKeySize + keyCell];
        else
            return 0;
    }

/* undocumented getter */
    double QCPColorMapData::cell(int keyIndex, int valueIndex)
    {
        if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
            return mData[valueIndex*mKeySize + keyIndex];
        else
            return 0;
    }

/*!
  Returns the alpha map value of the cell with the indices \a keyIndex and \a valueIndex.

  If this color map data doesn't have an alpha map (because \ref setAlpha was never called after
  creation or after a call to \ref clearAlpha), returns 255, which corresponds to full opacity.

  \see setAlpha
*/
    unsigned char QCPColorMapData::alpha(int keyIndex, int valueIndex)
    {
        if (mAlpha && keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
            return mAlpha[valueIndex*mKeySize + keyIndex];
        else
            return 255;
    }

/*!
  Resizes the data array to have \a keySize cells in the key dimension and \a valueSize cells in
  the value dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.

  Setting at least one of \a keySize or \a valueSize to zero frees the internal data array and \ref
  isEmpty returns true.

  \see setRange, setKeySize, setValueSize
*/
    void QCPColorMapData::setSize(int keySize, int valueSize)
    {
        if (keySize != mKeySize || valueSize != mValueSize)
        {
            mKeySize = keySize;
            mValueSize = valueSize;
            delete[] mData;
            mIsEmpty = mKeySize == 0 || mValueSize == 0;
            if (!mIsEmpty)
            {
#ifdef __EXCEPTIONS
                try { // 2D arrays get memory intensive fast. So if the allocation fails, at least output debug message
#endif
                mData = new double[size_t(mKeySize*mValueSize)];
#ifdef __EXCEPTIONS
                } catch (...) { mData = nullptr; }
#endif
                if (mData)
                    fill(0);
                else
                    qDebug() << Q_FUNC_INFO << "out of memory for data dimensions "<< mKeySize << "*" << mValueSize;
            } else
                mData = nullptr;

            if (mAlpha) // if we had an alpha map, recreate it with new size
                createAlpha();

            mDataModified = true;
        }
    }

/*!
  Resizes the data array to have \a keySize cells in the key dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.

  Setting \a keySize to zero frees the internal data array and \ref isEmpty returns true.

  \see setKeyRange, setSize, setValueSize
*/
    void QCPColorMapData::setKeySize(int keySize)
    {
        setSize(keySize, mValueSize);
    }

/*!
  Resizes the data array to have \a valueSize cells in the value dimension.

  The current data is discarded and the map cells are set to 0, unless the map had already the
  requested size.

  Setting \a valueSize to zero frees the internal data array and \ref isEmpty returns true.

  \see setValueRange, setSize, setKeySize
*/
    void QCPColorMapData::setValueSize(int valueSize)
    {
        setSize(mKeySize, valueSize);
    }

/*!
  Sets the coordinate ranges the data shall be distributed over. This defines the rectangular area
  covered by the color map in plot coordinates.

  The outer cells will be centered on the range boundaries given to this function. For example, if
  the key size (\ref setKeySize) is 3 and \a keyRange is set to <tt>QCPRange(2, 3)</tt> there will
  be cells centered on the key coordinates 2, 2.5 and 3.

  \see setSize
*/
    void QCPColorMapData::setRange(const QCPRange &keyRange, const QCPRange &valueRange)
    {
        setKeyRange(keyRange);
        setValueRange(valueRange);
    }

/*!
  Sets the coordinate range the data shall be distributed over in the key dimension. Together with
  the value range, This defines the rectangular area covered by the color map in plot coordinates.

  The outer cells will be centered on the range boundaries given to this function. For example, if
  the key size (\ref setKeySize) is 3 and \a keyRange is set to <tt>QCPRange(2, 3)</tt> there will
  be cells centered on the key coordinates 2, 2.5 and 3.

  \see setRange, setValueRange, setSize
*/
    void QCPColorMapData::setKeyRange(const QCPRange &keyRange)
    {
        mKeyRange = keyRange;
    }

/*!
  Sets the coordinate range the data shall be distributed over in the value dimension. Together with
  the key range, This defines the rectangular area covered by the color map in plot coordinates.

  The outer cells will be centered on the range boundaries given to this function. For example, if
  the value size (\ref setValueSize) is 3 and \a valueRange is set to <tt>QCPRange(2, 3)</tt> there
  will be cells centered on the value coordinates 2, 2.5 and 3.

  \see setRange, setKeyRange, setSize
*/
    void QCPColorMapData::setValueRange(const QCPRange &valueRange)
    {
        mValueRange = valueRange;
    }

/*!
  Sets the data of the cell, which lies at the plot coordinates given by \a key and \a value, to \a
  z.

  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::setData method as it uses a linear transformation to
  determine the cell index. Rather directly access the cell index with \ref
  QCPColorMapData::setCell.

  \see setCell, setRange
*/
    void QCPColorMapData::setData(double key, double value, double z)
    {
        int keyCell = int( (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5 );
        int valueCell = int( (value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower)*(mValueSize-1)+0.5 );
        if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 && valueCell < mValueSize)
        {
            mData[valueCell*mKeySize + keyCell] = z;
            if (z < mDataBounds.lower)
                mDataBounds.lower = z;
            if (z > mDataBounds.upper)
                mDataBounds.upper = z;
            mDataModified = true;
        }
    }

/*!
  Sets the data of the cell with indices \a keyIndex and \a valueIndex to \a z. The indices
  enumerate the cells starting from zero, up to the map's size-1 in the respective dimension (see
  \ref setSize).

  In the standard plot configuration (horizontal key axis and vertical value axis, both not
  range-reversed), the cell with indices (0, 0) is in the bottom left corner and the cell with
  indices (keySize-1, valueSize-1) is in the top right corner of the color map.

  \see setData, setSize
*/
    void QCPColorMapData::setCell(int keyIndex, int valueIndex, double z)
    {
        if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
        {
            mData[valueIndex*mKeySize + keyIndex] = z;
            if (z < mDataBounds.lower)
                mDataBounds.lower = z;
            if (z > mDataBounds.upper)
                mDataBounds.upper = z;
            mDataModified = true;
        } else
            qDebug() << Q_FUNC_INFO << "index out of bounds:" << keyIndex << valueIndex;
    }

/*!
  Sets the alpha of the color map cell given by \a keyIndex and \a valueIndex to \a alpha. A value
  of 0 for \a alpha results in a fully transparent cell, and a value of 255 results in a fully
  opaque cell.

  If an alpha map doesn't exist yet for this color map data, it will be created here. If you wish
  to restore full opacity and free any allocated memory of the alpha map, call \ref clearAlpha.

  Note that the cell-wise alpha which can be configured here is independent of any alpha configured
  in the color map's gradient (\ref QCPColorGradient). If a cell is affected both by the cell-wise
  and gradient alpha, the alpha values will be blended accordingly during rendering of the color
  map.

  \see fillAlpha, clearAlpha
*/
    void QCPColorMapData::setAlpha(int keyIndex, int valueIndex, unsigned char alpha)
    {
        if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
        {
            if (mAlpha || createAlpha())
            {
                mAlpha[valueIndex*mKeySize + keyIndex] = alpha;
                mDataModified = true;
            }
        } else
            qDebug() << Q_FUNC_INFO << "index out of bounds:" << keyIndex << valueIndex;
    }

/*!
  Goes through the data and updates the buffered minimum and maximum data values.

  Calling this method is only advised if you are about to call \ref QCPColorMap::rescaleDataRange
  and can not guarantee that the cells holding the maximum or minimum data haven't been overwritten
  with a smaller or larger value respectively, since the buffered maximum/minimum values have been
  updated the last time. Why this is the case is explained in the class description (\ref
  QCPColorMapData).

  Note that the method \ref QCPColorMap::rescaleDataRange provides a parameter \a
  recalculateDataBounds for convenience. Setting this to true will call this method for you, before
  doing the rescale.
*/
    void QCPColorMapData::recalculateDataBounds()
    {
        if (mKeySize > 0 && mValueSize > 0)
        {
            double minHeight = std::numeric_limits<double>::max();
            double maxHeight = -std::numeric_limits<double>::max();
            const int dataCount = mValueSize*mKeySize;
            for (int i=0; i<dataCount; ++i)
            {
                if (mData[i] > maxHeight)
                    maxHeight = mData[i];
                if (mData[i] < minHeight)
                    minHeight = mData[i];
            }
            mDataBounds.lower = minHeight;
            mDataBounds.upper = maxHeight;
        }
    }

/*!
  Frees the internal data memory.

  This is equivalent to calling \ref setSize "setSize(0, 0)".
*/
    void QCPColorMapData::clear()
    {
        setSize(0, 0);
    }

/*!
  Frees the internal alpha map. The color map will have full opacity again.
*/
    void QCPColorMapData::clearAlpha()
    {
        if (mAlpha)
        {
            delete[] mAlpha;
            mAlpha = nullptr;
            mDataModified = true;
        }
    }

/*!
  Sets all cells to the value \a z.
*/
    void QCPColorMapData::fill(double z)
    {
        const int dataCount = mValueSize*mKeySize;
        memset(mData, z, dataCount*sizeof(*mData));
        mDataBounds = QCPRange(z, z);
        mDataModified = true;
    }

/*!
  Sets the opacity of all color map cells to \a alpha. A value of 0 for \a alpha results in a fully
  transparent color map, and a value of 255 results in a fully opaque color map.

  If you wish to restore opacity to 100% and free any used memory for the alpha map, rather use
  \ref clearAlpha.

  \see setAlpha
*/
    void QCPColorMapData::fillAlpha(unsigned char alpha)
    {
        if (mAlpha || createAlpha(false))
        {
            const int dataCount = mValueSize*mKeySize;
            memset(mAlpha, alpha, dataCount*sizeof(*mAlpha));
            mDataModified = true;
        }
    }

/*!
  Transforms plot coordinates given by \a key and \a value to cell indices of this QCPColorMapData
  instance. The resulting cell indices are returned via the output parameters \a keyIndex and \a
  valueIndex.

  The retrieved key/value cell indices can then be used for example with \ref setCell.

  If you are only interested in a key or value index, you may pass \c nullptr as \a valueIndex or
  \a keyIndex.

  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::coordToCell method as it uses a linear transformation to
  determine the cell index.

  \see cellToCoord, QCPAxis::coordToPixel
*/
    void QCPColorMapData::coordToCell(double key, double value, int *keyIndex, int *valueIndex) const
    {
        if (keyIndex)
            *keyIndex = int( (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5 );
        if (valueIndex)
            *valueIndex = int( (value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower)*(mValueSize-1)+0.5 );
    }

/*!
  Transforms cell indices given by \a keyIndex and \a valueIndex to cell indices of this QCPColorMapData
  instance. The resulting coordinates are returned via the output parameters \a key and \a
  value.

  If you are only interested in a key or value coordinate, you may pass \c nullptr as \a key or \a
  value.

  \note The QCPColorMap always displays the data at equal key/value intervals, even if the key or
  value axis is set to a logarithmic scaling. If you want to use QCPColorMap with logarithmic axes,
  you shouldn't use the \ref QCPColorMapData::cellToCoord method as it uses a linear transformation to
  determine the cell index.

  \see coordToCell, QCPAxis::pixelToCoord
*/
    void QCPColorMapData::cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const
    {
        if (key)
            *key = keyIndex/double(mKeySize-1)*(mKeyRange.upper-mKeyRange.lower)+mKeyRange.lower;
        if (value)
            *value = valueIndex/double(mValueSize-1)*(mValueRange.upper-mValueRange.lower)+mValueRange.lower;
    }

/*! \internal

  Allocates the internal alpha map with the current data map key/value size and, if \a
  initializeOpaque is true, initializes all values to 255. If \a initializeOpaque is false, the
  values are not initialized at all. In this case, the alpha map should be initialized manually,
  e.g. with \ref fillAlpha.

  If an alpha map exists already, it is deleted first. If this color map is empty (has either key
  or value size zero, see \ref isEmpty), the alpha map is cleared.

  The return value indicates the existence of the alpha map after the call. So this method returns
  true if the data map isn't empty and an alpha map was successfully allocated.
*/
    bool QCPColorMapData::createAlpha(bool initializeOpaque)
    {
        clearAlpha();
        if (isEmpty())
            return false;

#ifdef __EXCEPTIONS
        try { // 2D arrays get memory intensive fast. So if the allocation fails, at least output debug message
#endif
        mAlpha = new unsigned char[size_t(mKeySize*mValueSize)];
#ifdef __EXCEPTIONS
        } catch (...) { mAlpha = nullptr; }
#endif
        if (mAlpha)
        {
            if (initializeOpaque)
                fillAlpha(255);
            return true;
        } else
        {
            qDebug() << Q_FUNC_INFO << "out of memory for data dimensions "<< mKeySize << "*" << mValueSize;
            return false;
        }
    }

} // QCP