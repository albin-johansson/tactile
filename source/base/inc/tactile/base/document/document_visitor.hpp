// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class IComponentView;
class IMapView;
class ILayerView;
class IObjectView;
class ITilesetView;
class ITileView;

/**
 * A read-only document visitor.
 *
 * \details
 * The elements in a document are visited in a depth-first manner. In general,
 * the high-level order of iteration is: maps, tilesets, layers, components.
 *
 * \details
 * This interface is the sole mechanism by which external modules (i.e. plugins)
 * may inspect the individual elements in a document. Beware that all views are
 * short-lived objects, you cannot use the obtained views beyond the scope of
 * the functions of this interface!
 */
class IDocumentVisitor
{
 public:
  TACTILE_INTERFACE_CLASS(IDocumentVisitor);

  /**
   * Visits a component.
   *
   * \param component A component view.
   */
  virtual void visit(const IComponentView& component) = 0;

  /**
   * Visits a map.
   *
   * \param map A map view.
   */
  virtual void visit(const IMapView& map) = 0;

  /**
   * Visits a layer.
   *
   * \param layer A layer view.
   */
  virtual void visit(const ILayerView& layer) = 0;

  /**
   * Visits an object.
   *
   * \param object An object view.
   */
  virtual void visit(const IObjectView& object) = 0;

  /**
   * Visits a tileset.
   *
   * \param tileset A tileset view.
   */
  virtual void visit(const ITilesetView& tileset) = 0;

  /**
   * Visits a tile in a tileset.
   *
   * \param tile A tile view.
   */
  virtual void visit(const ITileView& tile) = 0;
};

}  // namespace tactile
