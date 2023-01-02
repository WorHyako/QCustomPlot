#include "include/QCPPlottableLegendItem.hpp"

namespace QCP {
/*! \class QCPPlottableLegendItem
  \brief A legend item representing a plottable with an icon and the plottable name.

  This is the standard legend item for plottables. It displays an icon of the plottable next to the
  plottable name. The icon is drawn by the respective plottable itself (\ref
  QCPAbstractPlottable::drawLegendIcon), and tries to give an intuitive symbol for the plottable.
  For example, the QCPGraph draws a centered horizontal line and/or a single scatter point in the
  middle.

  Legend items of this type are always associated with one plottable (retrievable via the
  plottable() function and settable with the constructor). You may change the font of the plottable
  name with \ref setFont. Icon padding and border pen is taken from the parent QCPLegend, see \ref
  QCPLegend::setIconBorderPen and \ref QCPLegend::setIconTextPadding.

  The function \ref QCPAbstractPlottable::addToLegend/\ref QCPAbstractPlottable::removeFromLegend
  creates/removes legend items of this type.

  Since QCPLegend is based on QCPLayoutGrid, a legend item itself is just a subclass of
  QCPLayoutElement. While it could be added to a legend (or any other layout) via the normal layout
  interface, QCPLegend has specialized functions for handling legend items conveniently, see the
  documentation of \ref QCPLegend.
*/

/*!
  Creates a new legend item associated with \a plottable.

  Once it's created, it can be added to the legend via \ref QCPLegend::addItem.

  A more convenient way of adding/removing a plottable to/from the legend is via the functions \ref
  QCPAbstractPlottable::addToLegend and \ref QCPAbstractPlottable::removeFromLegend.
*/
    QCPPlottableLegendItem::QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable) :
    QCPAbstractLegendItem(parent),
    mPlottable(plottable)
    {
        setAntialiased(false);
    }

/*! \internal

  Returns the pen that shall be used to draw the icon border, taking into account the selection
  state of this item.
*/
    QPen QCPPlottableLegendItem::getIconBorderPen() const
    {
        return mSelected ? mParentLegend->selectedIconBorderPen() : mParentLegend->iconBorderPen();
    }

/*! \internal

  Returns the text color that shall be used to draw text, taking into account the selection state
  of this item.
*/
    QColor QCPPlottableLegendItem::getTextColor() const
    {
        return mSelected ? mSelectedTextColor : mTextColor;
    }

/*! \internal

  Returns the font that shall be used to draw text, taking into account the selection state of this
  item.
*/
    QFont QCPPlottableLegendItem::getFont() const
    {
        return mSelected ? mSelectedFont : mFont;
    }

/*! \internal

  Draws the item with \a painter. The size and position of the drawn legend item is defined by the
  parent layout (typically a \ref QCPLegend) and the \ref minimumOuterSizeHint and \ref
  maximumOuterSizeHint of this legend item.
*/
    void QCPPlottableLegendItem::draw(QCPPainter *painter)
    {
        if (!mPlottable) return;
        painter->setFont(getFont());
        painter->setPen(QPen(getTextColor()));
        QSize iconSize = mParentLegend->iconSize();
        QRect textRect = painter->fontMetrics().boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPlottable->name());
        QRect iconRect(mRect.topLeft(), iconSize);
        int textHeight = qMax(textRect.height(), iconSize.height());  // if text has smaller height than icon, center text vertically in icon height, else align tops
        painter->drawText(mRect.x()+iconSize.width()+mParentLegend->iconTextPadding(), mRect.y(), textRect.width(), textHeight, Qt::TextDontClip, mPlottable->name());
// draw icon:
        painter->save();
        painter->setClipRect(iconRect, Qt::IntersectClip);
        mPlottable->drawLegendIcon(painter, iconRect);
        painter->restore();
// draw icon border:
        if (getIconBorderPen().style() != Qt::NoPen)
        {
            painter->setPen(getIconBorderPen());
            painter->setBrush(Qt::NoBrush);
            int halfPen = qCeil(painter->pen().widthF()*0.5)+1;
            painter->setClipRect(mOuterRect.adjusted(-halfPen, -halfPen, halfPen, halfPen)); // extend default clip rect so thicker pens (especially during selection) are not clipped
            painter->drawRect(iconRect);
        }
    }

/*! \internal

  Calculates and returns the size of this item. This includes the icon, the text and the padding in
  between.

  \seebaseclassmethod
*/
    QSize QCPPlottableLegendItem::minimumOuterSizeHint() const
    {
        if (!mPlottable) return {};
        QSize result(0, 0);
        QRect textRect;
        QFontMetrics fontMetrics(getFont());
        QSize iconSize = mParentLegend->iconSize();
        textRect = fontMetrics.boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPlottable->name());
        result.setWidth(iconSize.width() + mParentLegend->iconTextPadding() + textRect.width());
        result.setHeight(qMax(textRect.height(), iconSize.height()));
        result.rwidth() += mMargins.left()+mMargins.right();
        result.rheight() += mMargins.top()+mMargins.bottom();
        return result;
    }


} // QCP