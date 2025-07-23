// Copyright (C) 2025 Albin Johansson

module;

#include <boost/safe_numerics/checked_integer.hpp>
#include <boost/safe_numerics/exception.hpp>

export module tactile.core.ext.boost.safe_numerics;

export namespace boost::safe_numerics {

using safe_numerics::safe_numerics_error;

namespace checked {
using checked::add;
using checked::subtract;
using checked::multiply;
using checked::divide;
}  // namespace checked

}  // namespace boost::safe_numerics
