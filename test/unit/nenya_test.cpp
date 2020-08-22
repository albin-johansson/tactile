#include "nenya.hpp"

#include <catch.hpp>
#include <string>

using mirror_int = nenya::mirror_type<int, struct mirror_int_t>;
using mirror_str = nenya::mirror_type<std::string, struct mirror_str_t>;

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

  CHECK(result.get() == (fst_i & snd_i)); // NOLINT

  static_assert(noexcept(fst & snd));
}

TEST_CASE("mirror_type:: bitwise OR", "[nenya]")
{
  auto fst_i = GENERATE(127, 831);
  auto snd_i = GENERATE(434, 127);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto result = fst | snd;

  CHECK(result.get() == (fst_i | snd_i)); // NOLINT

  static_assert(noexcept(fst | snd));
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

TEST_CASE("mirror_type:: multiplication assignment", "[nenya]")
{
  auto fst_i = GENERATE(81, -17);
  auto snd_i = GENERATE(4, 32);

  mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  fst *= snd;

  CHECK(fst.get() == fst_i * snd_i);
  CHECK(snd.get() == snd_i);

  static_assert(noexcept(fst * snd));
}
