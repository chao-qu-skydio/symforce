#pragma once

#include <initializer_list>
#include <vector>

#include "./key.h"

namespace sym {

enum class FactorKeyType { OPTIMIZE, FIX };

struct FactorKey {
  Key key;
  FactorKeyType type;

  FactorKey() = default;
  // Default type to OPTIMIZE, this is to be compatible with previous API
  // If you don't specify the type, the key is optimizable.
  FactorKey(Key key, FactorKeyType type = FactorKeyType::OPTIMIZE) : key(key), type(type) {}
};

class FactorKeys {
 private:
  std::vector<Key> keys_;
  std::vector<Key> optimized_keys_;

 public:
  // Constructor
  FactorKeys() = default;
  // This exists only for compatibility reason, want to kill it later on
  FactorKeys(const std::vector<Key>& keys, const std::vector<Key>& keys_to_opt)
      : keys_(keys), optimized_keys_(keys_to_opt.empty() ? keys_ : keys_to_opt) {}
  // This should be the one we use
  FactorKeys(std::initializer_list<FactorKey> factor_keys);

  size_t NumKeys() const noexcept {
    return keys_.size();
  }

  size_t NumOptimizedKeys() const noexcept {
    return optimized_keys_.size();
  }

  const std::vector<Key>& AllKeys() const noexcept {
    return keys_;
  }

  // This exists only for compatibility reason, will be replaced by some kind of iterators
  const std::vector<Key>& OptimizedKeys() const noexcept {
    return optimized_keys_;
  }
};

}  // namespace sym
