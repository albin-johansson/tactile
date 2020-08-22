#include "iron.hpp"

#include <catch.hpp>
#include <string>

using mirror_int = iron::mirror_type<int, struct mirror_int_t>;
using mirror_str = iron::mirror_type<std::string, struct mirror_str_t>;

TEST_CASE("mirror_type:: addition", "[iron]")
{
  auto fst_i = GENERATE(7, -421, 831934);
  auto snd_i = GENERATE(3, 145, -173400);

  const mirror_int fst{fst_i};
  const mirror_int snd{snd_i};

  const auto sum = fst + snd;

  CHECK(sum.get() == fst_i + snd_i);
}

TEST_CASE("mirror_type:: addition assignment", "[iron]")
{}

TEST_CASE("mirror_type:: subtraction", "[iron]")
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

TEST_CASE("mirror_type:: subtraction assignment", "[iron]")
{}

TEST_CASE("mirror_type:: unary minus", "[iron]")
{
  const int val_i{123};
  const mirror_int val{val_i};

  const auto inv = -val;
  const auto inv_i = -val_i;

  CHECK(inv.get() == -123);
  CHECK(inv.get() == inv_i);
}

TEST_CASE("mirror_type:: multiplication", "[iron]")
{
  const int fst_i{4};
  const int snd_i{6};

  mirror_int fst{fst_i};
  mirror_int snd{snd_i};

  const auto product = fst * snd;
  const auto product_i = fst_i * snd_i;

  CHECK(product.get() == 24);
  CHECK(product.get() == product_i);
}

TEST_CASE("mirror_type:: multiplication assignment", "[iron]")
{}
