#pragma once

#ifdef WIN32
#ifdef TACTILE_IO_EXPORT
#define TACTILE_IO_API __declspec(dllexport)
#else
#define TACTILE_IO_API __declspec(dllimport)
#endif  // TACTILE_IO_EXPORT
#else
#define TACTILE_IO_API
#endif  // WIN32
