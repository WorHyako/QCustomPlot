#include "include/QCPLayer.hpp"

#include <QDebug>

using namespace QCP;

/*! \class QCPLayer
  \brief A layer that may contain objects, to control the rendering order

  The Layering system of QCustomPlot is the mechanism to control the rendering order of the
  elements inside the plot.

  It is based on the two classes QCPLayer and QCPLayerable. QCustomPlot holds an ordered list of
  one or more instances of QCPLayer (see QCustomPlot::addLayer, QCustomPlot::layer,
  QCustomPlot::moveLayer, etc.). When replotting, QCustomPlot goes through the list of layers
  bottom to top and successively draws the layerables of the layers into the paint buffer(s).

  A QCPLayer contains an ordered list of QCPLayerable instances. QCPLayerable is an abstract base
  class from which almost all visible objects derive, like axes, grids, graphs, items, etc.

  \section qcplayer-defaultlayers Default layers

  Initially, QCustomPlot has six layers: "background", "grid", "main", "axes", "legend" and
  "overlay" (in that order). On top is the "overlay" layer, which only contains the QCustomPlot's
  selection rect (\ref QCustomPlot::selectionRect). The next two layers "axes" and "legend" contain
  the default axes and legend, so they will be drawn above plottables. In the middle, there is the
  "main" layer. It is initially empty and set as the current layer (see
  QCustomPlot::setCurrentLayer). This means, all new plottables, items etc. are created on this
  layer by default. Then comes the "grid" layer which contains the QCPGrid instances (which belong
  tightly to QCPAxis, see \ref QCPAxis::grid). The Axis rect background shall be drawn behind
  everything else, thus the default QCPAxisRect instance is placed on the "background" layer. Of
  course, the layer affiliation of the individual objects can be changed as required (\ref
  QCPLayerable::setLayer).

  \section qcplayer-ordering Controlling the rendering order via layers

  Controlling the ordering of layerables in the plot is easy: Create a new layer in the position
  you want the layerable to be in, e.g. above "main", with \ref QCustomPlot::addLayer. Then set the
  current layer with \ref QCustomPlot::setCurrentLayer to that new layer and finally create the
  objects normally. They will be placed on the new layer automatically, due to the current layer
  setting. Alternatively you could have also ignored the current layer setting and just moved the
  objects with \ref QCPLayerable::setLayer to the desired layer after creating them.

  It is also possible to move whole layers. For example, If you want the grid to be shown in front
  of all plottables/items on the "main" layer, just move it above "main" with
  QCustomPlot::moveLayer.

  The rendering order within one layer is simply by order of creation or insertion. The item
  created last (or added last to the layer), is drawn on top of all other objects on that layer.

  When a layer is deleted, the objects on it are not deleted with it, but fall on the layer below
  the deleted layer, see QCustomPlot::removeLayer.

  \section qcplayer-buffering Replotting only a specific layer

  If the layer mode (\ref setMode) is set to \ref lmBuffered, you can replot only this specific
  layer by calling \ref replot. In certain situations this can provide better replot performance,
  compared with a full replot of all layers. Upon creation of a new layer, the layer mode is
  initialized to \ref lmLogical. The only layer that is set to \ref lmBuffered in a new \ref
  QCustomPlot instance is the "overlay" layer, containing the selection rect.
*/

/*! \fn QList<QCPLayerable*> QCPLayer::children() const

  Returns a list of all layerables on this layer. The order corresponds to the rendering order:
  layerables with higher indices are drawn above layerables with lower indices.
*/

/*! \fn int QCPLayer::index() const

  Returns the index this layer has in the QCustomPlot. The index is the integer number by which this layer can be
  accessed via \ref QCustomPlot::layer.

  Layers with higher indices will be drawn above layers with lower indices.
*/

/*!
  Creates a new QCPLayer instance.

  Normally you shouldn't directly instantiate layers, use \ref QCustomPlot::addLayer instead.

  \warning It is not checked that \a layerName is actually a unique layer name in \a parentPlot.
  This check is only performed by \ref QCustomPlot::addLayer.
*/
QCPLayer::QCPLayer(QCustomPlot *parentPlot, const QString &layerName) :
        QObject(parentPlot),
        mParentPlot(parentPlot),
        mName(layerName),
        mIndex(-1), // will be set to a proper value by the QCustomPlot layer creation function
        mVisible(true),
        mMode(lmLogical) {
// Note: no need to make sure layerName is unique, because layer
// management is done with QCustomPlot functions.
}

QCPLayer::~QCPLayer() {
// If child layerables are still on this layer, detach them, so they don't try to reach back to this
// then invalid layer once they get deleted/moved themselves. This only happens when layers are deleted
// directly, like in the QCustomPlot destructor. (The regular layer removal procedure for the user is to
// call QCustomPlot::removeLayer, which moves all layerables off this layer before deleting it.)

    while (!mChildren.isEmpty())
        mChildren.last()->setLayer(nullptr); // removes itself from mChildren via removeChild()

    if (mParentPlot->currentLayer() == this)
        qDebug() << Q_FUNC_INFO
                 << "The parent plot's mCurrentLayer will be a dangling pointer. Should have been set to a valid layer or nullptr beforehand.";
}

/*!
  Sets whether this layer is visible or not. If \a visible is set to false, all layerables on this
  layer will be invisible.

  This function doesn't change the visibility property of the layerables (\ref
  QCPLayerable::setVisible), but the \ref QCPLayerable::realVisibility of each layerable takes the
  visibility of the parent layer into account.
*/
void QCPLayer::setVisible(bool visible) {
    mVisible = visible;
}

/*!
  Sets the rendering mode of this layer.

  If \a mode is set to \ref lmBuffered for a layer, it will be given a dedicated paint buffer by
  the parent QCustomPlot instance. This means it may be replotted individually by calling \ref
  QCPLayer::replot, without needing to replot all other layers.

  Layers which are set to \ref lmLogical (the default) are used only to define the rendering order
  and can't be replotted individually.

  Note that each layer which is set to \ref lmBuffered requires additional paint buffers for the
  layers below, above and for the layer itself. This increases the memory consumption and
  (slightly) decreases the repainting speed because multiple paint buffers need to be joined. So
  you should carefully choose which layers benefit from having their own paint buffer. A typical
  example would be a layer which contains certain layerables (e.g. items) that need to be changed
  and thus replotted regularly, while all other layerables on other layers stay static. By default,
  only the topmost layer called "overlay" is in mode \ref lmBuffered, and contains the selection
  rect.

  \see replot
*/
void QCPLayer::setMode(QCPLayer::LayerMode mode) {
    if (mMode != mode) {
        mMode = mode;
        if (QSharedPointer<QCPAbstractPaintBuffer> pb = mPaintBuffer.toStrongRef())
            pb->setInvalidated();
    }
}

/*! \internal

  Draws the contents of this layer with the provided \a painter.

  \see replot, drawToPaintBuffer
*/
void QCPLayer::draw(QCPPainter *painter) {
            foreach (QCPLayerable *child, mChildren) {
            if (child->realVisibility()) {
                painter->save();
                painter->setClipRect(child->clipRect().translated(0, -1));
                child->applyDefaultAntialiasingHint(painter);
                child->draw(painter);
                painter->restore();
            }
        }
}

/*! \internal

  Draws the contents of this layer into the paint buffer which is associated with this layer. The
  association is established by the parent QCustomPlot, which manages all paint buffers (see \ref
  QCustomPlot::setupPaintBuffers).

  \see draw
*/
void QCPLayer::drawToPaintBuffer() {
    if (QSharedPointer<QCPAbstractPaintBuffer> pb = mPaintBuffer.toStrongRef()) {
        if (QCPPainter *painter = pb->startPainting()) {
            if (painter->isActive())
                draw(painter);
            else
                qDebug() << Q_FUNC_INFO << "paint buffer returned inactive painter";
            delete painter;
            pb->donePainting();
        } else
            qDebug() << Q_FUNC_INFO << "paint buffer returned nullptr painter";
    } else
        qDebug() << Q_FUNC_INFO << "no valid paint buffer associated with this layer";
}

/*!
  If the layer mode (\ref setMode) is set to \ref lmBuffered, this method allows replotting only
  the layerables on this specific layer, without the need to replot all other layers (as a call to
  \ref QCustomPlot::replot would do).

  QCustomPlot also makes sure to replot all layers instead of only this one, if the layer ordering
  or any layerable-layer-association has changed since the last full replot and any other paint
  buffers were thus invalidated.

  If the layer mode is \ref lmLogical however, this method simply calls \ref QCustomPlot::replot on
  the parent QCustomPlot instance.

  \see draw
*/
void QCPLayer::replot() {
    if (mMode == lmBuffered && !mParentPlot->hasInvalidatedPaintBuffers()) {
        if (QSharedPointer<QCPAbstractPaintBuffer> pb = mPaintBuffer.toStrongRef()) {
            pb->clear(Qt::transparent);
            drawToPaintBuffer();
            pb->setInvalidated(
                    false); // since layer is lmBuffered, we know only this layer is on buffer and we can reset invalidated flag
            mParentPlot->update();
        } else
            qDebug() << Q_FUNC_INFO << "no valid paint buffer associated with this layer";
    } else
        mParentPlot->replot();
}

/*! \internal

  Adds the \a layerable to the list of this layer. If \a prepend is set to true, the layerable will
  be prepended to the list, i.e. be drawn beneath the other layerables already in the list.

  This function does not change the \a mLayer member of \a layerable to this layer. (Use
  QCPLayerable::setLayer to change the layer of an object, not this function.)

  \see removeChild
*/
void QCPLayer::addChild(QCPLayerable *layerable, bool prepend) {
    if (!mChildren.contains(layerable)) {
        if (prepend)
            mChildren.prepend(layerable);
        else
            mChildren.append(layerable);
        if (QSharedPointer<QCPAbstractPaintBuffer> pb = mPaintBuffer.toStrongRef())
            pb->setInvalidated();
    } else
        qDebug() << Q_FUNC_INFO << "layerable is already child of this layer" << reinterpret_cast<quintptr>(layerable);
}

/*! \internal

  Removes the \a layerable from the list of this layer.

  This function does not change the \a mLayer member of \a layerable. (Use QCPLayerable::setLayer
  to change the layer of an object, not this function.)

  \see addChild
*/
void QCPLayer::removeChild(QCPLayerable *layerable) {
    if (mChildren.removeOne(layerable)) {
        if (QSharedPointer<QCPAbstractPaintBuffer> pb = mPaintBuffer.toStrongRef())
            pb->setInvalidated();
    } else
        qDebug() << Q_FUNC_INFO << "layerable is not child of this layer" << reinterpret_cast<quintptr>(layerable);
}
