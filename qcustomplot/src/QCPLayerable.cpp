#include "include/QCPLayerable.hpp"

namespace QCP {

/*! \class QCPLayerable
  \brief Base class for all drawable objects

  This is the abstract base class most visible objects derive from, e.g. plottables, axes, grid
  etc.

  Every layerable is on a layer (QCPLayer) which allows controlling the rendering order by stacking
  the layers accordingly.

  For details about the layering mechanism, see the QCPLayer documentation.
*/

/* start documentation of inline functions */

/*! \fn QCPLayerable *QCPLayerable::parentLayerable() const

  Returns the parent layerable of this layerable. The parent layerable is used to provide
  visibility hierarchies in conjunction with the method \ref realVisibility. This way, layerables
  only get drawn if their parent layerables are visible, too.

  Note that a parent layerable is not necessarily also the QObject parent for memory management.
  Further, a layerable doesn't always have a parent layerable, so this function may return \c
  nullptr.

  A parent layerable is set implicitly when placed inside layout elements and doesn't need to be
  set manually by the user.
*/

/* end documentation of inline functions */
/* start documentation of pure virtual functions */

/*! \fn virtual void QCPLayerable::applyDefaultAntialiasingHint(QCPPainter *painter) const = 0
  \internal

  This function applies the default antialiasing setting to the specified \a painter, using the
  function \ref applyAntialiasingHint. It is the antialiasing state the painter is put in, when
  \ref draw is called on the layerable. If the layerable has multiple entities whose antialiasing
  setting may be specified individually, this function should set the antialiasing state of the
  most prominent entity. In this case however, the \ref draw function usually calls the specialized
  versions of this function before drawing each entity, effectively overriding the setting of the
  default antialiasing hint.

  <b>First example:</b> QCPGraph has multiple entities that have an antialiasing setting: The graph
  line, fills and scatters. Those can be configured via QCPGraph::setAntialiased,
  QCPGraph::setAntialiasedFill and QCPGraph::setAntialiasedScatters. Consequently, there isn't only
  the QCPGraph::applyDefaultAntialiasingHint function (which corresponds to the graph line's
  antialiasing), but specialized ones like QCPGraph::applyFillAntialiasingHint and
  QCPGraph::applyScattersAntialiasingHint. So before drawing one of those entities, QCPGraph::draw
  calls the respective specialized applyAntialiasingHint function.

  <b>Second example:</b> QCPItemLine consists only of a line so there is only one antialiasing
  setting which can be controlled with QCPItemLine::setAntialiased. (This function is inherited by
  all layerables. The specialized functions, as seen on QCPGraph, must be added explicitly to the
  respective layerable subclass.) Consequently it only has the normal
  QCPItemLine::applyDefaultAntialiasingHint. The \ref QCPItemLine::draw function doesn't need to
  care about setting any antialiasing states, because the default antialiasing hint is already set
  on the painter when the \ref draw function is called, and that's the state it wants to draw the
  line with.
*/

/*! \fn virtual void QCPLayerable::draw(QCPPainter *painter) const = 0
  \internal

  This function draws the layerable with the specified \a painter. It is only called by
  QCustomPlot, if the layerable is visible (\ref setVisible).

  Before this function is called, the painter's antialiasing state is set via \ref
  applyDefaultAntialiasingHint, see the documentation there. Further, the clipping rectangle was
  set to \ref clipRect.
*/

/* end documentation of pure virtual functions */
/* start documentation of signals */

/*! \fn void QCPLayerable::layerChanged(QCPLayer *newLayer);

  This signal is emitted when the layer of this layerable changes, i.e. this layerable is moved to
  a different layer.

  \see setLayer
*/

/* end documentation of signals */

/*!
  Creates a new QCPLayerable instance.

  Since QCPLayerable is an abstract base class, it can't be instantiated directly. Use one of the
  derived classes.

  If \a plot is provided, it automatically places itself on the layer named \a targetLayer. If \a
  targetLayer is an empty string, it places itself on the current layer of the plot (see \ref
  QCustomPlot::setCurrentLayer).

  It is possible to provide \c nullptr as \a plot. In that case, you should assign a parent plot at
  a later time with \ref initializeParentPlot.

  The layerable's parent layerable is set to \a parentLayerable, if provided. Direct layerable
  parents are mainly used to control visibility in a hierarchy of layerables. This means a
  layerable is only drawn, if all its ancestor layerables are also visible. Note that \a
  parentLayerable does not become the QObject-parent (for memory management) of this layerable, \a
  plot does. It is not uncommon to set the QObject-parent to something else in the constructors of
  QCPLayerable subclasses, to guarantee a working destruction hierarchy.
*/
    QCPLayerable::QCPLayerable(QCustomPlot *plot, QString targetLayer, QCPLayerable *parentLayerable) :
            QObject(plot),
            mVisible(true),
            mParentPlot(plot),
            mParentLayerable(parentLayerable),
            mLayer(nullptr),
            mAntialiased(true)
    {
        if (mParentPlot)
        {
            if (targetLayer.isEmpty())
                setLayer(mParentPlot->currentLayer());
            else if (!setLayer(targetLayer))
                qDebug() << Q_FUNC_INFO << "setting QCPlayerable initial layer to" << targetLayer << "failed.";
        }
    }

    QCPLayerable::~QCPLayerable()
    {
        if (mLayer)
        {
            mLayer->removeChild(this);
            mLayer = nullptr;
        }
    }

/*!
  Sets the visibility of this layerable object. If an object is not visible, it will not be drawn
  on the QCustomPlot surface, and user interaction with it (e.g. click and selection) is not
  possible.
*/
    void QCPLayerable::setVisible(bool on)
    {
        mVisible = on;
    }

/*!
  Sets the \a layer of this layerable object. The object will be placed on top of the other objects
  already on \a layer.

  If \a layer is 0, this layerable will not be on any layer and thus not appear in the plot (or
  interact/receive events).

  Returns true if the layer of this layerable was successfully changed to \a layer.
*/
    bool QCPLayerable::setLayer(QCPLayer *layer)
    {
        return moveToLayer(layer, false);
    }

/*! \overload
  Sets the layer of this layerable object by name

  Returns true on success, i.e. if \a layerName is a valid layer name.
*/
    bool QCPLayerable::setLayer(const QString &layerName)
    {
        if (!mParentPlot)
        {
            qDebug() << Q_FUNC_INFO << "no parent QCustomPlot set";
            return false;
        }
        if (QCPLayer *layer = mParentPlot->layer(layerName))
        {
            return setLayer(layer);
        } else
        {
            qDebug() << Q_FUNC_INFO << "there is no layer with name" << layerName;
            return false;
        }
    }

/*!
  Sets whether this object will be drawn antialiased or not.

  Note that antialiasing settings may be overridden by QCustomPlot::setAntialiasedElements and
  QCustomPlot::setNotAntialiasedElements.
*/
    void QCPLayerable::setAntialiased(bool enabled)
    {
        mAntialiased = enabled;
    }

/*!
  Returns whether this layerable is visible, taking the visibility of the layerable parent and the
  visibility of this layerable's layer into account. This is the method that is consulted to decide
  whether a layerable shall be drawn or not.

  If this layerable has a direct layerable parent (usually set via hierarchies implemented in
  subclasses, like in the case of \ref QCPLayoutElement), this function returns true only if this
  layerable has its visibility set to true and the parent layerable's \ref realVisibility returns
  true.
*/
    bool QCPLayerable::realVisibility() const
    {
        return mVisible && (!mLayer || mLayer->visible()) && (!mParentLayerable || mParentLayerable.data()->realVisibility());
    }

/*!
  This function is used to decide whether a click hits a layerable object or not.

  \a pos is a point in pixel coordinates on the QCustomPlot surface. This function returns the
  shortest pixel distance of this point to the object. If the object is either invisible or the
  distance couldn't be determined, -1.0 is returned. Further, if \a onlySelectable is true and the
  object is not selectable, -1.0 is returned, too.

  If the object is represented not by single lines but by an area like a \ref QCPItemText or the
  bars of a \ref QCPBars plottable, a click inside the area should also be considered a hit. In
  these cases this function thus returns a constant value greater zero but still below the parent
  plot's selection tolerance. (typically the selectionTolerance multiplied by 0.99).

  Providing a constant value for area objects allows selecting line objects even when they are
  obscured by such area objects, by clicking close to the lines (i.e. closer than
  0.99*selectionTolerance).

  The actual setting of the selection state is not done by this function. This is handled by the
  parent QCustomPlot when the mouseReleaseEvent occurs, and the finally selected object is notified
  via the \ref selectEvent/\ref deselectEvent methods.

  \a details is an optional output parameter. Every layerable subclass may place any information
  in \a details. This information will be passed to \ref selectEvent when the parent QCustomPlot
  decides on the basis of this selectTest call, that the object was successfully selected. The
  subsequent call to \ref selectEvent will carry the \a details. This is useful for multi-part
  objects (like QCPAxis). This way, a possibly complex calculation to decide which part was clicked
  is only done once in \ref selectTest. The result (i.e. the actually clicked part) can then be
  placed in \a details. So in the subsequent \ref selectEvent, the decision which part was
  selected doesn't have to be done a second time for a single selection operation.

  In the case of 1D Plottables (\ref QCPAbstractPlottable1D, like \ref QCPGraph or \ref QCPBars) \a
  details will be set to a \ref QCPDataSelection, describing the closest data point to \a pos.

  You may pass \c nullptr as \a details to indicate that you are not interested in those selection
  details.

  \see selectEvent, deselectEvent, mousePressEvent, wheelEvent, QCustomPlot::setInteractions,
  QCPAbstractPlottable1D::selectTestRect
*/
    double QCPLayerable::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
    {
        Q_UNUSED(pos)
        Q_UNUSED(onlySelectable)
        Q_UNUSED(details)
        return -1.0;
    }

/*! \internal

  Sets the parent plot of this layerable. Use this function once to set the parent plot if you have
  passed \c nullptr in the constructor. It can not be used to move a layerable from one QCustomPlot
  to another one.

  Note that, unlike when passing a non \c nullptr parent plot in the constructor, this function
  does not make \a parentPlot the QObject-parent of this layerable. If you want this, call
  QObject::setParent(\a parentPlot) in addition to this function.

  Further, you will probably want to set a layer (\ref setLayer) after calling this function, to
  make the layerable appear on the QCustomPlot.

  The parent plot change will be propagated to subclasses via a call to \ref parentPlotInitialized
  so they can react accordingly (e.g. also initialize the parent plot of child layerables, like
  QCPLayout does).
*/
    void QCPLayerable::initializeParentPlot(QCustomPlot *parentPlot)
    {
        if (mParentPlot)
        {
            qDebug() << Q_FUNC_INFO << "called with mParentPlot already initialized";
            return;
        }

        if (!parentPlot)
            qDebug() << Q_FUNC_INFO << "called with parentPlot zero";

        mParentPlot = parentPlot;
        parentPlotInitialized(mParentPlot);
    }

/*! \internal

  Sets the parent layerable of this layerable to \a parentLayerable. Note that \a parentLayerable does not
  become the QObject-parent (for memory management) of this layerable.

  The parent layerable has influence on the return value of the \ref realVisibility method. Only
  layerables with a fully visible parent tree will return true for \ref realVisibility, and thus be
  drawn.

  \see realVisibility
*/
    void QCPLayerable::setParentLayerable(QCPLayerable *parentLayerable)
    {
        mParentLayerable = parentLayerable;
    }

/*! \internal

  Moves this layerable object to \a layer. If \a prepend is true, this object will be prepended to
  the new layer's list, i.e. it will be drawn below the objects already on the layer. If it is
  false, the object will be appended.

  Returns true on success, i.e. if \a layer is a valid layer.
*/
    bool QCPLayerable::moveToLayer(QCPLayer *layer, bool prepend)
    {
        if (layer && !mParentPlot)
        {
            qDebug() << Q_FUNC_INFO << "no parent QCustomPlot set";
            return false;
        }
        if (layer && layer->parentPlot() != mParentPlot)
        {
            qDebug() << Q_FUNC_INFO << "layer" << layer->name() << "is not in same QCustomPlot as this layerable";
            return false;
        }

        QCPLayer *oldLayer = mLayer;
        if (mLayer)
            mLayer->removeChild(this);
        mLayer = layer;
        if (mLayer)
            mLayer->addChild(this, prepend);
        if (mLayer != oldLayer)
            emit layerChanged(mLayer);
        return true;
    }

/*! \internal

  Sets the QCPainter::setAntialiasing state on the provided \a painter, depending on the \a
  localAntialiased value as well as the overrides \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements. Which override enum this function takes into account is
  controlled via \a overrideElement.
*/
    void QCPLayerable::applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const
    {
        if (mParentPlot && mParentPlot->notAntialiasedElements().testFlag(overrideElement))
            painter->setAntialiasing(false);
        else if (mParentPlot && mParentPlot->antialiasedElements().testFlag(overrideElement))
            painter->setAntialiasing(true);
        else
            painter->setAntialiasing(localAntialiased);
    }

/*! \internal

  This function is called by \ref initializeParentPlot, to allow subclasses to react on the setting
  of a parent plot. This is the case when \c nullptr was passed as parent plot in the constructor,
  and the parent plot is set at a later time.

  For example, QCPLayoutElement/QCPLayout hierarchies may be created independently of any
  QCustomPlot at first. When they are then added to a layout inside the QCustomPlot, the top level
  element of the hierarchy gets its parent plot initialized with \ref initializeParentPlot. To
  propagate the parent plot to all the children of the hierarchy, the top level element then uses
  this function to pass the parent plot on to its child elements.

  The default implementation does nothing.

  \see initializeParentPlot
*/
    void QCPLayerable::parentPlotInitialized(QCustomPlot *parentPlot)
    {
        Q_UNUSED(parentPlot)
    }

/*! \internal

  Returns the selection category this layerable shall belong to. The selection category is used in
  conjunction with \ref QCustomPlot::setInteractions to control which objects are selectable and
  which aren't.

  Subclasses that don't fit any of the normal \ref QCP::Interaction values can use \ref
  QCP::iSelectOther. This is what the default implementation returns.

  \see QCustomPlot::setInteractions
*/
    QCP::Interaction QCPLayerable::selectionCategory() const
    {
        return QCP::iSelectOther;
    }

/*! \internal

  Returns the clipping rectangle of this layerable object. By default, this is the viewport of the
  parent QCustomPlot. Specific subclasses may reimplement this function to provide different
  clipping rects.

  The returned clipping rect is set on the painter before the draw function of the respective
  object is called.
*/
    QRect QCPLayerable::clipRect() const
    {
        if (mParentPlot)
            return mParentPlot->viewport();
        else
            return {};
    }

/*! \internal

  This event is called when the layerable shall be selected, as a consequence of a click by the
  user. Subclasses should react to it by setting their selection state appropriately. The default
  implementation does nothing.

  \a event is the mouse event that caused the selection. \a additive indicates, whether the user
  was holding the multi-select-modifier while performing the selection (see \ref
  QCustomPlot::setMultiSelectModifier). if \a additive is true, the selection state must be toggled
  (i.e. become selected when unselected and unselected when selected).

  Every selectEvent is preceded by a call to \ref selectTest, which has returned positively (i.e.
  returned a value greater than 0 and less than the selection tolerance of the parent QCustomPlot).
  The \a details data you output from \ref selectTest is fed back via \a details here. You may
  use it to transport any kind of information from the selectTest to the possibly subsequent
  selectEvent. Usually \a details is used to transfer which part was clicked, if it is a layerable
  that has multiple individually selectable parts (like QCPAxis). This way selectEvent doesn't need
  to do the calculation again to find out which part was actually clicked.

  \a selectionStateChanged is an output parameter. If the pointer is non-null, this function must
  set the value either to true or false, depending on whether the selection state of this layerable
  was actually changed. For layerables that only are selectable as a whole and not in parts, this
  is simple: if \a additive is true, \a selectionStateChanged must also be set to true, because the
  selection toggles. If \a additive is false, \a selectionStateChanged is only set to true, if the
  layerable was previously unselected and now is switched to the selected state.

  \see selectTest, deselectEvent
*/
    void QCPLayerable::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
    {
        Q_UNUSED(event)
        Q_UNUSED(additive)
        Q_UNUSED(details)
        Q_UNUSED(selectionStateChanged)
    }

/*! \internal

  This event is called when the layerable shall be deselected, either as consequence of a user
  interaction or a call to \ref QCustomPlot::deselectAll. Subclasses should react to it by
  unsetting their selection appropriately.

  just as in \ref selectEvent, the output parameter \a selectionStateChanged (if non-null), must
  return true or false when the selection state of this layerable has changed or not changed,
  respectively.

  \see selectTest, selectEvent
*/
    void QCPLayerable::deselectEvent(bool *selectionStateChanged)
    {
        Q_UNUSED(selectionStateChanged)
    }

/*!
  This event gets called when the user presses a mouse button while the cursor is over the
  layerable. Whether a cursor is over the layerable is decided by a preceding call to \ref
  selectTest.

  The current pixel position of the cursor on the QCustomPlot widget is accessible via \c
  event->pos(). The parameter \a details contains layerable-specific details about the hit, which
  were generated in the previous call to \ref selectTest. For example, One-dimensional plottables
  like \ref QCPGraph or \ref QCPBars convey the clicked data point in the \a details parameter, as
  \ref QCPDataSelection packed as QVariant. Multi-part objects convey the specific \c
  SelectablePart that was hit (e.g. \ref QCPAxis::SelectablePart in the case of axes).

  QCustomPlot uses an event propagation system that works the same as Qt's system. If your
  layerable doesn't reimplement the \ref mousePressEvent or explicitly calls \c event->ignore() in
  its reimplementation, the event will be propagated to the next layerable in the stacking order.

  Once a layerable has accepted the \ref mousePressEvent, it is considered the mouse grabber and
  will receive all following calls to \ref mouseMoveEvent or \ref mouseReleaseEvent for this mouse
  interaction (a "mouse interaction" in this context ends with the release).

  The default implementation does nothing except explicitly ignoring the event with \c
  event->ignore().

  \see mouseMoveEvent, mouseReleaseEvent, mouseDoubleClickEvent, wheelEvent
*/
    void QCPLayerable::mousePressEvent(QMouseEvent *event, const QVariant &details)
    {
        Q_UNUSED(details)
        event->ignore();
    }

/*!
  This event gets called when the user moves the mouse while holding a mouse button, after this
  layerable has become the mouse grabber by accepting the preceding \ref mousePressEvent.

  The current pixel position of the cursor on the QCustomPlot widget is accessible via \c
  event->pos(). The parameter \a startPos indicates the position where the initial \ref
  mousePressEvent occurred, that started the mouse interaction.

  The default implementation does nothing.

  \see mousePressEvent, mouseReleaseEvent, mouseDoubleClickEvent, wheelEvent
*/
    void QCPLayerable::mouseMoveEvent(QMouseEvent *event, const QPointF &startPos)
    {
        Q_UNUSED(startPos)
        event->ignore();
    }

/*!
  This event gets called when the user releases the mouse button, after this layerable has become
  the mouse grabber by accepting the preceding \ref mousePressEvent.

  The current pixel position of the cursor on the QCustomPlot widget is accessible via \c
  event->pos(). The parameter \a startPos indicates the position where the initial \ref
  mousePressEvent occurred, that started the mouse interaction.

  The default implementation does nothing.

  \see mousePressEvent, mouseMoveEvent, mouseDoubleClickEvent, wheelEvent
*/
    void QCPLayerable::mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos)
    {
        Q_UNUSED(startPos)
        event->ignore();
    }

/*!
  This event gets called when the user presses the mouse button a second time in a double-click,
  while the cursor is over the layerable. Whether a cursor is over the layerable is decided by a
  preceding call to \ref selectTest.

  The \ref mouseDoubleClickEvent is called instead of the second \ref mousePressEvent. So in the
  case of a double-click, the event succession is
  <i>pressEvent &ndash; releaseEvent &ndash; doubleClickEvent &ndash; releaseEvent</i>.

  The current pixel position of the cursor on the QCustomPlot widget is accessible via \c
  event->pos(). The parameter \a details contains layerable-specific details about the hit, which
  were generated in the previous call to \ref selectTest. For example, One-dimensional plottables
  like \ref QCPGraph or \ref QCPBars convey the clicked data point in the \a details parameter, as
  \ref QCPDataSelection packed as QVariant. Multi-part objects convey the specific \c
  SelectablePart that was hit (e.g. \ref QCPAxis::SelectablePart in the case of axes).

  Similarly to \ref mousePressEvent, once a layerable has accepted the \ref mouseDoubleClickEvent,
  it is considered the mouse grabber and will receive all following calls to \ref mouseMoveEvent
  and \ref mouseReleaseEvent for this mouse interaction (a "mouse interaction" in this context ends
  with the release).

  The default implementation does nothing except explicitly ignoring the event with \c
  event->ignore().

  \see mousePressEvent, mouseMoveEvent, mouseReleaseEvent, wheelEvent
*/
    void QCPLayerable::mouseDoubleClickEvent(QMouseEvent *event, const QVariant &details)
    {
        Q_UNUSED(details)
        event->ignore();
    }

/*!
  This event gets called when the user turns the mouse scroll wheel while the cursor is over the
  layerable. Whether a cursor is over the layerable is decided by a preceding call to \ref
  selectTest.

  The current pixel position of the cursor on the QCustomPlot widget is accessible via \c
  event->pos().

  The \c event->angleDelta() indicates how far the mouse wheel was turned, which is usually +/- 120
  for single rotation steps. However, if the mouse wheel is turned rapidly, multiple steps may
  accumulate to one event, making the delta larger. On the other hand, if the wheel has very smooth
  steps or none at all, the delta may be smaller.

  The default implementation does nothing.

  \see mousePressEvent, mouseMoveEvent, mouseReleaseEvent, mouseDoubleClickEvent
*/
    void QCPLayerable::wheelEvent(QWheelEvent *event)
    {
        event->ignore();
    }

} // QCP