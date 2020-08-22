#include "iron.hpp"

#include <catch.hpp>
#include <string>

using mirror_int = iron::mirror_type<int, struct mirror_int_t>;
using mirror_str = iron::mirror_type<std::string, struct mirror_str_t>;

TEST_CASE("iron mirror_type", "[iron]")
{
  SECTION("operator +")
  {
    auto fst_i = GENERATE(7, -421, 831934);
    auto snd_i = GENERATE(3, 145, -173400);

    const mirror_int fst{fst_i};
    const mirror_int snd{snd_i};

    const auto sum = fst + snd;

    CHECK(sum.get() == fst_i + snd_i);
  }

  SECTION("Binary operator-")
  {
    const int fst_i{15};
    const int snd_i{4};

    const mirror_int fst{fst_i};
    const mirror_int snd{snd_i};

    const auto diff = fst - snd;
    const auto diff_i = fst_i - snd_i;

    CHECK(diff.get() == 11);
    CHECK(diff.get() == diff_i);
  }

  SECTION("Unary operator-")
  {
    const int val_i{123};
    const mirror_int val{val_i};

    const auto inv = -val;
    const auto inv_i = -val_i;

    CHECK(inv.get() == -123);
    CHECK(inv.get() == inv_i);
  }

  SECTION("Binary operator*")
  {
    const int fst_i{4};
    const int snd_i{6};

    mirror_int fst{fst_i};
    mirror_int snd{snd_i};

    fst = snd;            // assigment
    auto a = fst + snd;   // addition
    fst += snd;           // addition assignment
    auto b = fst - snd;   // subtraction
    fst -= snd;           // subtraction assignment
    auto c = fst ^ snd;   // XOR
    fst ^= snd;           // XOR assignment
    auto d = fst & snd;   // AND
    fst &= snd;           // AND assignment
    auto e = fst | snd;   // OR
    fst |= snd;           // OR assignment
    auto f = fst % snd;   // Modulo
    fst %= snd;           // Modulo assignment
    auto o = fst << snd;  // left-shift
    fst <<= snd;          // left-shift assignment
    auto p = fst >> snd;  // right-shift
    fst >>= snd;          // right-shift assignment
    auto g = fst == snd;  // Equality
    auto h = fst != snd;  // Inequality
    auto i = fst < snd;   // less-than
    auto j = fst <= snd;  // less-than-equals
    auto k = fst > snd;   // greater-than
    auto l = fst >= snd;  // greater-than-equals
    auto q = !fst;        // logical negation
    auto r = ~fst;        // bitwise NOT

    const std::string s;
    const auto _ = s[0];

    //    mirror_str str{"foo"};
    //    auto strstr = str + str;
    //    str += mirror_str{"bar"};
    //    bool b = str == str;
    //    bool c = str != str;
    //
    //    const auto product = fst * snd;
    //    const auto product_i = fst_i * snd_i;
    //
    //    CHECK(product.get() == 24);
    //    CHECK(product.get() == product_i);
  }
  //  mi++;
  //  ++mi;
  //  auto div = mi / mi;
  //  auto mul = mi * mi;
  //  auto x_or = mi ^ mi;
  //  auto mod = mi % mi;
  //  auto neg = ~mi;
}