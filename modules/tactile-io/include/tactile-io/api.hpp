#ifndef TACTILE_IO_API_HPP_
#define TACTILE_IO_API_HPP_

#ifdef WIN32
#ifdef TACTILE_IO_EXPORT
#define TACTILE_IO_API __declspec(dllexport)
#else
#define TACTILE_IO_API __declspec(dllimport)
#endif  // TACTILE_IO_EXPORT
#else
#define TACTILE_IO_API
#endif  // WIN32

#define TACTILE_IO_API_QUERY [[nodiscard]] TACTILE_IO_API

#endif  // TACTILE_IO_API_HPP_
