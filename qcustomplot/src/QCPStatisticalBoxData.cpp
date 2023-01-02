#include "include/QCPStatisticalBoxData.hpp"

namespace QCP {
/*! \class QCPStatisticalBoxData
  \brief Holds the data of one single data point for QCPStatisticalBox.

  The stored data is:

  \li \a key: coordinate on the key axis of this data point (this is the \a mainKey and the \a sortKey)

  \li \a minimum: the position of the lower whisker, typically the minimum measurement of the
  sample that's not considered an outlier.

  \li \a lowerQuartile: the lower end of the box. The lower and the upper quartiles are the two
  statistical quartiles around the median of the sample, they should contain 50% of the sample
  data.

  \li \a median: the value of the median mark inside the quartile box. The median separates the
  sample data in half (50% of the sample data is below/above the median). (This is the \a mainValue)

  \li \a upperQuartile: the upper end of the box. The lower and the upper quartiles are the two
  statistical quartiles around the median of the sample, they should contain 50% of the sample
  data.

  \li \a maximum: the position of the upper whisker, typically the maximum measurement of the
  sample that's not considered an outlier.

  \li \a outliers: a QVector of outlier values that will be drawn as scatter points at the \a key
  coordinate of this data point (see \ref QCPStatisticalBox::setOutlierStyle)

  The container for storing multiple data points is \ref QCPStatisticalBoxDataContainer. It is a
  typedef for \ref QCPDataContainer with \ref QCPStatisticalBoxData as the DataType template
  parameter. See the documentation there for an explanation regarding the data type's generic
  methods.

  \see QCPStatisticalBoxDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPStatisticalBoxData::sortKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPStatisticalBoxData QCPStatisticalBoxData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey. All other members are set to zero.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPStatisticalBoxData::sortKeyIsMainKey()

  Since the member \a key is both the data point key coordinate and the data ordering parameter,
  this method returns true.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPStatisticalBoxData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPStatisticalBoxData::mainValue() const

  Returns the \a median member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPStatisticalBoxData::valueRange() const

  Returns a QCPRange spanning from the \a minimum to the \a maximum member of this statistical box
  data point, possibly further expanded by outliers.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a data point with key and all values set to zero.
*/
    QCPStatisticalBoxData::QCPStatisticalBoxData() :
            key(0),
            minimum(0),
            lowerQuartile(0),
            median(0),
            upperQuartile(0),
            maximum(0)
    {
    }

/*!
  Constructs a data point with the specified \a key, \a minimum, \a lowerQuartile, \a median, \a
  upperQuartile, \a maximum and optionally a number of \a outliers.
*/
    QCPStatisticalBoxData::QCPStatisticalBoxData(double key, double minimum, double lowerQuartile, double median, double upperQuartile, double maximum, const QVector<double> &outliers) :
            key(key),
            minimum(minimum),
            lowerQuartile(lowerQuartile),
            median(median),
            upperQuartile(upperQuartile),
            maximum(maximum),
            outliers(outliers)
    {
    }


} // QCP