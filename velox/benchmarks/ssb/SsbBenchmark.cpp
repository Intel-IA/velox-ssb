/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <folly/Benchmark.h>
#include <folly/init/Init.h>
#include <gflags/gflags.h>

#include "velox/common/base/SuccinctPrinter.h"
#include "velox/common/file/FileSystems.h"
#include "velox/connectors/hive/HiveConnector.h"
#include "velox/dwio/common/Options.h"
#include "velox/dwio/dwrf/reader/DwrfReader.h"
#include "velox/dwio/parquet/RegisterParquetReader.h"
#include "velox/exec/PlanNodeStats.h"
#include "velox/exec/Split.h"
#include "velox/exec/tests/utils/HiveConnectorTestBase.h"
#include "velox/exec/tests/utils/SsbQueryBuilder.h"
#include "velox/functions/prestosql/aggregates/RegisterAggregateFunctions.h"
#include "velox/functions/prestosql/registration/RegistrationFunctions.h"
#include "velox/parse/TypeResolver.h"

#include <sys/time.h>

using namespace facebook::velox;
using namespace facebook::velox::exec;
using namespace facebook::velox::exec::test;
using namespace facebook::velox::dwio::common;

namespace {
static bool notEmpty(const char* /*flagName*/, const std::string& value) {
  return !value.empty();
}

static bool validateDataFormat(const char* flagname, const std::string& value) {
  if ((value.compare("parquet") == 0) || (value.compare("dwrf") == 0)) {
    return true;
  }
  std::cout
      << fmt::format(
             "Invalid value for --{}: {}. Allowed values are [\"parquet\", \"dwrf\"]",
             flagname,
             value)
      << std::endl;
  return false;
}

void ensureTaskCompletion(exec::Task* task) {
  // ASSERT_TRUE requires a function with return type void.
  ASSERT_TRUE(waitForTaskCompletion(task));
}

void printResults(const std::vector<RowVectorPtr>& results) {
  std::cout << "Results:" << std::endl;
  bool printType = true;
  for (const auto& vector : results) {
    // Print RowType only once.
    if (printType) {
      std::cout << vector->type()->asRow().toString() << std::endl;
      printType = false;
    }
    for (vector_size_t i = 0; i < vector->size(); ++i) {
      std::cout << vector->toString(i) << std::endl;
    }
  }
}
} // namespace

DEFINE_string(data_path, "", "Root path of TPC-H data");
DEFINE_int32(
    run_query_verbose,
    -1,
    "Run a given query and print execution statistics");
DEFINE_bool(
    include_custom_stats,
    false,
    "Include custom statistics along with execution statistics");
DEFINE_bool(include_results, false, "Include results in the output");
DEFINE_bool(use_native_parquet_reader, true, "Use Native Parquet Reader");
DEFINE_int32(num_drivers, 4, "Number of drivers");
DEFINE_string(data_format, "parquet", "Data format");
DEFINE_int32(num_splits_per_file, 10, "Number of splits per file");

DEFINE_validator(data_path, &notEmpty);
DEFINE_validator(data_format, &validateDataFormat);

class SsbBenchmark {
 public:
  void initialize() {
    functions::prestosql::registerAllScalarFunctions();
    aggregate::prestosql::registerAllAggregateFunctions();
    parse::registerTypeResolver();
    filesystems::registerLocalFileSystem();
    if (FLAGS_use_native_parquet_reader) {
      parquet::registerParquetReaderFactory(parquet::ParquetReaderType::NATIVE);
    } else {
      parquet::registerParquetReaderFactory(parquet::ParquetReaderType::DUCKDB);
    }
    dwrf::registerDwrfReaderFactory();
    auto hiveConnector =
        connector::getConnectorFactory(
            connector::hive::HiveConnectorFactory::kHiveConnectorName)
            ->newConnector(kHiveConnectorId, nullptr);
    connector::registerConnector(hiveConnector);
  }

  std::pair<std::unique_ptr<TaskCursor>, std::vector<RowVectorPtr>> run(
      const SsbPlan& ssbPlan) {
    CursorParameters params;
    params.maxDrivers = FLAGS_num_drivers;
    params.planNode = ssbPlan.plan;
    const int numSplitsPerFile = FLAGS_num_splits_per_file;

    bool noMoreSplits = false;
    auto addSplits = [&](exec::Task* task) {
      if (!noMoreSplits) {
        for (const auto& entry : ssbPlan.dataFiles) {
          for (const auto& path : entry.second) {
            auto const splits = HiveConnectorTestBase::makeHiveConnectorSplits(
                path, numSplitsPerFile, ssbPlan.dataFileFormat);
            for (const auto& split : splits) {
              task->addSplit(entry.first, exec::Split(split));
            }
          }
          task->noMoreSplits(entry.first);
        }
      }
      noMoreSplits = true;
    };
    return readCursor(params, addSplits);
  }
};

SsbBenchmark benchmark;
std::shared_ptr<SsbQueryBuilder> queryBuilder;

BENCHMARK(q1) {
  const auto planContext = queryBuilder->getQueryPlan(1);
  benchmark.run(planContext);
}

BENCHMARK(q2) {
  const auto planContext = queryBuilder->getQueryPlan(2);
  benchmark.run(planContext);
}

BENCHMARK(q3) {
  const auto planContext = queryBuilder->getQueryPlan(3);
  benchmark.run(planContext);
}

BENCHMARK(q4) {
  const auto planContext = queryBuilder->getQueryPlan(4);
  benchmark.run(planContext);
}

BENCHMARK(q5) {
  const auto planContext = queryBuilder->getQueryPlan(5);
  benchmark.run(planContext);
}

BENCHMARK(q6) {
  const auto planContext = queryBuilder->getQueryPlan(6);
  benchmark.run(planContext);
}

BENCHMARK(q7) {
  const auto planContext = queryBuilder->getQueryPlan(7);
  benchmark.run(planContext);
}

BENCHMARK(q8) {
  const auto planContext = queryBuilder->getQueryPlan(8);
  benchmark.run(planContext);
}

BENCHMARK(q9) {
  const auto planContext = queryBuilder->getQueryPlan(9);
  benchmark.run(planContext);
}

BENCHMARK(q10) {
  const auto planContext = queryBuilder->getQueryPlan(10);
  benchmark.run(planContext);
}

BENCHMARK(q11) {
  const auto planContext = queryBuilder->getQueryPlan(11);
  benchmark.run(planContext);
}

BENCHMARK(q12) {
  const auto planContext = queryBuilder->getQueryPlan(12);
  benchmark.run(planContext);
}

BENCHMARK(q13) {
  const auto planContext = queryBuilder->getQueryPlan(13);
  benchmark.run(planContext);
}

int main(int argc, char** argv) {
  std::string kUsage(
      "This program benchmarks SSB queries. Run 'velox_ssb_benchmark -helpon=SsbBenchmark' for available options.\n");
  gflags::SetUsageMessage(kUsage);
  folly::init(&argc, &argv, false);
  benchmark.initialize();
  queryBuilder =
      std::make_shared<SsbQueryBuilder>(toFileFormat(FLAGS_data_format));
  queryBuilder->initialize(FLAGS_data_path);
  if (FLAGS_run_query_verbose == -1) {
    folly::runBenchmarks();
  } else {
    const auto queryPlan = queryBuilder->getQueryPlan(FLAGS_run_query_verbose);
    const auto [cursor, actualResults] = benchmark.run(queryPlan);
    if (!cursor) {
      LOG(ERROR) << "Query terminated with error. Exiting";
      exit(1);
    }
    auto task = cursor->task();
    ensureTaskCompletion(task.get());
    if (FLAGS_include_results) {
      printResults(actualResults);
      std::cout << std::endl;
    }
    const auto stats = task->taskStats();
    std::cout << fmt::format(
                     "Execution time: {}",
                     succinctMillis(
                         stats.executionEndTimeMs - stats.executionStartTimeMs))
              << std::endl;
    std::cout << fmt::format(
                     "Splits total: {}, finished: {}",
                     stats.numTotalSplits,
                     stats.numFinishedSplits)
              << std::endl;
    std::cout << printPlanWithStats(
                     *queryPlan.plan, stats, FLAGS_include_custom_stats)
              << std::endl;
  }
}
