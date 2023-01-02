#include "include/QCPDataRange.hpp"

using namespace QCP;


/*! \class QCPDataRange
  \brief Describes a data range given by begin and end index

  QCPDataRange holds two integers describing the begin (\ref setBegin) and end (\ref setEnd) index
  of a contiguous set of data points. The \a end index corresponds to the data point just after the
  last data point of the data range, like in standard iterators.

  Data Ranges are not bound to a certain plottable, thus they can be freely exchanged, created and
  modified. If a non-contiguous data set shall be described, the class \ref QCPDataSelection is
  used, which holds and manages multiple instances of \ref QCPDataRange. In most situations, \ref
  QCPDataSelection is thus used.

  Both \ref QCPDataRange and \ref QCPDataSelection offer convenience methods to work with them,
  e.g. \ref bounded, \ref expanded, \ref intersects, \ref intersection, \ref adjusted, \ref
  contains. Further, addition and subtraction operators (defined in \ref QCPDataSelection) can be
  used to join/subtract data ranges and data selections (or mixtures), to retrieve a corresponding
  \ref QCPDataSelection.

  %QCustomPlot's \ref dataselection "data selection mechanism" is based on \ref QCPDataSelection and
  QCPDataRange.

  \note Do not confuse \ref QCPDataRange with \ref QCPRange. A \ref QCPRange describes an interval
  in floating point plot coordinates, e.g. the current axis range.
*/

/* start documentation of inline functions */

/*! \fn int QCPDataRange::size() const

  Returns the number of data points described by this data range. This is equal to the end index
  minus the begin index.

  \see length
*/

/*! \fn int QCPDataRange::length() const

  Returns the number of data points described by this data range. Equivalent to \ref size.
*/

/*! \fn void QCPDataRange::setBegin(int begin)

  Sets the begin of this data range. The \a begin index points to the first data point that is part
  of the data range.

  No checks or corrections are made to ensure the resulting range is valid (\ref isValid).

  \see setEnd
*/

/*! \fn void QCPDataRange::setEnd(int end)

  Sets the end of this data range. The \a end index points to the data point just after the last
  data point that is part of the data range.

  No checks or corrections are made to ensure the resulting range is valid (\ref isValid).

  \see setBegin
*/

/*! \fn bool QCPDataRange::isValid() const

  Returns whether this range is valid. A valid range has a begin index greater or equal to 0, and
  an end index greater or equal to the begin index.

  \note Invalid ranges should be avoided and are never the result of any of QCustomPlot's methods
  (unless they are themselves fed with invalid ranges). Do not pass invalid ranges to QCustomPlot's
  methods. The invalid range is not inherently prevented in QCPDataRange, to allow temporary
  invalid begin/end values while manipulating the range. An invalid range is not necessarily empty
  (\ref isEmpty), since its \ref length can be negative and thus non-zero.
*/

/*! \fn bool QCPDataRange::isEmpty() const

  Returns whether this range is empty, i.e. whether its begin index equals its end index.

  \see size, length
*/

/*! \fn QCPDataRange QCPDataRange::adjusted(int changeBegin, int changeEnd) const

  Returns a data range where \a changeBegin and \a changeEnd were added to the begin and end
  indices, respectively.
*/

/* end documentation of inline functions */

/*!
  Creates an empty QCPDataRange, with begin and end set to 0.
*/
QCPDataRange::QCPDataRange() :
        mBegin(0),
        mEnd(0)
{
}

/*!
  Creates a QCPDataRange, initialized with the specified \a begin and \a end.

  No checks or corrections are made to ensure the resulting range is valid (\ref isValid).
*/
QCPDataRange::QCPDataRange(int begin, int end) :
        mBegin(begin),
        mEnd(end)
{
}

/*!
  Returns a data range that matches this data range, except that parts exceeding \a other are
  excluded.

  This method is very similar to \ref intersection, with one distinction: If this range and the \a
  other range share no intersection, the returned data range will be empty with begin and end set
  to the respective boundary side of \a other, at which this range is residing. (\ref intersection
  would just return a range with begin and end set to 0.)
*/
QCPDataRange QCPDataRange::bounded(const QCPDataRange &other) const
{
    QCPDataRange result(intersection(other));
    if (result.isEmpty()) // no intersection, preserve respective bounding side of otherRange as both begin and end of return value
    {
        if (mEnd <= other.mBegin)
            result = QCPDataRange(other.mBegin, other.mBegin);
        else
            result = QCPDataRange(other.mEnd, other.mEnd);
    }
    return result;
}

/*!
  Returns a data range that contains both this data range as well as \a other.
*/
QCPDataRange QCPDataRange::expanded(const QCPDataRange &other) const
{
    return {qMin(mBegin, other.mBegin), qMax(mEnd, other.mEnd)};
}

/*!
  Returns the data range which is contained in both this data range and \a other.

  This method is very similar to \ref bounded, with one distinction: If this range and the \a other
  range share no intersection, the returned data range will be empty with begin and end set to 0.
  (\ref bounded would return a range with begin and end set to one of the boundaries of \a other,
  depending on which side this range is on.)

  \see QCPDataSelection::intersection
*/
QCPDataRange QCPDataRange::intersection(const QCPDataRange &other) const
{
    QCPDataRange result(qMax(mBegin, other.mBegin), qMin(mEnd, other.mEnd));
    if (result.isValid())
        return result;
    else
        return {};
}

/*!
  Returns whether this data range and \a other share common data points.

  \see intersection, contains
*/
bool QCPDataRange::intersects(const QCPDataRange &other) const
{
    return !( (mBegin > other.mBegin && mBegin >= other.mEnd) ||
              (mEnd <= other.mBegin && mEnd < other.mEnd) );
}

/*!
  Returns whether all data points of \a other are also contained inside this data range.

  \see intersects
*/
bool QCPDataRange::contains(const QCPDataRange &other) const
{
    return mBegin <= other.mBegin && mEnd >= other.mEnd;
}

