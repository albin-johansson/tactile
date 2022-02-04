#include "crash.hpp"

#include <fstream>  // ofstream
#include <ios>      // ios

namespace tactile {

void dump_crash_info(const boost::stacktrace::stacktrace& trace)
{
  std::ofstream stream{"crash.txt", std::ios::out | std::ios::trunc};
  stream << trace;
}

}  // namespace tactile