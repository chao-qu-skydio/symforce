#include <benchmark/benchmark.h>

#include "./values.h"

namespace sym {
namespace {

namespace bm = benchmark;

void BM_ValuesSet(bm::State& state) {
  Valuesd values;
  Eigen::Vector3d x{0, 0, 0};

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      values.Set<Eigen::Vector3d>(Key('a', i), x);
    }
    bm::DoNotOptimize(values);
  }
}
BENCHMARK(BM_ValuesSet)->Arg(10)->Arg(100)->Arg(1000);

void BM_ValuesAt(bm::State& state) {
  Valuesd values;
  Eigen::Vector3d x{0, 0, 0};
  const auto n = state.range(0);

  for (int i = 0; i < n; ++i) {
    values.Set<Eigen::Vector3d>(Key('a', i), x);
  }

  for (auto _ : state) {
    for (int i = 0; i < n; ++i) {
      x += values.At<Eigen::Vector3d>(Key('a', i));
    }
    bm::DoNotOptimize(x);
  }
}
BENCHMARK(BM_ValuesAt)->Arg(10)->Arg(100)->Arg(1000);

}  // namespace
}  // namespace sym
