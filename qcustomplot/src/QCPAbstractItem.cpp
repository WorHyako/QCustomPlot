#include "QCPAbstractItem.hpp"

using namespace QCP;

/*! \class QCPAbstractItem
  \brief The abstract base class for all items in a plot.

  In QCustomPlot, items are supplemental graphical elements that are neither plottables
  (QCPAbstractPlottable) nor axes (QCPAxis). While plottables are always tied to two axes and thus
  plot coordinates, items can also be placed in absolute coordinates independent of any axes. Each
  specific item has at least one QCPItemPosition member which controls the positioning. Some items
  are defined by more than one coordinate and thus have two or more QCPItemPosition members (For
  example, QCPItemRect has \a topLeft and \a bottomRight).

  This abstract base class defines a very basic interface like visibility and clipping. Since this
  class is abstract, it can't be instantiated. Use one of the subclasses or create a subclass
  yourself to create new items.

  The built-in items are:
  <table>
  <tr><td>QCPItemLine</td><td>A line defined by a start and an end point. May have different ending styles on each side (e.g. arrows).</td></tr>
  <tr><td>QCPItemStraightLine</td><td>A straight line defined by a start and a direction point. Unlike QCPItemLine, the straight line is infinitely long and has no endings.</td></tr>
  <tr><td>QCPItemCurve</td><td>A curve defined by start, end and two intermediate control points. May have different ending styles on each side (e.g. arrows).</td></tr>
  <tr><td>QCPItemRect</td><td>A rectangle</td></tr>
  <tr><td>QCPItemEllipse</td><td>An ellipse</td></tr>
  <tr><td>QCPItemPixmap</td><td>An arbitrary pixmap</td></tr>
  <tr><td>QCPItemText</td><td>A text label</td></tr>
  <tr><td>QCPItemBracket</td><td>A bracket which may be used to reference/highlight certain parts in the plot.</td></tr>
  <tr><td>QCPItemTracer</td><td>An item that can be attached to a QCPGraph and sticks to its data points, given a key coordinate.</td></tr>
  </table>

  \section items-clipping Clipping

  Items are by default clipped to the main axis rect (they are only visible inside the axis rect).
  To make an item visible outside that axis rect, disable clipping via \ref setClipToAxisRect
  "setClipToAxisRect(false)".

  On the other hand if you want the item to be clipped to a different axis rect, specify it via
  \ref setClipAxisRect. This clipAxisRect property of an item is only used for clipping behaviour, and
  in principle is independent of the coordinate axes the item might be tied to via its position
  members (\ref QCPItemPosition::setAxes). However, it is common that the axis rect for clipping
  also contains the axes used for the item positions.

  \section items-using Using items

  First you instantiate the item you want to use and add it to the plot:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-1
  by default, the positions of the item are bound to the x- and y-Axis of the plot. So we can just
  set the plot coordinates where the line should start/end:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-2
  If we don't want the line to be positioned in plot coordinates but a different coordinate system,
  e.g. absolute pixel positions on the QCustomPlot surface, we need to change the position type like this:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-3
  Then we can set the coordinates, this time in pixels:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-4
  and make the line visible on the entire QCustomPlot, by disabling clipping to the axis rect:
  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpitemline-creation-5

  For more advanced plots, it is even possible to set different types and parent anchors per X/Y
  coordinate of an item position, using for example \ref QCPItemPosition::setTypeX or \ref
  QCPItemPosition::setParentAnchorX. For details, see the documentation of \ref QCPItemPosition.

  \section items-subclassing Creating own items

  To create an own item, you implement a subclass of QCPAbstractItem. These are the pure
  virtual functions, you must implement:
  \li \ref selectTest
  \li \ref draw

  See the documentation of those functions for what they need to do.

  \subsection items-positioning Allowing the item to be positioned

  As mentioned, item positions are represented by QCPItemPosition members. Let's assume the new item shall
  have only one point as its position (as opposed to two like a rect or multiple like a polygon). You then add
  a public member of type QCPItemPosition like so:

  \code QCPItemPosition * const myPosition;\endcode

  the const makes sure the pointer itself can't be modified from the user of your new item (the QCPItemPosition
  instance it points to, can be modified, of course).
  The initialization of this pointer is made easy with the \ref createPosition function. Just assign
  the return value of this function to each QCPItemPosition in the constructor of your item. \ref createPosition
  takes a string which is the name of the position, typically this is identical to the variable name.
  For example, the constructor of QCPItemExample could look like this:

  \code
  QCPItemExample::QCPItemExample(QCustomPlot *parentPlot) :
    QCPAbstractItem(parentPlot),
    myPosition(createPosition("myPosition"))
  {
    // other constructor code
  }
  \endcode

  \subsection items-drawing The draw function

  To give your item a visual representation, reimplement the \ref draw function and use the passed
  QCPPainter to draw the item. You can retrieve the item position in pixel coordinates from the
  position member(s) via \ref QCPItemPosition::pixelPosition.

  To optimize performance you should calculate a bounding rect first (don't forget to take the pen
  width into account), check whether it intersects the \ref clipRect, and only draw the item at all
  if this is the case.

  \subsection items-selection The selectTest function

  Your implementation of the \ref selectTest function may use the helpers \ref
  QCPVector2D::distanceSquaredToLine and \ref rectDistance. With these, the implementation of the
  selection test becomes significantly simpler for most items. See the documentation of \ref
  selectTest for what the function parameters mean and what the function should return.

  \subsection anchors Providing anchors

  Providing anchors (QCPItemAnchor) starts off like adding a position. First you create a public
  member, e.g.

  \code QCPItemAnchor * const bottom;\endcode

  and create it in the constructor with the \ref createAnchor function, assigning it a name and an
  anchor id (an integer enumerating all anchors on the item, you may create an own enum for this).
  Since anchors can be placed anywhere, relative to the item's position(s), your item needs to
  provide the position of every anchor with the reimplementation of the \ref anchorPixelPosition(int
  anchorId) function.

  In essence the QCPItemAnchor is merely an intermediary that itself asks your item for the pixel
  position when anything attached to the anchor needs to know the coordinates.
*/

/*! \fn QList<QCPItemPosition*> QCPAbstractItem::positions() const

  Returns all positions of the item in a list.

  \see anchors, position
*/

/*! \fn QList<QCPItemAnchor*> QCPAbstractItem::anchors() const

  Returns all anchors of the item in a list. Note that since a position (QCPItemPosition) is always
  also an anchor, the list will also contain the positions of this item.

  \see positions, anchor
*/

/*! \fn void QCPAbstractItem::draw(QCPPainter *painter) = 0
  \internal

  Draws this item with the provided \a painter.

  The cliprect of the provided painter is set to the rect returned by \ref clipRect before this
  function is called. The clipRect depends on the clipping settings defined by \ref
  setClipToAxisRect and \ref setClipAxisRect.
*/

/*! \fn void QCPAbstractItem::selectionChanged(bool selected)
  This signal is emitted when the selection state of this item has changed, either by user interaction
  or by a direct call to \ref setSelected.
*/

/*!
  Base class constructor which initializes base class members.
*/
QCPAbstractItem::QCPAbstractItem(QCustomPlot *parentPlot) :
        QCPLayerable(parentPlot),
        mClipToAxisRect(false),
        mSelectable(true),
        mSelected(false) {
    parentPlot->registerItem(this);

    QList<QCPAxisRect *> rects = parentPlot->axisRects();
    if (!rects.isEmpty()) {
        setClipToAxisRect(true);
        setClipAxisRect(rects.first());
    }
}

QCPAbstractItem::~QCPAbstractItem() {
// don't delete mPositions because every position is also an anchor and thus in mAnchors
    qDeleteAll(mAnchors);
}

/* can't make this a header inline function, because QPointer breaks with forward declared types, see QTBUG-29588 */
QCPAxisRect *QCPAbstractItem::clipAxisRect() const {
    return mClipAxisRect.get();
}

/*!
  Sets whether the item shall be clipped to an axis rect or whether it shall be visible on the
  entire QCustomPlot. The axis rect can be set with \ref setClipAxisRect.

  \see setClipAxisRect
*/
void QCPAbstractItem::setClipToAxisRect(bool clip) {
    mClipToAxisRect = clip;
    if (mClipToAxisRect) {
        setParentLayerable(reinterpret_cast<QCPLayerable *>(mClipAxisRect.get()));
    }
}

/*!
  Sets the clip axis rect. It defines the rect that will be used to clip the item when \ref
  setClipToAxisRect is set to true.

  \see setClipToAxisRect
*/
void QCPAbstractItem::setClipAxisRect(QCPAxisRect *rect) {
    mClipAxisRect = std::make_shared<QCPAxisRect>(rect);
    if (mClipToAxisRect) {
        setParentLayerable(reinterpret_cast<QCPLayerable *>(mClipAxisRect.get()));
    }
}

/*!
  Sets whether the user can (de-)select this item by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains QCustomPlot::iSelectItems.)

  However, even when \a selectable was set to false, it is possible to set the selection manually,
  by calling \ref setSelected.

  \see QCustomPlot::setInteractions, setSelected
*/
void QCPAbstractItem::setSelectable(bool selectable) {
    if (mSelectable != selectable) {
        mSelectable = selectable;
        emit selectableChanged(mSelectable);
    }
}

/*!
  Sets whether this item is selected or not. When selected, it might use a different visual
  appearance (e.g. pen and brush), this depends on the specific item though.

  The entire selection mechanism for items is handled automatically when \ref
  QCustomPlot::setInteractions contains QCustomPlot::iSelectItems. You only need to call this
  function when you wish to change the selection state manually.

  This function can change the selection state even when \ref setSelectable was set to false.

  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.

  \see setSelectable, selectTest
*/
void QCPAbstractItem::setSelected(bool selected) {
    if (mSelected != selected) {
        mSelected = selected;
        emit selectionChanged(mSelected);
    }
}

/*!
  Returns the QCPItemPosition with the specified \a name. If this item doesn't have a position by
  that name, returns \c nullptr.

  This function provides an alternative way to access item positions. Normally, you access
  positions direcly by their member pointers (which typically have the same variable name as \a
  name).

  \see positions, anchor
*/
QCPItemPosition *QCPAbstractItem::position(const QString &name) const {
            foreach (auto *position, mPositions) {
            if (reinterpret_cast<QCPItemAnchor *>(position)->name() == name) {
                return position;
            }
        }
    qDebug() << Q_FUNC_INFO << "position with name not found:" << name;
    return nullptr;
}

/*!
  Returns the QCPItemAnchor with the specified \a name. If this item doesn't have an anchor by
  that name, returns \c nullptr.

  This function provides an alternative way to access item anchors. Normally, you access
  anchors direcly by their member pointers (which typically have the same variable name as \a
  name).

  \see anchors, position
*/
QCPItemAnchor *QCPAbstractItem::anchor(const QString &name) const {
            foreach (auto *anchor, mAnchors) {
            if (anchor->name() == name) {
                return anchor;
            }
        }
    qDebug() << Q_FUNC_INFO << "anchor with name not found:" << name;
    return nullptr;
}

/*!
  Returns whether this item has an anchor with the specified \a name.

  Note that you can check for positions with this function, too. This is because every position is
  also an anchor (QCPItemPosition inherits from QCPItemAnchor).

  \see anchor, position
*/
bool QCPAbstractItem::hasAnchor(const QString &name) const {
            foreach (QCPItemAnchor *anchor, mAnchors) {
            if (anchor->name() == name) {
                return true;
            }
        }
    return false;
}

/*! \internal

  Returns the rect the visual representation of this item is clipped to. This depends on the
  current setting of \ref setClipToAxisRect as well as the axis rect set with \ref setClipAxisRect.

  If the item is not clipped to an axis rect, QCustomPlot's viewport rect is returned.

  \see draw
*/
QRect QCPAbstractItem::clipRect() const {
    if (mClipToAxisRect && mClipAxisRect) {
        return reinterpret_cast<QCPLayoutElement *>(mClipAxisRect.get())->rect();
    } else {
        return mParentPlot->viewport();
    }
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing item lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.

  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.

  \see setAntialiased
*/
void QCPAbstractItem::applyDefaultAntialiasingHint(QCPPainter *painter) const {
    applyAntialiasingHint(painter, mAntialiased, QCP::aeItems);
}

/*! \internal

  A convenience function which returns the selectTest value for a specified \a rect and a specified
  click position \a pos. \a filledRect defines whether a click inside the rect should also be
  considered a hit or whether only the rect border is sensitive to hits.

  This function may be used to help with the implementation of the \ref selectTest function for
  specific items.

  For example, if your item consists of four rects, call this function four times, once for each
  rect, in your \ref selectTest reimplementation. Finally, return the minimum (non -1) of all four
  returned values.
*/
double QCPAbstractItem::rectDistance(const QRectF &rect, const QPointF &pos, bool filledRect) const {
    double result = -1;

// distance to border:
    const QList<QLineF> lines =
            QList<QLineF>() << QLineF(rect.topLeft(), rect.topRight()) << QLineF(rect.bottomLeft(), rect.bottomRight())
                            << QLineF(rect.topLeft(), rect.bottomLeft()) << QLineF(rect.topRight(), rect.bottomRight());
    const QCPVector2D posVec(pos);
    double minDistSqr = (std::numeric_limits<double>::max)();
            foreach (const auto &line, lines) {
            double distSqr = posVec.distanceSquaredToLine(QCPVector2D(line.p1()), QCPVector2D(line.p2()));
            if (distSqr < minDistSqr)
                minDistSqr = distSqr;
        }
    result = qSqrt(minDistSqr);

// filled rect, allow click inside to count as hit:
    if (filledRect && result > mParentPlot->selectionTolerance() * 0.99f) {
        if (rect.contains(pos)) {
            result = mParentPlot->selectionTolerance() * 0.99f;
        }
    }
    return result;
}

/*! \internal

  Returns the pixel position of the anchor with Id \a anchorId. This function must be reimplemented in
  item subclasses if they want to provide anchors (QCPItemAnchor).

  For example, if the item has two anchors with id 0 and 1, this function takes one of these anchor
  ids and returns the respective pixel points of the specified anchor.

  \see createAnchor
*/
QPointF QCPAbstractItem::anchorPixelPosition(int anchorId) const {
    qDebug() << Q_FUNC_INFO
             << "called on item which shouldn't have any anchors (this method not reimplemented). anchorId" << anchorId;
    return {};
}

/*! \internal

  Creates a QCPItemPosition, registers it with this item and returns a pointer to it. The specified
  \a name must be a unique string that is usually identical to the variable name of the position
  member (This is needed to provide the name-based \ref position access to positions).

  Don't delete positions created by this function manually, as the item will take care of it.

  Use this function in the constructor (initialization list) of the specific item subclass to
  create each position member. Don't create QCPItemPositions with \b new yourself, because they
  won't be registered with the item properly.

  \see createAnchor
*/
QCPItemPosition *QCPAbstractItem::createPosition(const QString &name) {
    if (hasAnchor(name))
        qDebug() << Q_FUNC_INFO << "anchor/position with name exists already:" << name;
    QCPItemPosition *newPosition = new QCPItemPosition(mParentPlot, this, name);
    mPositions.append(newPosition);
    mAnchors.append(reinterpret_cast<QCPItemAnchor *>(newPosition)); // every position is also an anchor
    newPosition->setAxes(mParentPlot->xAxis, mParentPlot->yAxis);
    newPosition->setType(QCPItemPosition::ptPlotCoords);
    if (mParentPlot->axisRect())
        newPosition->setAxisRect(mParentPlot->axisRect());
    newPosition->setCoords(0, 0);
    return newPosition;
}

/*! \internal

  Creates a QCPItemAnchor, registers it with this item and returns a pointer to it. The specified
  \a name must be a unique string that is usually identical to the variable name of the anchor
  member (This is needed to provide the name based \ref anchor access to anchors).

  The \a anchorId must be a number identifying the created anchor. It is recommended to create an
  enum (e.g. "AnchorIndex") for this on each item that uses anchors. This id is used by the anchor
  to identify itself when it calls QCPAbstractItem::anchorPixelPosition. That function then returns
  the correct pixel coordinates for the passed anchor id.

  Don't delete anchors created by this function manually, as the item will take care of it.

  Use this function in the constructor (initialization list) of the specific item subclass to
  create each anchor member. Don't create QCPItemAnchors with \b new yourself, because then they
  won't be registered with the item properly.

  \see createPosition
*/
QCPItemAnchor *QCPAbstractItem::createAnchor(const QString &name, int anchorId) {
    if (hasAnchor(name))
        qDebug() << Q_FUNC_INFO << "anchor/position with name exists already:" << name;
    QCPItemAnchor *newAnchor = new QCPItemAnchor(mParentPlot, this, name, anchorId);
    mAnchors.append(newAnchor);
    return newAnchor;
}

/* inherits documentation from base class */
void
QCPAbstractItem::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged) {
    Q_UNUSED(event)
    Q_UNUSED(details)
    if (mSelectable) {
        bool selBefore = mSelected;
        setSelected(additive ? !mSelected : true);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

/* inherits documentation from base class */
void QCPAbstractItem::deselectEvent(bool *selectionStateChanged) {
    if (mSelectable) {
        bool selBefore = mSelected;
        setSelected(false);
        if (selectionStateChanged)
            *selectionStateChanged = mSelected != selBefore;
    }
}

/* inherits documentation from base class */
QCP::Interaction QCPAbstractItem::selectionCategory() const {
    return QCP::iSelectItems;
}
