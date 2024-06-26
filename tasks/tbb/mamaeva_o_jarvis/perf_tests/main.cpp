// Copyright 2024 Mamaeva Olga
#include <gtest/gtest.h>
#include <oneapi/tbb.h>

#include <algorithm>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "tbb/mamaeva_o_jarvis/include/ops_tbb.hpp"

using namespace mamaeva_tbb;

TEST(mamaeva_o_jarvis_tbb, test_pipeline_run) {
  int n = 20000;
  int h = 1200;
  std::vector<r> points(n);
  std::vector<r> hull(h);
  std::vector<r> out(hull.size());

  mamaeva_tbb::prepare_points(points.data(), points.size(), hull.data(), hull.size(), 250.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskData->inputs_count.emplace_back(points.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTask = std::make_shared<JarvisHullTbb>(taskData);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTask);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  std::sort(out.begin(), out.end());
  std::sort(hull.begin(), hull.end());
  for (size_t i = 0; i < hull.size(); i++) {
    ASSERT_NEAR(hull[i].x, out[i].x, 1e-3);
    ASSERT_NEAR(hull[i].y, out[i].y, 1e-3);
  }
}

TEST(mamaeva_o_jarvis_tbb, test_task_run) {
  int n = 20000;
  int h = 1200;
  std::vector<r> points(n);
  std::vector<r> hull(h);
  std::vector<r> out(hull.size());

  mamaeva_tbb::prepare_points(points.data(), points.size(), hull.data(), hull.size(), 250.0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t *>(points.data()));
  taskData->inputs_count.emplace_back(points.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTask = std::make_shared<JarvisHullTbb>(taskData);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = oneapi::tbb::tick_count::now();
  perfAttr->current_timer = [&] { return (oneapi::tbb::tick_count::now() - t0).seconds(); };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTask);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  std::sort(out.begin(), out.end());
  std::sort(hull.begin(), hull.end());
  for (size_t i = 0; i < hull.size(); i++) {
    ASSERT_NEAR(hull[i].x, out[i].x, 1e-3);
    ASSERT_NEAR(hull[i].y, out[i].y, 1e-3);
  }
}
