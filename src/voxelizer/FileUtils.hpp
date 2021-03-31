#pragma once

#include <Common.h>

#include <cstdio>

namespace utils
{

enum class FilePermissions {
  Read,
  Write,
  ReadWrite,
  BinaryRead,
  BinaryWrite,
  BinaryReadWrite,
};

inline FILE *OpenFile(const char *file, FilePermissions permissions)
{
  const char *cPermissions[] = {"r", "w", "w+", "rb", "wb", "wb+"};
  FILE *ret = NULL;
  ret = fopen(file, cPermissions[(u32)permissions]);
  if (!ret) {
    // TODO: better error handling
    printf("FAILED TO OPEN FILE: %s\n", file);
    exit(EXIT_FAILURE);
  }
  return ret;
}

inline std::string ReadEntireFileAsString(const char *file)
{
  auto *fp = OpenFile(file, FilePermissions::Read);
  fseek(fp, 0, SEEK_END);
  u64 length = ftell(fp);
  rewind(fp);
  if (length == 0) {
    printf("Failed to read file size\n");
  }
  std::string data(length, 0);
  fread(data.data(), sizeof(u8), length, fp);
  fclose(fp);
  return data;
}

inline std::vector<u8> ReadEntireFileAsVector(const char *file)
{
  auto *fp = OpenFile(file, FilePermissions::BinaryRead);
  fseek(fp, 0, SEEK_END);
  u64 length = ftell(fp);
  rewind(fp);
  if (length == 0) {
    printf("Failed to read file size\n");
  }
  std::vector<u8> data(length, 0);
  fread(data.data(), sizeof(u8), data.size(), fp);
  fclose(fp);
  return data;
}

} // namespace utils