#pragma once

#include <fstream>  // ofstream
#include <ios>      // ios

#include <tactile_def.hpp>

namespace Tactile {

inline void WriteToFile(const CStr file, const CStr data)
{
  std::ofstream stream{file, std::ios::out | std::ios::trunc};
  stream << data;
}

}  // namespace Tactile
