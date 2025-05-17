// Copyright (C) 2025 Albin Johansson

/// Provides APIs related to map layers.
///
/// The primary API exposed by this module is the ILayer interface. There are
/// currently three implementations of this interface: GroupLayer, TileLayer, and
/// AnnotationLayer.
export module tactile.core.layer;

export import :annotation_layer;
export import :group_layer;
export import :interfaces;
export import :layer_info;
export import :tile_layer;
