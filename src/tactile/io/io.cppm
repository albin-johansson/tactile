// Copyright (C) 2025 Albin Johansson

/// Provides the common I/O API.
///
/// Some basic functionality, such as Base64 encoding, is provided directly by
/// this module. More complicated aspects, such as save format serialization, is
/// exposed only via interfaces.
export module tactile.io;

export import :base64;
export import :compression;
