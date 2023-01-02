#include "include/QCPBarsData.hpp"

namespace QCP {
/*! \class QCPBarsData
  \brief Holds the data of one single data point (one bar) for QCPBars.

  The stored data is:
  \li \a key: coordinate on the key axis of this bar (this is the \a mainKey and the \a sortKey)
  \li \a value: height coordinate on the value axis of this bar (this is the \a mainValue)

  The container for storing multiple data points is \ref QCPBarsDataContainer. It is a typedef for
  \ref QCPDataContainer with \ref QCPBarsData as the DataType template parameter. See the
  documentation there for an explanation regarding the data type's generic methods.

  \see QCPBarsDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPBarsData::sortKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPBarsData QCPBarsData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey. All other members are set to zero.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPBarsData::sortKeyIsMainKey()

  Since the member \a key is both the data point key coordinate and the data ordering parameter,
  this method returns true.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPBarsData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPBarsData::mainValue() const

  Returns the \a value member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPBarsData::valueRange() const

  Returns a QCPRange with both lower and upper boundary set to \a value of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a bar data point with key and value set to zero.
*/
    QCPBarsData::QCPBarsData() :
            key(0),
            value(0)
    {
    }

/*!
  Constructs a bar data point with the specified \a key and \a value.
*/
    QCPBarsData::QCPBarsData(double key, double value) :
            key(key),
            value(value)
    {
    }


} // QCP