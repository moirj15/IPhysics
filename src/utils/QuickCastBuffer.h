#pragma once

#include "../common.h"

#include <assert.h>
#include <vector>

template<typename ContentType, typename CastToType>
class QuickCastBuffer
{
  std::vector<ContentType> mContents;
  static constexpr u64 CAST_SIZE = sizeof(CastToType) / sizeof(ContentType);

public:
  QuickCastBuffer() = default;
  explicit QuickCastBuffer(u64 size) : mContents(size)
  {
  }
  QuickCastBuffer(u64 size, ContentType *data) : mContents(data, data + size)
  {
  }
  QuickCastBuffer(const std::vector<ContentType> &data) : mContents(data)
  {
  }
  QuickCastBuffer(std::vector<ContentType> &&data) : mContents(data)
  {
  }

  NODISCARD inline const std::vector<ContentType> &GetBuffer() const
  {
    return mContents;
  }

  NODISCARD inline bool IsEmpty() const
  {
    return mContents.empty();
  }

  NODISCARD inline u64 SizeInBytes() const
  {
    return mContents.size() * sizeof(ContentType);
  }

  NODISCARD inline u64 BufferSize() const
  {
    return mContents.size();
  }

  NODISCARD inline u64 CastBufferSize() const
  {
    return (mContents.size() * sizeof(ContentType)) / sizeof(CastToType);
  }

  inline void SetBufferSize(u64 size)
  {
    mContents.resize(size);
  }

  NODISCARD inline const ContentType *BufferPtr() const
  {
    return mContents.data();
  }

  NODISCARD inline u64 BufferCastToTypeSize() const
  {
    return mContents.size() / sizeof(CastToType);
  }

  NODISCARD inline ContentType AccessBuffer(u64 i) const
  {
    return mContents[i];
  }
  NODISCARD inline ContentType &AccessBuffer(u64 i)
  {
    return mContents[i];
  }

  NODISCARD inline const CastToType &AccessCastBuffer(u64 i) const
  {
    assert(((i * CAST_SIZE) < mContents.size()) && "Out of Bounds Cast Buffer Access");
    return *((CastToType *)&mContents[i * CAST_SIZE]);
  }

  NODISCARD inline CastToType &AccessCastBuffer(u64 i)
  {
    assert(((i * CAST_SIZE) < mContents.size()) && "Out of Bounds Cast Buffer Access");
    return *((CastToType *)&mContents[i * CAST_SIZE]);
  }

  inline void BufferPushBack(const ContentType &c)
  {
    mContents.push_back(c);
  }

  inline void CastBufferPushBack(const CastToType &c)
  {
    for (u64 i = 0; i < (sizeof(CastToType) / sizeof(ContentType)); i++)
    {
      mContents.push_back(c[i]);
    }
  }

  inline void CastBufferPushBack(const std::vector<CastToType> &c)
  {
    for (const auto &value : c)
    {
      for (u64 i = 0; i < (sizeof(CastToType) / sizeof(ContentType)); i++)
      {
        mContents.push_back(value[i]);
      }
    }
  }

  inline void Clear()
  {
    mContents.clear();
  }
};