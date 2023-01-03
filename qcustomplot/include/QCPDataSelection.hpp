#ifndef QCUSTOMPLOT_QCPDATASELECTION_HPP
#define QCUSTOMPLOT_QCPDATASELECTION_HPP

#include "defs.hpp"
#include "enums.hpp"
#include "QCPDataRange.hpp"

#include <QtCore>

namespace QCP {

    class QCP_LIB_DECL QCPDataSelection {
    public:
        explicit QCPDataSelection();

        explicit QCPDataSelection(const QCPDataRange &range);

        bool operator==(const QCPDataSelection &other) const;

        bool operator!=(const QCPDataSelection &other) const { return !(*this == other); }

        QCPDataSelection &operator+=(const QCPDataSelection &other);

        QCPDataSelection &operator+=(const QCPDataRange &other);

        QCPDataSelection &operator-=(const QCPDataSelection &other);

        QCPDataSelection &operator-=(const QCPDataRange &other);

        friend inline const QCPDataSelection operator+(const QCPDataSelection &a, const QCPDataSelection &b);

        friend inline const QCPDataSelection operator+(const QCPDataRange &a, const QCPDataSelection &b);

        friend inline const QCPDataSelection operator+(const QCPDataSelection &a, const QCPDataRange &b);

        friend inline const QCPDataSelection operator+(const QCPDataRange &a, const QCPDataRange &b);

        friend inline const QCPDataSelection operator-(const QCPDataSelection &a, const QCPDataSelection &b);

        friend inline const QCPDataSelection operator-(const QCPDataRange &a, const QCPDataSelection &b);

        friend inline const QCPDataSelection operator-(const QCPDataSelection &a, const QCPDataRange &b);

        friend inline const QCPDataSelection operator-(const QCPDataRange &a, const QCPDataRange &b);

        int dataRangeCount() const { return mDataRanges.size(); }

        int dataPointCount() const;

        QCPDataRange dataRange(int index = 0) const;

        QList<QCPDataRange> dataRanges() const { return mDataRanges; }

        QCPDataRange span() const;

        void addDataRange(const QCPDataRange &dataRange, bool simplify = true);

        void clear();

        bool isEmpty() const { return mDataRanges.isEmpty(); }

        void simplify();

        void enforceType(QCP::SelectionType type);

        bool contains(const QCPDataSelection &other) const;

        QCPDataSelection intersection(const QCPDataRange &other) const;

        QCPDataSelection intersection(const QCPDataSelection &other) const;

        QCPDataSelection inverse(const QCPDataRange &outerRange) const;

    private:
        QList<QCPDataRange> mDataRanges;

        inline static bool lessThanDataRangeBegin(const QCPDataRange &a, const QCPDataRange &b) {
            return a.begin() < b.begin();
        }
    };
}

Q_DECLARE_METATYPE(QCP::QCPDataSelection)

using namespace QCP;

/*!
  Return a \ref QCPDataSelection with the data points in \a a joined with the data points in \a b.
  The resulting data selection is already simplified (see \ref QCPDataSelection::simplify).
*/
inline const QCPDataSelection operator+(const QCPDataSelection &a, const QCPDataSelection &b) {
    QCPDataSelection result(a);
    result += b;
    return result;
}

/*!
  Return a \ref QCPDataSelection with the data points in \a a joined with the data points in \a b.
  The resulting data selection is already simplified (see \ref QCPDataSelection::simplify).
*/
inline const QCPDataSelection operator+(const QCPDataRange &a, const QCPDataSelection &b) {
    QCPDataSelection result(a);
    result += b;
    return result;
}

/*!
  Return a \ref QCPDataSelection with the data points in \a a joined with the data points in \a b.
  The resulting data selection is already simplified (see \ref QCPDataSelection::simplify).
*/
inline const QCPDataSelection operator+(const QCPDataSelection &a, const QCPDataRange &b) {
    QCPDataSelection result(a);
    result += b;
    return result;
}

/*!
  Return a \ref QCPDataSelection with the data points in \a a joined with the data points in \a b.
  The resulting data selection is already simplified (see \ref QCPDataSelection::simplify).
*/
inline const QCPDataSelection operator+(const QCPDataRange &a, const QCPDataRange &b) {
    QCPDataSelection result(a);
    result += b;
    return result;
}

/*!
  Return a \ref QCPDataSelection with the data points which are in \a a but not in \a b.
*/
inline const QCPDataSelection operator-(const QCPDataSelection &a, const QCPDataSelection &b) {
    QCPDataSelection result(a);
    result -= b;
    return result;
}

/*!
  Return a \ref QCPDataSelection with the data points which are in \a a but not in \a b.
*/
inline const QCPDataSelection operator-(const QCPDataRange &a, const QCPDataSelection &b) {
    QCPDataSelection result(a);
    result -= b;
    return result;
}

/*!
  Return a \ref QCPDataSelection with the data points which are in \a a but not in \a b.
*/
inline const QCPDataSelection operator-(const QCPDataSelection &a, const QCPDataRange &b) {
    QCPDataSelection result(a);
    result -= b;
    return result;
}

/*!
  Return a \ref QCPDataSelection with the data points which are in \a a but not in \a b.
*/
inline const QCPDataSelection operator-(const QCPDataRange &a, const QCPDataRange &b) {
    QCPDataSelection result(a);
    result -= b;
    return result;
}

/*! \relates QCPDataRange

  Prints \a dataRange in a human readable format to the qDebug output.
*/
inline QDebug operator<<(QDebug d, const QCPDataRange &dataRange) {
    d.nospace() << "QCPDataRange(" << dataRange.begin() << ", " << dataRange.end() << ")";
    return d;
}

/*! \relates QCPDataSelection

  Prints \a selection in a human readable format to the qDebug output.
*/
inline QDebug operator<<(QDebug d, const QCPDataSelection &selection) {
    d.nospace() << "QCPDataSelection(";
    for (int i = 0; i < selection.dataRangeCount(); ++i) {
        if (i != 0)
            d << ", ";
        d << selection.dataRange(i);
    }
    d << ")";
    return d;
}

#endif
