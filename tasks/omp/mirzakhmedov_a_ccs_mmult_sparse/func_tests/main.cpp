// Copyright 2024 Mirzakhmedov Alexander
#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "omp/mirzakhmedov_a_ccs_mmult_sparse/include/ccs_mmult_sparse_omp.hpp"

TEST(Mirzakhmedov_a_ccs_mmult_sparse, test_I) {
  MatrixInCCS_Sparse M1(1, 1, 1);
  MatrixInCCS_Sparse M2(1, 1, 1);
  MatrixInCCS_Sparse M3;

  M1.col_p = {0, 1};
  M1.row = {0};
  M1.val = {std::complex<double>(0.0, 1.0)};

  M2.col_p = {0, 1};
  M2.row = {0};
  M2.val = {std::complex<double>(0.0, -1.0)};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M1));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M2));
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t*>(&M3));

  // Create Task
  Omp_MatMultCCS testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  std::complex<double> correct_reply(1.0, 0.0);
  ASSERT_NEAR(std::abs(M3.val[0] - correct_reply), 0.0, 1e-6);
}

TEST(Mirzakhmedov_a_ccs_mmult_sparse, test_II) {
  std::complex<double> vvector(1.0, 1.0);
  MatrixInCCS_Sparse M1(50, 1, 50);
  MatrixInCCS_Sparse M2(1, 50, 50);
  MatrixInCCS_Sparse M3(50, 50, 2500);

  M1.col_p = {0, 50};

  for (int i = 0; i <= 50; i++) {
    M2.col_p.push_back(i);
  }
  for (int i = 0; i < 50; i++) {
    M1.row.push_back((double)i);
    M1.val.emplace_back(vvector);
    M2.row.push_back(0.0);
    M2.val.emplace_back(vvector);
  };

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M1));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M2));
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t*>(&M3));

  // Create Task
  Omp_MatMultCCS testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  std::complex<double> correct_reply(0.0, 0.0);
  for (int i = 0; i < 50 * 50; i++) {
    ASSERT_EQ(M3.val[i], correct_reply);
  }
}

TEST(Mirzakhmedov_a_ccs_mmult_sparse, test_III) {
  std::complex<double> vvector(2.0, 1.0);
  MatrixInCCS_Sparse M1(50, 50);
  MatrixInCCS_Sparse M2(50, 1);
  MatrixInCCS_Sparse M3(50, 1);

  int l = 1;
  int m = 0;

  for (int i = 0; i <= 50; i++) {
    M1.col_p.push_back(m);
    m += l;
    l++;
  }

  l = 1;
  m = 0;
  for (int i = 0; i < M1.col_p[50]; i++) {
    M1.val.emplace_back(vvector);
    if (m >= l) {
      m = 0;
      l++;
    }
    M1.row.push_back(m);
    m++;
  }

  M2.col_p = {0, 50};
  for (int i = 0; i < 50; i++) {
    M2.val.emplace_back(vvector);
    M2.row.push_back(i);
  }

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M1));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M2));
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t*>(&M3));

  // Create Task
  Omp_MatMultCCS testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  l = 50;
  for (size_t i = 0; i < M3.val.size(); i++) {
    ASSERT_EQ(M3.val[i], std::complex<double>(3.0 * l, 4.0 * l));
    l--;
  }
}

TEST(Mirzakhmedov_a_ccs_mmult_sparse, test_IV) {
  MatrixInCCS_Sparse M1(1, 1, 1);
  MatrixInCCS_Sparse M2(1, 1, 1);
  MatrixInCCS_Sparse M3;

  M1.col_p = {0, 1};
  M1.row = {0};
  M1.val = {std::complex<double>(0.0, 1.0)};

  M2.col_p = {0, 1};
  M2.row = {0};
  M2.val = {std::complex<double>(0.0, -1.0)};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M1));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M2));
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t*>(&M3));

  // Create Task
  Omp_MatMultCCS testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  std::complex<double> correct_reply(1.0, 0.0);
  ASSERT_NEAR(std::abs(M3.val[0] - correct_reply), 0.0, 1e-6);
}

TEST(Mirzakhmedov_a_ccs_mmult_sparse, test_V) {
  std::complex<double> vvector(2.0, 1.0);
  MatrixInCCS_Sparse M1(50, 50);
  MatrixInCCS_Sparse M2(50, 50);
  MatrixInCCS_Sparse M3(50, 50);

  for (int i = 0; i <= 50; i++) {
    M1.col_p.push_back(i);
    M2.col_p.push_back(i);
  }

  for (int i = 0; i < 50; i++) {
    M1.row.push_back(i);
    M1.val.emplace_back(vvector);
    M2.row.push_back(i);
    M2.val.emplace_back(vvector);
  };

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M1));
  taskDataOmp->inputs.emplace_back(reinterpret_cast<uint8_t*>(&M2));
  taskDataOmp->outputs.emplace_back(reinterpret_cast<uint8_t*>(&M3));

  // Create Task
  Omp_MatMultCCS testTaskOmp(taskDataOmp);
  ASSERT_EQ(testTaskOmp.validation(), true);
  testTaskOmp.pre_processing();
  testTaskOmp.run();
  testTaskOmp.post_processing();

  std::complex<double> correct_reply(3.0, 4.0);
  for (size_t i = 0; i < M3.val.size(); i++) {
    ASSERT_EQ(M3.val[i], correct_reply);
  }
}