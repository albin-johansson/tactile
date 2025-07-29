// Copyright (C) 2025 Albin Johansson

/// Provides the common logging API.
///
/// The primary API of this module is the Logger class. While the Logger class
/// isn't a singleton, there is a global logger instance, accessible via the
/// get_logger function. Tactile code should use this function to log
/// information.
///
/// Note that you need to install one or more "sinks" to the logger to see any
/// output from the logger.
///
/// This module cannot depend on any Tactile modules other than
/// "tactile.core.prelude" to make it usable within as many modules as possible.
export module tactile.core.log;

// export import :buffer;
export import :level;
export import :logger;
export import :sink;
