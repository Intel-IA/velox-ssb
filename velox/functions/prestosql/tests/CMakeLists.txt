# Copyright (c) Facebook, Inc. and its affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

add_subdirectory(utils)

add_executable(
  velox_functions_test
  ArithmeticTest.cpp
  ArrayAllMatchTest.cpp
  ArrayAnyMatchTest.cpp
  ArrayAverageTest.cpp
  ArrayCombinationsTest.cpp
  ArrayConstructorTest.cpp
  ArrayContainsTest.cpp
  ArrayDistinctTest.cpp
  ArrayDuplicatesTest.cpp
  ArrayExceptTest.cpp
  ArrayFilterTest.cpp
  ArrayJoinTest.cpp
  ArrayHasDuplicatesTest.cpp
  ArrayIntersectTest.cpp
  ArrayMaxTest.cpp
  ArrayMinTest.cpp
  ArrayNoneMatchTest.cpp
  ArrayNormalizeTest.cpp
  ArrayPositionTest.cpp
  ArraySortTest.cpp
  ArrayShuffleTest.cpp
  ArraysOverlapTest.cpp
  ArraySumTest.cpp
  BitwiseTest.cpp
  CardinalityTest.cpp
  CeilFloorTest.cpp
  ComparisonsTest.cpp
  DateTimeFunctionsTest.cpp
  DecimalArithmeticTest.cpp
  ElementAtTest.cpp
  HyperLogLogCastTest.cpp
  HyperLogLogFunctionsTest.cpp
  InPredicateTest.cpp
  JsonCastTest.cpp
  JsonExtractScalarTest.cpp
  JsonFunctionsTest.cpp
  MapEntriesTest.cpp
  MapFilterTest.cpp
  MapKeysAndValuesTest.cpp
  MapTest.cpp
  MapZipWithTest.cpp
  NotTest.cpp
  ReduceTest.cpp
  RegexpReplaceTest.cpp
  RepeatTest.cpp
  ReverseTest.cpp
  RoundTest.cpp
  ScalarFunctionRegTest.cpp
  SliceTest.cpp
  SplitTest.cpp
  StringFunctionsTest.cpp
  TransformTest.cpp
  TransformKeysTest.cpp
  TransformValuesTest.cpp
  URLFunctionsTest.cpp
  WidthBucketArrayTest.cpp
  GreatestLeastTest.cpp
  ZipTest.cpp
  ZipWithTest.cpp)

add_test(velox_functions_test velox_functions_test)

target_link_libraries(
  velox_functions_test
  velox_functions_test_lib
  velox_exec
  velox_expression
  velox_functions_prestosql
  velox_functions_lib
  velox_exec_test_lib
  velox_dwio_common_test_utils
  velox_vector_fuzzer
  gtest
  gtest_main
  gflags::gflags
  gmock
  gmock_main)
