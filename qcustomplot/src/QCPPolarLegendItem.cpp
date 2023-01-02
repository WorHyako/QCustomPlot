#include "include/QCPPolarLegendItem.hpp"

namespace QCP {
/*! \class QCPPolarLegendItem
  \brief A legend item for polar plots

  \warning In this QCustomPlot version, polar plots are a tech preview. Expect documentation and
  functionality to be incomplete, as well as changing public interfaces in the future.
*/
    QCPPolarLegendItem::QCPPolarLegendItem(QCPLegend *parent, QCPPolarGraph *graph) :
    QCPAbstractLegendItem(parent),
    mPolarGraph(graph)
    {
        setAntialiased(false);
    }

    void QCPPolarLegendItem::draw(QCPPainter *painter)
    {
        if (!mPolarGraph) return;
        painter->setFont(getFont());
        painter->setPen(QPen(getTextColor()));
        QSizeF iconSize = mParentLegend->iconSize();
        QRectF textRect = painter->fontMetrics().boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPolarGraph->name());
        QRectF iconRect(mRect.topLeft(), iconSize);
        int textHeight = qMax(textRect.height(), iconSize.height());  // if text has smaller height than icon, center text vertically in icon height, else align tops
        painter->drawText(mRect.x()+iconSize.width()+mParentLegend->iconTextPadding(), mRect.y(), textRect.width(), textHeight, Qt::TextDontClip, mPolarGraph->name());
// draw icon:
        painter->save();
        painter->setClipRect(iconRect, Qt::IntersectClip);
        mPolarGraph->drawLegendIcon(painter, iconRect);
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

    QSize QCPPolarLegendItem::minimumOuterSizeHint() const
    {
        if (!mPolarGraph) return QSize();
        QSize result(0, 0);
        QRect textRect;
        QFontMetrics fontMetrics(getFont());
        QSize iconSize = mParentLegend->iconSize();
        textRect = fontMetrics.boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPolarGraph->name());
        result.setWidth(iconSize.width() + mParentLegend->iconTextPadding() + textRect.width());
        result.setHeight(qMax(textRect.height(), iconSize.height()));
        result.rwidth() += mMargins.left()+mMargins.right();
        result.rheight() += mMargins.top()+mMargins.bottom();
        return result;
    }

    QPen QCPPolarLegendItem::getIconBorderPen() const
    {
        return mSelected ? mParentLegend->selectedIconBorderPen() : mParentLegend->iconBorderPen();
    }

    QColor QCPPolarLegendItem::getTextColor() const
    {
        return mSelected ? mSelectedTextColor : mTextColor;
    }

    QFont QCPPolarLegendItem::getFont() const
    {
        return mSelected ? mSelectedFont : mFont;
    }

} // QCP