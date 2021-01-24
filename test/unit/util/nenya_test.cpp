#include "nenya.hpp"

#include <gtest/gtest.h>

#include <optional>  // optional
#include <string>    // string

namespace tags {
struct string_t;
struct integer_t;
struct boolean_t;
struct maybe_t;
}  // namespace tags

using string = nenya::mirror_type<std::string, tags::string_t>;
using integer = nenya::mirror_type<int, tags::integer_t>;
using boolean = nenya::mirror_type<bool, tags::boolean_t>;

template <typename T>
using maybe = nenya::mirror_type<std::optional<T>, tags::maybe_t>;

TEST(Nenya, Defaults)
{
  const string str;
  const integer i;  // Should be zero-initialised
  const boolean b;  // Should be zero-initialised

  EXPECT_TRUE(str->empty());
  EXPECT_EQ(0, i.get());
  EXPECT_FALSE(b);
}

TEST(Nenya, ExplicitConversions)
{
  const string str{"foo"};
  const integer i{42};
  const boolean b{true};

  EXPECT_EQ("foo", static_cast<std::string>(str));
  EXPECT_EQ(42, static_cast<int>(i));
  EXPECT_TRUE(static_cast<bool>(b));

  static_assert(!noexcept(static_cast<std::string>(str)));
  static_assert(noexcept(static_cast<int>(i)));
  static_assert(noexcept(static_cast<bool>(b)));
}

TEST(Nenya, Addition)
{
  {  // Integers
    const int a{42};
    const int b{7};
    const auto sum = a + b;

    const integer aa{a};
    const integer bb{b};

    EXPECT_EQ(integer{sum}, aa + bb);
    static_assert(noexcept(aa + bb));
  }

  {  // Strings
    const std::string a{"foo"};
    const std::string b{"bar"};
    const auto concatenated = a + b;

    const string aa{a};
    const string bb{b};

    EXPECT_EQ(string{concatenated}, aa + bb);
    static_assert(!noexcept(aa + bb));
  }
}

TEST(Nenya, Subtraction)
{
  const int a{42};
  const int b{7};
  const auto ab = a - b;
  const auto ba = b - a;

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{ab}, aa - bb);
  EXPECT_EQ(integer{ba}, bb - aa);
  static_assert(noexcept(aa - bb));
}

TEST(Nenya, Division)
{
  const int a{42};
  const int b{7};
  const auto quotient = a / b;

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{quotient}, aa / bb);
  static_assert(noexcept(aa / bb));
}

TEST(Nenya, Multiplication)
{
  const int a{42};
  const int b{7};
  const auto product = a * b;

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{product}, aa * bb);
  static_assert(noexcept(aa * bb));
}

TEST(Nenya, Mod)
{
  const int a{42};
  const int b{7};
  const auto mod = a % b;

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{mod}, aa % bb);
  static_assert(noexcept(aa % bb));
}

TEST(Nenya, BitwiseAND)
{
  const int a{42};
  const int b{7};
  const auto res = a & b;  // NOLINT

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{res}, aa & bb);
  static_assert(noexcept(aa & bb));
}

TEST(Nenya, BitwiseOR)
{
  const int a{42};
  const int b{7};
  const auto res = a | b;  // NOLINT

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{res}, aa | bb);
  static_assert(noexcept(aa | bb));
}

TEST(Nenya, BitwiseXOR)
{
  const int a{42};
  const int b{7};
  const auto res = a ^ b;  // NOLINT

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{res}, aa ^ bb);
  static_assert(noexcept(aa ^ bb));
}

TEST(Nenya, LeftShift)
{
  const int a{42};
  const int b{7};
  const auto shift = a << b;  // NOLINT

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{shift}, aa << bb);
  static_assert(noexcept(aa << bb));
}

TEST(Nenya, RightShift)
{
  const int a{42};
  const int b{7};
  const auto shift = a >> b;  // NOLINT

  const integer aa{a};
  const integer bb{b};

  EXPECT_EQ(integer{shift}, aa >> bb);
  static_assert(noexcept(aa >> bb));
}

TEST(Nenya, UnaryMinus)
{
  const int a{42};
  const auto res = -a;

  const integer aa{a};

  EXPECT_EQ(integer{res}, -aa);
  static_assert(noexcept(-aa));
}

TEST(Nenya, AdditionAssignment)
{
  {  // Integers
    int a{42};
    const int b{7};

    integer aa{a};
    const integer bb{b};

    a += b;
    aa += bb;

    EXPECT_EQ(integer{a}, aa);
    static_assert(noexcept(aa += bb));
  }

  {  // Strings
    std::string a{"foo"};
    const std::string b{"bar"};

    string aa{a};
    const string bb{b};

    a += b;
    aa += bb;

    EXPECT_EQ(string{a}, aa);
    static_assert(!noexcept(aa += bb));
  }
}

TEST(Nenya, SubtractionAssignment)
{
  int a{42};
  const int b{7};

  integer aa{a};
  const integer bb{b};

  a -= b;
  aa -= bb;

  EXPECT_EQ(integer{a}, aa);
  static_assert(noexcept(aa -= bb));
}

TEST(Nenya, DivisionAssignment)
{
  int a{42};
  const int b{7};

  integer aa{a};
  const integer bb{b};

  a /= b;
  aa /= bb;

  EXPECT_EQ(integer{a}, aa);
  static_assert(noexcept(aa /= bb));
}

TEST(Nenya, MultiplicationAssignment)
{
  int a{42};
  const int b{7};

  integer aa{a};
  const integer bb{b};

  a *= b;
  aa *= bb;

  EXPECT_EQ(integer{a}, aa);
  static_assert(noexcept(aa *= bb));
}

TEST(Nenya, ModuloAssignment)
{
  int a{42};
  const int b{7};

  integer aa{a};
  const integer bb{b};

  a %= b;
  aa %= bb;

  EXPECT_EQ(integer{a}, aa);
  static_assert(noexcept(aa %= bb));
}

TEST(Nenya, BitwiseANDAssignment)
{
  int a{42};
  const int b{7};

  integer aa{a};
  const integer bb{b};

  a &= b;  // NOLINT
  aa &= bb;

  EXPECT_EQ(integer{a}, aa);
  static_assert(noexcept(aa &= bb));
}

TEST(Nenya, BitwiseORAssignment)
{
  int a{42};
  const int b{7};

  integer aa{a};
  const integer bb{b};

  a |= b;  // NOLINT
  aa |= bb;

  EXPECT_EQ(integer{a}, aa);
  static_assert(noexcept(aa |= bb));
}

TEST(Nenya, LeftShiftAssignment)
{
  int a{42};
  const int b{7};

  integer aa{a};
  const integer bb{b};

  a <<= b;  // NOLINT
  aa <<= bb;

  EXPECT_EQ(integer{a}, aa);
  static_assert(noexcept(aa <<= bb));
}

TEST(Nenya, RightShiftAssignment)
{
  int a{42};
  const int b{7};

  integer aa{a};
  const integer bb{b};

  a >>= b;  // NOLINT
  aa >>= bb;

  EXPECT_EQ(integer{a}, aa);
  static_assert(noexcept(aa >>= bb));
}

TEST(Nenya, Subscript)
{
  {  // Non-const
    string str{"foo"};
    EXPECT_EQ('f', str[0]);

    str[0] = 'z';
    EXPECT_EQ('z', str[0]);
  }

  {  // Const
    const string str{"foo"};
    EXPECT_EQ('f', str[0]);
  }
}

TEST(Nenya, BoolConversion)
{
  {  // Defaults
    const boolean b;
    const maybe<int> mi;

    EXPECT_FALSE(b);
    EXPECT_FALSE(mi);

    static_assert(noexcept(b.operator bool()));
    static_assert(noexcept(mi.operator bool()));
  }

  {  // With custom values
    const boolean b{true};
    const maybe<int> mi{123};

    EXPECT_TRUE(b);
    EXPECT_TRUE(mi);
  }
}

TEST(Nenya, PointerOperator)
{
  const maybe<std::string> str{"foo"};
  EXPECT_TRUE(str->has_value());
}

TEST(Nenya, EqualityOperator)
{
  {  // Integers
    const integer a{42};
    EXPECT_EQ(a, a);

    const integer b{a};
    EXPECT_EQ(a, b);
    EXPECT_EQ(b, a);

    const integer c{123};
    EXPECT_FALSE(a == c);
    EXPECT_FALSE(c == a);

    static_assert(noexcept(a == b));
  }

  {  // Strings
    const string a{"foo"};
    EXPECT_EQ(a, a);

    const string b{a};
    EXPECT_EQ(a, b);
    EXPECT_EQ(b, a);

    const string c{"bar"};
    EXPECT_FALSE(a == c);
    EXPECT_FALSE(c == a);

    static_assert(noexcept(a == b));
  }
}

TEST(Nenya, InequalityOperator)
{
  {  // Integers
    const integer a{42};
    EXPECT_EQ(a, a);

    const integer b{a};
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(b != a);

    const integer c{123};
    EXPECT_NE(a, c);
    EXPECT_NE(c, a);

    static_assert(noexcept(a != b));
  }

  {  // Strings
    const string a{"foo"};
    EXPECT_EQ(a, a);

    const string b{a};
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(b != a);

    const string c{"bar"};
    EXPECT_NE(a, c);
    EXPECT_NE(c, a);

    static_assert(noexcept(a != b));
  }
}

TEST(Nenya, LessThanOperator)
{
  {  // Integers
    const integer a{41};
    EXPECT_FALSE(a < a);

    const integer b{42};
    EXPECT_LT(a, b);

    const integer c{40};
    EXPECT_FALSE(a < c);

    static_assert(noexcept(a < b));
  }

  {  // Strings
    const string a{"y"};
    EXPECT_FALSE(a < a);

    const string b{"z"};
    EXPECT_LT(a, b);

    const string c{"x"};
    EXPECT_FALSE(a < c);

    static_assert(noexcept(a < b));
  }
}

TEST(Nenya, LessThanOrEqualOperator)
{
  {  // Integers
    const integer a{41};
    EXPECT_LE(a, a);

    const integer b{42};
    EXPECT_LE(a, b);

    const integer c{40};
    EXPECT_FALSE(a <= c);

    static_assert(noexcept(a <= b));
  }

  {  // Strings
    const string a{"y"};
    EXPECT_LE(a, a);

    const string b{"z"};
    EXPECT_LE(a, b);

    const string c{"x"};
    EXPECT_FALSE(a <= c);

    static_assert(noexcept(a <= b));
  }
}

TEST(Nenya, GreaterThanOperator)
{
  {  // Integers
    const integer a{7};
    EXPECT_FALSE(a > a);

    const integer b{6};
    EXPECT_GT(a, b);

    const integer c{8};
    EXPECT_FALSE(a > c);

    static_assert(noexcept(a > b));
  }

  {  // Strings
    const string a{"y"};
    EXPECT_FALSE(a > a);

    const string b{"x"};
    EXPECT_GT(a, b);

    const string c{"z"};
    EXPECT_FALSE(a > c);

    static_assert(noexcept(a > b));
  }
}

TEST(Nenya, GreaterThanOrEqualOperator)
{
  {  // Integers
    const integer a{7};
    EXPECT_GE(a, a);

    const integer b{6};
    EXPECT_GE(a, b);

    const integer c{8};
    EXPECT_FALSE(a >= c);

    static_assert(noexcept(a >= b));
  }

  {  // Strings
    const string a{"y"};
    EXPECT_GE(a, a);

    const string b{"x"};
    EXPECT_GE(a, b);

    const string c{"z"};
    EXPECT_FALSE(a >= c);

    static_assert(noexcept(a >= b));
  }
}

TEST(Nenya, Get)
{
  const string str{"abc"};
  const integer i{7};

  EXPECT_EQ(std::string{"abc"}, str.get());
  EXPECT_EQ(7, i.get());
}
