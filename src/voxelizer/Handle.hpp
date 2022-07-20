#pragma once
#include "Common.h"

#include <cassert>
template<typename T>
class Handle
{
    u32 m_handle = 0;

public:
    Handle() = default;
    explicit Handle(u32 handle) : m_handle(handle) {  }
    Handle(const Handle &o) : m_handle(o.m_handle) {  }
    Handle(Handle &&o) noexcept : m_handle(o.m_handle) {  }
    Handle &operator=(const Handle &o)
    {
        m_handle = o.m_handle;
        return *this;
    }

    bool operator==(const Handle &o) const { return m_handle == o.m_handle; }
    explicit operator bool() const { return IsValid(); }

    static Handle Invalid() { return Handle(); }

    [[nodiscard]] u32 Data() const { return m_handle; }
    [[nodiscard]] bool IsValid() const { return m_handle != 0; }
};

namespace std
{

template<typename T>
struct hash<Handle<T>> {
    size_t operator()(const Handle<T> &handle) const { return hash<u32>()(handle.Data()); }
};

} // namespace std
