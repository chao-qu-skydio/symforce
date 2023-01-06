#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <symforce/opt/factor_keys.h>

TEST_CASE("Test FactorKeys constructor from keys only", "[factor_keys]") {
  sym::FactorKeys factor_keys({{'a'}, sym::Key{'b', 1}, sym::Key{'c', 1, 2}, {'d'}});
  CHECK(factor_keys.NumKeys() == 4);
  CHECK(factor_keys.NumOptimizedKeys() == 0);
}

TEST_CASE("Test FactorKeys constructor from keys and keys_to_optimizer", "[factor_keys]") {
  sym::FactorKeys factor_keys({{'a'}, {'b'}, {'c', 0}, {'d', 1, 2}}, {{'a'}, {'b'}});
  CHECK(factor_keys.NumKeys() == 4);
  CHECK(factor_keys.NumOptimizedKeys() == 2);
}

TEST_CASE("Test FactorKeys constructor from initializer_list", "[factor_keys]") {
  sym::FactorKeys factor_keys{{'a'} /* default to fixed */,
                              {'b', sym::FactorKeyType::OPTIMIZE},
                              {{'c', 0}, sym::FactorKeyType::FIX},
                              sym::Key{'d', 1, 2}};

  CHECK(factor_keys.NumKeys() == 4);
  CHECK(factor_keys.NumOptimizedKeys() == 1);
}
