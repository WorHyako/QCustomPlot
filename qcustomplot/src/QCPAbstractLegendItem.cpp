#include "QCPAbstractLegendItem.hpp"

using namespace QCP;

/*! \class QCPAbstractLegendItem
  \brief The abstract base class for all entries in a QCPLegend.

  It defines a very basic interface for entries in a QCPLegend. For representing plottables in the
  legend, the subclass \ref QCPPlottableLegendItem is more suitable.

  Only derive directly from this class when you need absolute freedom (e.g. a custom legend entry
  that's not even associated with a plottable).

  You must implement the following pure virtual functions:
  \li \ref draw (from QCPLayerable)

  You inherit the following members you may use:
  <table>
    <tr>
      <td>QCPLegend *\b mParentLegend</td>
      <td>A pointer to the parent QCPLegend.</td>
    </tr><tr>
      <td>QFont \b mFont</td>
      <td>The generic font of the item. You should use this font for all or at least the most prominent text of the item.</td>
    </tr>
  </table>
*/

/*! \fn void QCPAbstractLegendItem::selectionChanged(bool selected)

  This signal is emitted when the selection state of this legend item has changed, either by user
  interaction or by a direct call to \ref setSelected.
*/

/*!
  Constructs a QCPAbstractLegendItem and associates it with the QCPLegend \a parent. This does not
  cause the item to be added to \a parent, so \ref QCPLegend::addItem must be called separately.
*/
QCPAbstractLegendItem::QCPAbstractLegendItem(QCPLegend *parent) :
        QCPLayoutElement(reinterpret_cast<QCPLayerable *>(parent)->parentPlot()),
        mParentLegend(parent),
        mFont(parent->font()),
        mTextColor(parent->textColor()),
        mSelectedFont(parent->selectedFont()),
        mSelectedTextColor(parent->selectedTextColor()),
        mSelectable(true),
        mSelected(false) {
    setLayer(QLatin1String("legend"));
    setMargins(QMargins(0, 0, 0, 0));
}

/*!
  Sets the default font of this specific legend item to \a font.

  \see setTextColor, QCPLegend::setFont
*/
void QCPAbstractLegendItem::setFont(const QFont &font) {
    mFont = font;
}

/*!
  Sets the default text color of this specific legend item to \a color.

  \see setFont, QCPLegend::setTextColor
*/
void QCPAbstractLegendItem::setTextColor(const QColor &color) {
    mTextColor = color;
}

/*!
  When this legend item is selected, \a font is used to draw generic text, instead of the normal
  font set with \ref setFont.

  \see setFont, QCPLegend::setSelectedFont
*/
void QCPAbstractLegendItem::setSelectedFont(const QFont &font) {
    mSelectedFont = font;
}

/*!
  When this legend item is selected, \a color is used to draw generic text, instead of the normal
  color set with \ref setTextColor.

  \see setTextColor, QCPLegend::setSelectedTextColor
*/
void QCPAbstractLegendItem::setSelectedTextColor(const QColor &color) {
    mSelectedTextColor = color;
}

/*!
  Sets whether this specific legend item is selectable.

  \see setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAbstractLegendItem::setSelectable(bool selectable) {
    if (mSelectable != selectable) {
        mSelectable = selectable;
        emit selectableChanged(mSelectable);
    }
}

/*!
  Sets whether this specific legend item is selected.

  It is possible to set the selection state of this item by calling this function directly, even if
  setSelectable is set to false.

  \see setSelectableParts, QCustomPlot::setInteractions
*/
void QCPAbstractLegendItem::setSelected(bool selected) {
    if (mSelected != selected) {
        mSelected = selected;
        emit selectionChanged(mSelected);
    }
}

/* inherits documentation from base class */
double QCPAbstractLegendItem::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const {
    Q_UNUSED(details)
    if (!mParentPlot) return -1;
    if (onlySelectable && (!mSelectable || !mParentLegend->selectableParts().testFlag(QCPLegend::spItems)))
        return -1;

    if (mRect.contains(pos.toPoint()))
        return mParentPlot->selectionTolerance() * 0.99;
    else
        return -1;
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::applyDefaultAntialiasingHint(QCPPainter *painter) const {
    applyAntialiasingHint(painter, mAntialiased, QCP::aeLegendItems);
}

/* inherits documentation from base class */
QRect QCPAbstractLegendItem::clipRect() const {
    return mOuterRect;
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::selectEvent(QMouseEvent *event, bool additive, const QVariant &details,
                                        bool *selectionStateChanged) {
    Q_UNUSED(event)
    Q_UNUSED(details)
    if (mSelectable && mParentLegend->selectableParts().testFlag(QCPLegend::spItems)) {
        bool selBefore = mSelected;
        setSelected(additive ? !mSelected : true);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

/* inherits documentation from base class */
void QCPAbstractLegendItem::deselectEvent(bool *selectionStateChanged) {
    if (mSelectable && mParentLegend->selectableParts().testFlag(QCPLegend::spItems)) {
        bool selBefore = mSelected;
        setSelected(false);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}
