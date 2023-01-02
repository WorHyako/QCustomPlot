#include "include/QCPGraphData.hpp"

namespace QCP {
/*! \class QCPGraphData
  \brief Holds the data of one single data point for QCPGraph.

  The stored data is:
  \li \a key: coordinate on the key axis of this data point (this is the \a mainKey and the \a sortKey)
  \li \a value: coordinate on the value axis of this data point (this is the \a mainValue)

  The container for storing multiple data points is \ref QCPGraphDataContainer. It is a typedef for
  \ref QCPDataContainer with \ref QCPGraphData as the DataType template parameter. See the
  documentation there for an explanation regarding the data type's generic methods.

  \see QCPGraphDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPGraphData::sortKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPGraphData QCPGraphData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey. All other members are set to zero.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPGraphData::sortKeyIsMainKey()

  Since the member \a key is both the data point key coordinate and the data ordering parameter,
  this method returns true.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPGraphData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPGraphData::mainValue() const

  Returns the \a value member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPGraphData::valueRange() const

  Returns a QCPRange with both lower and upper boundary set to \a value of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a data point with key and value set to zero.
*/
    QCPGraphData::QCPGraphData() :
            key(0),
            value(0)
    {
    }

/*!
  Constructs a data point with the specified \a key and \a value.
*/
    QCPGraphData::QCPGraphData(double key, double value) :
            key(key),
            value(value)
    {
    }

} // QCP