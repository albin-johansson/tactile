// Copyright (C) 2025 Albin Johansson

/// Provides APIs related to metadata, such as properties and components.
///
/// Many objects, such as maps and layers, support user-defined properties and
/// components. A property, in this context, means a named value of some kind.
/// The Attr class is used to represent a single property value. A component is
/// a named collection of properties that can be directly attached to any object
/// that may feature properties.
export module tactile.core.meta;

export import :attr;
