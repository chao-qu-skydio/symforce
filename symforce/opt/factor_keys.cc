#include "./factor_keys.h"

#include "symforce/opt/assert.h"

namespace sym {

FactorKeys::FactorKeys(std::initializer_list<FactorKey> factor_keys) {
  keys_.reserve(factor_keys.size());

  for (const FactorKey& factor_key : factor_keys) {
    keys_.push_back(factor_key.key);
    if (factor_key.type == FactorKeyType::OPTIMIZE) {
      optimized_keys_.push_back(factor_key.key);
    }
  }
}

}  // namespace sym
