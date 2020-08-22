#include "nenya.hpp"

#include <catch.hpp>
#include <string>

using mirror_int = nenya::mirror_type<int, struct mirror_int_t>;
using mirror_str = nenya::mirror_type<std::string, struct mirror_str_t>;

TEST_CASE("mirror_type:: default values", "[nenya]")
{
  const mirror_int i;
  const mirror_str s;

  CHECK(i.get() == 0);
  CHECK(s.get() == "");  // NOLINT
}

TEST_CASE("mirror_type:: explicit conversions", "[nenya]")
{
  const mirror_int i{7};
  const auto j = static_cast<int>(i);

  CHECK(i.get() == j);
  static_assert(noexcept(static_cast<int>(i)));

  const mirror_str s{"foo"};
  const auto t = static_cast<std::string>(s);

  CHECK(s.get() == t);
  static_assert(!noexcept(static_cast<std::string>(s)));
}

TEST_CASE("mirror_type:: addition", "[nenya]")
{
  auto fst_i = GENERATE(7, -421, 831934);
  auto snd_i = GENERATE(3, 145, -173400);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto sum = fst + snd;

  CHECK(sum.get() == fst_i + snd_i);

  static_assert(noexcept(fst + snd));
}

TEST_CASE("mirror_type:: subtraction", "[nenya]")
{
  const int fst_i{15};
  const int snd_i{4};

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto diff = fst - snd;
  const auto diff_i = fst_i - snd_i;

  CHECK(diff.get() == 11);
  CHECK(diff.get() == diff_i);

  static_assert(noexcept(fst - snd));
}

TEST_CASE("mirror_type:: division", "[nenya]")
{
  auto i = GENERATE(45, -8);
  auto j = GENERATE(-73, 4);

  const mirror_int fst{i};
  const mirror_int snd{j};

  const auto fstOverSnd = fst / snd;
  const auto sndOverFst = snd / fst;

  CHECK(fstOverSnd.get() == i / j);
  CHECK(sndOverFst.get() == j / i);

  static_assert(noexcept(fst / snd));
}

TEST_CASE("mirror_type:: multiplication", "[nenya]")
{
  auto fst_i = GENERATE(4, -58);
  auto snd_i = GENERATE(6, 39);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto product = fst * snd;

  CHECK(product.get() == fst_i * snd_i);

  static_assert(noexcept(fst * snd));
}

TEST_CASE("mirror_type:: modulo", "[nenya]")
{
  auto fst_i = GENERATE(145, 4);
  auto snd_i = GENERATE(64, 27);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto mod = fst % snd;

  CHECK(mod.get() == fst_i % snd_i);

  static_assert(noexcept(fst % snd));
}

TEST_CASE("mirror_type:: bitwise AND", "[nenya]")
{
  auto fst_i = GENERATE(492, 92);
  auto snd_i = GENERATE(8124, 492);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto result = fst & snd;

  CHECK(result.get() == (fst_i & snd_i));  // NOLINT

  static_assert(noexcept(fst & snd));
}

TEST_CASE("mirror_type:: bitwise OR", "[nenya]")
{
  auto fst_i = GENERATE(127, 831);
  auto snd_i = GENERATE(434, 127);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto result = fst | snd;

  CHECK(result.get() == (fst_i | snd_i));  // NOLINT

  static_assert(noexcept(fst | snd));
}

TEST_CASE("mirror_type:: XOR", "[nenya]")
{
  auto fst_i = GENERATE(0b1010, 0b0111);
  auto snd_i = GENERATE(0b1100, 0b1010);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto result = fst ^ snd;

  CHECK(result.get() == (fst_i ^ snd_i));  // NOLINT

  static_assert(noexcept(fst ^ snd));
}

TEST_CASE("mirror_type:: left shift", "[nenya]")
{
  auto fst_i = GENERATE(0b0001, 0b1011);
  auto snd_i = GENERATE(0b1100, 0b0001);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto result = fst << snd;

  CHECK(result.get() == (fst_i << snd_i));  // NOLINT

  static_assert(noexcept(fst << snd));
}

TEST_CASE("mirror_type:: right shift", "[nenya]")
{
  auto fst_i = GENERATE(0b1101, 0b1011);
  auto snd_i = GENERATE(0b0001, 0b1101);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto result = fst >> snd;

  CHECK(result.get() == (fst_i >> snd_i));  // NOLINT

  static_assert(noexcept(fst >> snd));
}

TEST_CASE("mirror_type:: unary minus", "[nenya]")
{
  const int val_i{123};
  const mirror_int val{val_i};

  const auto inv = -val;
  const auto inv_i = -val_i;

  CHECK(inv.get() == -123);
  CHECK(inv.get() == inv_i);

  static_assert(noexcept(-val));
}

TEST_CASE("mirror_type:: addition assignment", "[nenya]")
{
  SECTION("int")
  {
    auto fst_i = GENERATE(391, -123);
    auto snd_i = GENERATE(-712, 839);

    mirror_int fst{fst_i};

    const mirror_int snd{snd_i};

    fst += snd;

    CHECK(fst.get() == fst_i + snd_i);
    CHECK(snd.get() == snd_i);

    static_assert(noexcept(fst += snd));
  }

  SECTION("string")
  {
    std::string foo{"foo"};
    const std::string bar{"bar"};

    mirror_str foo_str{foo};
    const mirror_str bar_str{bar};

    foo_str += bar_str;

    CHECK(foo_str.get() == foo + bar);
    CHECK(bar_str.get() == bar);

    static_assert(!noexcept(foo_str += bar_str));
  }
}

TEST_CASE("mirror_type:: subtraction assignment", "[nenya]")
{
  auto i = GENERATE(123, -321);
  auto j = GENERATE(-931, 483);

  mirror_int fst{i};
  const mirror_int snd{j};

  fst -= snd;

  CHECK(fst.get() == i - j);
  CHECK(snd.get() == j);

  static_assert(noexcept(fst -= snd));
}

TEST_CASE("mirror_type:: division assignment", "[nenya]")
{
  auto fst_i = GENERATE(9123, -18);
  auto snd_i = GENERATE(31, 1233);

  mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  fst /= snd;

  CHECK(fst.get() == fst_i / snd_i);
  CHECK(snd.get() == snd_i);

  static_assert(noexcept(fst /= snd));
}

TEST_CASE("mirror_type:: multiplication assignment", "[nenya]")
{
  auto fst_i = GENERATE(81, -17);
  auto snd_i = GENERATE(4, 32);

  mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  fst *= snd;

  CHECK(fst.get() == fst_i * snd_i);
  CHECK(snd.get() == snd_i);

  static_assert(noexcept(fst *= snd));
}

TEST_CASE("mirror_type:: subscript", "[nenya]")
{
  SECTION("Not const")
  {
    mirror_str str{"foo"};

    char& ch = str[0];
    CHECK(ch == 'f');

    ch = 'b';
    CHECK(ch == 'b');

    CHECK(str.get() == "boo");

    static_assert(noexcept(str[0]));
  }

  SECTION("Const")
  {
    const mirror_str str{"bar"};
    const char& ch = str[0];
    CHECK(ch == 'b');

    static_assert(noexcept(str[0]));
  }
}

TEST_CASE("mirror_type:: modulo assignment", "[nenya]")
{
  auto fst_i = GENERATE(1882, -117);
  auto snd_i = GENERATE(339, 3873);

  mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  fst %= snd;

  CHECK(fst.get() == fst_i % snd_i);
  CHECK(snd.get() == snd_i);

  static_assert(noexcept(fst %= snd));
}

TEST_CASE("mirror_type:: bitwise AND assignment", "[nenya]")
{
  auto fst_i = GENERATE(812, -441);
  auto snd_i = GENERATE(33123, 812);

  mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  fst &= snd;

  CHECK(fst.get() == (fst_i & snd_i));  // NOLINT
  CHECK(snd.get() == snd_i);

  static_assert(noexcept(fst &= snd));
}

TEST_CASE("mirror_type:: bitwise OR assignment", "[nenya]")
{
  auto fst_i = GENERATE(8193, -994);
  auto snd_i = GENERATE(3314, 8193);

  mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  fst |= snd;

  CHECK(fst.get() == (fst_i | snd_i));  // NOLINT
  CHECK(snd.get() == snd_i);

  static_assert(noexcept(fst |= snd));
}

TEST_CASE("mirror_type:: left shift assignment", "[nenya]")
{
  auto fst_i = GENERATE(546, -912);
  auto snd_i = GENERATE(1234, 31234);

  mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  fst <<= snd;

  CHECK(fst.get() == (fst_i << snd_i));  // NOLINT
  CHECK(snd.get() == snd_i);

  static_assert(noexcept(fst <<= snd));
}

TEST_CASE("mirror_type:: right shift assignment", "[nenya]")
{
  auto fst_i = GENERATE(4812, -415);
  auto snd_i = GENERATE(5467, 4812);

  mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  fst >>= snd;

  CHECK(fst.get() == (fst_i >> snd_i));  // NOLINT
  CHECK(snd.get() == snd_i);

  static_assert(noexcept(fst >>= snd));
}

TEST_CASE("mirror_type:: equality operator", "[nenya]")
{
  SECTION("int")
  {
    const mirror_int i{8124};
    const mirror_int j{i};
    CHECK(i == j);
    CHECK(j == i);

    const mirror_int k{1834};
    CHECK_FALSE(i == k);
    CHECK_FALSE(k == i);

    static_assert(noexcept(i == j));
  }

  SECTION("string")
  {
    const mirror_str a{"foo"};
    const mirror_str b{a};  // NOLINT
    CHECK(a == b);
    CHECK(b == a);

    const mirror_str c{"bar"};
    CHECK_FALSE(a == c);
    CHECK_FALSE(c == a);

    static_assert(noexcept(a == c));
  }
}

TEST_CASE("mirror_type:: inequality operator", "[nenya]")
{
  SECTION("int")
  {
    const mirror_int i{8124};
    const mirror_int j{482};
    CHECK(i != j);
    CHECK(j != i);

    const mirror_int k{i};
    CHECK_FALSE(i != k);
    CHECK_FALSE(k != i);

    static_assert(noexcept(i != j));
  }

  SECTION("string")
  {
    const mirror_str a{"foo"};
    const mirror_str b{"bar"};
    CHECK(a != b);
    CHECK(b != a);

    const mirror_str c{a};  // NOLINT
    CHECK_FALSE(a != c);
    CHECK_FALSE(c != a);

    static_assert(noexcept(a != b));
  }
}

TEST_CASE("mirror_type:: less-than operator", "[nenya]")
{
  SECTION("int")
  {
    const mirror_int i{123};
    const mirror_int j{321};
    CHECK(i < j);
    CHECK_FALSE(j < i);

    static_assert(noexcept(i < j));
  }

  SECTION("string")
  {
    const mirror_str a{"abb"};
    const mirror_str b{"abc"};
    CHECK(a < b);
    CHECK_FALSE(b < a);

    static_assert(noexcept(a < b));
  }
}

TEST_CASE("mirror_type:: less-than-or-equals operator", "[nenya]")
{
  SECTION("int")
  {
    const mirror_int i{123};
    const mirror_int j{321};
    CHECK(i <= j);
    CHECK_FALSE(j <= i);

    const mirror_int k{i};
    CHECK(i >= k);

    static_assert(noexcept(i <= j));
  }

  SECTION("string")
  {
    const mirror_str a{"abb"};
    const mirror_str b{"abc"};
    CHECK(a <= b);
    CHECK_FALSE(b <= a);

    const mirror_str c{a};  // NOLINT
    CHECK(a >= c);

    static_assert(noexcept(a <= b));
  }
}

TEST_CASE("mirror_type:: greater-than operator", "[nenya]")
{
  SECTION("int")
  {
    const mirror_int i{8391};
    const mirror_int j{583};
    CHECK(i > j);
    CHECK_FALSE(j > i);

    static_assert(noexcept(i > j));
  }

  SECTION("string")
  {
    const mirror_str a{"bbb"};
    const mirror_str b{"aaa"};
    CHECK(a > b);
    CHECK_FALSE(b > a);

    static_assert(noexcept(a > b));
  }
}

TEST_CASE("mirror_type:: greater-than-or-equals operator", "[nenya]")
{
  SECTION("int")
  {
    const mirror_int i{8391};
    const mirror_int j{583};

    CHECK(i >= j);
    CHECK_FALSE(j >= i);

    const mirror_int k{i};
    CHECK(i >= k);

    static_assert(noexcept(i >= j));
  }

  SECTION("string")
  {
    const mirror_str a{"bbb"};
    const mirror_str b{"aaa"};

    CHECK(a >= b);
    CHECK_FALSE(b >= a);

    const mirror_str c{a};  // NOLINT
    CHECK(a >= c);

    static_assert(noexcept(a >= b));
  }
}