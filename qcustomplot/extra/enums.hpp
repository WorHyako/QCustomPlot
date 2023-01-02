#ifndef QCUSTOMPLOT_ENUMS_H
#define QCUSTOMPLOT_ENUMS_H

#include <QtCore>

namespace QCP {

/*!
  Defines the different units in which the image resolution can be specified in the export
  functions.

  \see QCustomPlot::savePng, QCustomPlot::saveJpg, QCustomPlot::saveBmp, QCustomPlot::saveRastered
*/
    enum ResolutionUnit {
        ruDotsPerMeter,       ///< Resolution is given in dots per meter (dpm)
        ruDotsPerCentimeter, ///< Resolution is given in dots per centimeter (dpcm)
        ruDotsPerInch       ///< Resolution is given in dots per inch (DPI/PPI)
    };

/*!
  Defines how cosmetic pens (pens with numerical width 0) are handled during export.

  \see QCustomPlot::savePdf
*/
    enum ExportPen {
        epNoCosmetic,     ///< Cosmetic pens are converted to pens with pixel width 1 when exporting
        epAllowCosmetic ///< Cosmetic pens are exported normally (e.g. in PDF exports, cosmetic pens always appear as 1 pixel on screen, independent of viewer zoom level)
    };

/*!
  Represents negative and positive sign domain, e.g. for passing to \ref
  QCPAbstractPlottable::getKeyRange and \ref QCPAbstractPlottable::getValueRange.

  This is primarily needed when working with logarithmic axis scales, since only one of the sign
  domains can be visible at a time.
*/
    enum SignDomain {
        sdNegative,  ///< The negative sign domain, i.e. numbers smaller than zero
        sdBoth,     ///< Both sign domains, including zero, i.e. all numbers
        sdPositive ///< The positive sign domain, i.e. numbers greater than zero
    };

/*!
  Defines the sides of a rectangular entity to which margins can be applied.

  \see QCPLayoutElement::setAutoMargins, QCPAxisRect::setAutoMargins
*/
    enum MarginSide {
        msLeft = 0x01, ///< <tt>0x01</tt> left margin
        msRight = 0x02, ///< <tt>0x02</tt> right margin
        msTop = 0x04, ///< <tt>0x04</tt> top margin
        msBottom = 0x08, ///< <tt>0x08</tt> bottom margin
        msAll = 0xFF, ///< <tt>0xFF</tt> all margins
        msNone = 0x00 ///< <tt>0x00</tt> no margin
    };
    Q_DECLARE_FLAGS(MarginSides, MarginSide)

/*!
  Defines what objects of a plot can be forcibly drawn antialiased/not antialiased. If an object is
  neither forcibly drawn antialiased nor forcibly drawn not antialiased, it is up to the respective
  element how it is drawn. Typically it provides a \a setAntialiased function for this.

  \c AntialiasedElements is a flag of or-combined elements of this enum type.

  \see QCustomPlot::setAntialiasedElements, QCustomPlot::setNotAntialiasedElements
*/
    enum AntialiasedElement {
        aeAxes = 0x0001, ///< <tt>0x0001</tt> Axis base line and tick marks
        aeGrid = 0x0002, ///< <tt>0x0002</tt> Grid lines
        aeSubGrid = 0x0004, ///< <tt>0x0004</tt> Sub grid lines
        aeLegend = 0x0008, ///< <tt>0x0008</tt> Legend box
        aeLegendItems = 0x0010, ///< <tt>0x0010</tt> Legend items
        aePlottables = 0x0020, ///< <tt>0x0020</tt> Main lines of plottables
        aeItems = 0x0040, ///< <tt>0x0040</tt> Main lines of items
        aeScatters = 0x0080, ///< <tt>0x0080</tt> Scatter symbols of plottables (excluding scatter symbols of type ssPixmap)
        aeFills = 0x0100, ///< <tt>0x0100</tt> Borders of fills (e.g. under or between graphs)
        aeZeroLine = 0x0200, ///< <tt>0x0200</tt> Zero-lines, see \ref QCPGrid::setZeroLinePen
        aeOther = 0x8000, ///< <tt>0x8000</tt> Other elements that don't fit into any of the existing categories
        aeAll = 0xFFFF, ///< <tt>0xFFFF</tt> All elements
        aeNone = 0x0000 ///< <tt>0x0000</tt> No elements
    };
    Q_DECLARE_FLAGS(AntialiasedElements, AntialiasedElement)

/*!
  Defines plotting hints that control various aspects of the quality and speed of plotting.

  \see QCustomPlot::setPlottingHints
*/
    enum PlottingHint {
        phNone = 0x000, ///< <tt>0x000</tt> No hints are set
        phFastPolylines = 0x001, ///< <tt>0x001</tt> Graph/Curve lines are drawn with a faster method. This reduces the quality especially of the line segment
        ///< joins, thus is most effective for pen sizes larger than 1. It is only used for solid line pens.
        phImmediateRefresh = 0x002, ///< <tt>0x002</tt> causes an immediate repaint() instead of a soft update() when QCustomPlot::replot() is called with parameter \ref QCustomPlot::rpRefreshHint.
        ///< This is set by default to prevent the plot from freezing on fast consecutive replots (e.g. user drags ranges with mouse).
        phCacheLabels = 0x004 ///< <tt>0x004</tt> axis (tick) labels will be cached as pixmaps, increasing replot performance.
    };
    Q_DECLARE_FLAGS(PlottingHints, PlottingHint)

/*!
  Defines the mouse interactions possible with QCustomPlot.

  \c Interactions is a flag of or-combined elements of this enum type.

  \see QCustomPlot::setInteractions
*/
    enum Interaction {
        iNone = 0x000, ///< <tt>0x000</tt> None of the interactions are possible
        iRangeDrag = 0x001, ///< <tt>0x001</tt> Axis ranges are draggable (see \ref QCPAxisRect::setRangeDrag, \ref QCPAxisRect::setRangeDragAxes)
        iRangeZoom = 0x002, ///< <tt>0x002</tt> Axis ranges are zoomable with the mouse wheel (see \ref QCPAxisRect::setRangeZoom, \ref QCPAxisRect::setRangeZoomAxes)
        iMultiSelect = 0x004, ///< <tt>0x004</tt> The user can select multiple objects by holding the modifier set by \ref QCustomPlot::setMultiSelectModifier while clicking
        iSelectPlottables = 0x008, ///< <tt>0x008</tt> Plottables are selectable (e.g. graphs, curves, bars,... see QCPAbstractPlottable)
        iSelectAxes = 0x010, ///< <tt>0x010</tt> Axes are selectable (or parts of them, see QCPAxis::setSelectableParts)
        iSelectLegend = 0x020, ///< <tt>0x020</tt> Legends are selectable (or their child items, see QCPLegend::setSelectableParts)
        iSelectItems = 0x040, ///< <tt>0x040</tt> Items are selectable (Rectangles, Arrows, Textitems, etc. see \ref QCPAbstractItem)
        iSelectOther = 0x080, ///< <tt>0x080</tt> All other objects are selectable (e.g. your own derived layerables, other layout elements,...)
        iSelectPlottablesBeyondAxisRect = 0x100 ///< <tt>0x100</tt> When performing plottable selection/hit tests, this flag extends the sensitive area beyond the axis rect
    };
    Q_DECLARE_FLAGS(Interactions, Interaction)

/*!
  Defines the behaviour of the selection rect.

  \see QCustomPlot::setSelectionRectMode, QCustomPlot::selectionRect, QCPSelectionRect
*/
    enum SelectionRectMode {
        srmNone,    ///< The selection rect is disabled, and all mouse events are forwarded to the underlying objects, e.g. for axis range dragging
        srmZoom,   ///< When dragging the mouse, a selection rect becomes active. Upon releasing, the axes that are currently set as range zoom axes (\ref QCPAxisRect::setRangeZoomAxes) will have their ranges zoomed accordingly.
        srmSelect, ///< When dragging the mouse, a selection rect becomes active. Upon releasing, plottable data points that were within the selection rect are selected, if the plottable's selectability setting permits. (See  \ref dataselection "data selection mechanism" for details.)
        srmCustom ///< When dragging the mouse, a selection rect becomes active. It is the programmer's responsibility to connect according slots to the selection rect's signals (e.g. \ref QCPSelectionRect::accepted) in order to process the user interaction.
    };

/*!
  Defines the different ways a plottable can be selected. These images show the effect of the
  different selection types, when the indicated selection rect was dragged:

  <center>
  <table>
  <tr>
    <td>\image html selectiontype-none.png stNone</td>
    <td>\image html selectiontype-whole.png stWhole</td>
    <td>\image html selectiontype-singledata.png stSingleData</td>
    <td>\image html selectiontype-datarange.png stDataRange</td>
    <td>\image html selectiontype-multipledataranges.png stMultipleDataRanges</td>
  </tr>
  </table>
  </center>

  \see QCPAbstractPlottable::setSelectable, QCPDataSelection::enforceType
*/
    enum SelectionType {
        stNone,                ///< The plottable is not selectable
        stWhole,              ///< Selection behaves like \ref stMultipleDataRanges, but if there are any data points selected, the entire plottable is drawn as selected.
        stSingleData,         ///< One individual data point can be selected at a time
        stDataRange,          ///< Multiple contiguous data points (a data range) can be selected
        stMultipleDataRanges ///< Any combination of data points/ranges can be selected
    };

}

#endif
