#include "include/QCPFinancialData.hpp"

namespace QCP {
/*! \class QCPFinancialData
  \brief Holds the data of one single data point for QCPFinancial.

  The stored data is:
  \li \a key: coordinate on the key axis of this data point (this is the \a mainKey and the \a sortKey)
  \li \a open: The opening value at the data point (this is the \a mainValue)
  \li \a high: The high/maximum value at the data point
  \li \a low: The low/minimum value at the data point
  \li \a close: The closing value at the data point

  The container for storing multiple data points is \ref QCPFinancialDataContainer. It is a typedef
  for \ref QCPDataContainer with \ref QCPFinancialData as the DataType template parameter. See the
  documentation there for an explanation regarding the data type's generic methods.

  \see QCPFinancialDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPFinancialData::sortKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPFinancialData QCPFinancialData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey. All other members are set to zero.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPFinancialData::sortKeyIsMainKey()

  Since the member \a key is both the data point key coordinate and the data ordering parameter,
  this method returns true.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPFinancialData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPFinancialData::mainValue() const

  Returns the \a open member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPFinancialData::valueRange() const

  Returns a QCPRange spanning from the \a low to the \a high value of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a data point with key and all values set to zero.
*/
    QCPFinancialData::QCPFinancialData() :
            key(0),
            open(0),
            high(0),
            low(0),
            close(0)
    {
    }

/*!
  Constructs a data point with the specified \a key and OHLC values.
*/
    QCPFinancialData::QCPFinancialData(double key, double open, double high, double low, double close) :
            key(key),
            open(open),
            high(high),
            low(low),
            close(close)
    {
    }


} // QCP