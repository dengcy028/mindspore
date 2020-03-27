/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <fcntl.h>
#include <iostream>
#include <memory>
#include "common/common_test.h"
#include "./common.h"
#include "utils/system/file_system.h"
#include "utils/system/env.h"
#define private public
#include "debug/e2e_dump.h"
#undef private

namespace mindspore {
class TestMemoryDumper : public UT::Common {
 public:
  TestMemoryDumper() {}
};

TEST_F(TestMemoryDumper, test_DumpToFileAbsPath) {
  int len = 1000;
  int data[1000] = {0};
  for (uint32_t i = 0; i < len; i++) {
    data[i] = i % 10;
  }

  int ret;
  char filename[] = "/tmp/dumpToFileTestFile";
  ret = mindspore::Dump::DumpToFile(filename, data, len * sizeof(int));
  ASSERT_EQ(ret, true);

  int fd = open(filename, O_RDONLY);
  int readBack[1000] = {0};
  int readSize = read(fd, readBack, len * sizeof(int));
  (void)close(fd);
  ASSERT_EQ(readSize, len * sizeof(int));

  ret = true;
  for (uint32_t i = 0; i < len; i++) {
    if (data[i] != readBack[i]) {
      ret = false;
      break;
    }
  }
  std::shared_ptr<system::FileSystem> fs = system::Env::GetFileSystem();
  if (fs->FileExist(filename)) {
    fs->DeleteFile(filename);
  }
  ASSERT_EQ(ret, true);
}

TEST_F(TestMemoryDumper, test_DumpToFileRelativePath) {
  int len = 1000;
  int data[1000] = {0};
  for (uint32_t i = 0; i < len; i++) {
    data[i] = i % 10;
  }

  int ret;
  char filename[] = "../../dumpToFileTestFile";
  ret = mindspore::Dump::DumpToFile(filename, data, len * sizeof(int));
  ASSERT_EQ(ret, true);

  int fd = open(filename, O_RDONLY);
  int readBack[1000] = {0};
  int readSize = read(fd, readBack, len * sizeof(int));
  (void)close(fd);
  ASSERT_EQ(readSize, len * sizeof(int));

  ret = true;
  for (uint32_t i = 0; i < len; i++) {
    if (data[i] != readBack[i]) {
      ret = false;
      break;
    }
  }
  std::shared_ptr<system::FileSystem> fs = system::Env::GetFileSystem();
  if (fs->FileExist(filename)) {
    fs->DeleteFile(filename);
  }

  ASSERT_EQ(ret, true);
}

TEST_F(TestMemoryDumper, test_DumpToFileNotExistDir) {
  int len = 1;
  int data[1] = {0};
  for (uint32_t i = 0; i < len; i++) {
    data[i] = i % 10;
  }

  char filename[] = "./tmp/dumpToFileTestFile";
  int ret = mindspore::Dump::DumpToFile(filename, data, len * sizeof(int));
  ASSERT_EQ(ret, true);

  int fd = open(filename, O_RDONLY);
  int readBack[1000] = {0};
  int readSize = read(fd, readBack, len * sizeof(int));
  (void)close(fd);
  ASSERT_EQ(readSize, len * sizeof(int));

  ret = true;
  for (uint32_t i = 0; i < len; i++) {
    if (data[i] != readBack[i]) {
      ret = false;
      break;
    }
  }
  std::shared_ptr<system::FileSystem> fs = system::Env::GetFileSystem();
  if (fs->FileExist(filename)) {
    fs->DeleteFile(filename);
  }

  ASSERT_EQ(ret, true);
}
}  // namespace mindspore