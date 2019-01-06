#pragma once

#include <byteswap.h>

namespace Net::Endian
{
    template <typename T>
    T Swap(T val)
    {
        return val;
    }

#define LITTLE_ENDIAN_HOST 1
#define BIG_ENDIAN_HOST 0 // TODO - I think NWN uses little endian network order?

#if BIG_ENDIAN_HOST

    template <>
    std::int16_t Swap(std::int16_t val)
    {
        return bswap_16(val);
    }

    template <>
    std::uint16_t Swap(std::uint16_t val)
    {
        return bswap_16(val);
    }

    template <>
    std::int32_t Swap(std::int32_t val)
    {
        return bswap_32(val);
    }

    template <>
    std::uint32_t Swap(std::uint32_t val)
    {
        return bswap_32(val);
    }

    template <>
    std::int64_t Swap(std::int64_t val)
    {
        return bswap_64(val);
    }

    template <>
    std::uint64_t Swap(std::uint64_t val)
    {
        return bswap_64(val);
    }
#endif
}
