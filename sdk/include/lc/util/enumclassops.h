//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <type_traits>

//! Implements comparison (except for equality) with the enum's underlying type
#define IMPL_ENUM_CLASS_OP_CMP(TYPE)                                       \
    inline bool operator<=(TYPE lhs, std::underlying_type<TYPE>::type rhs) \
    {                                                                      \
        return static_cast<std::underlying_type<TYPE>::type>(lhs) <= rhs;  \
    }                                                                      \
    inline bool operator<(TYPE lhs, std::underlying_type<TYPE>::type rhs)  \
    {                                                                      \
        return static_cast<std::underlying_type<TYPE>::type>(lhs) < rhs;   \
    }                                                                      \
    inline bool operator>=(TYPE lhs, std::underlying_type<TYPE>::type rhs) \
    {                                                                      \
        return static_cast<std::underlying_type<TYPE>::type>(lhs) >= rhs;  \
    }                                                                      \
    inline bool operator>(TYPE lhs, std::underlying_type<TYPE>::type rhs)  \
    {                                                                      \
        return static_cast<std::underlying_type<TYPE>::type>(lhs) > rhs;   \
    }                                                                      \
    inline bool operator==(TYPE lhs, std::underlying_type<TYPE>::type rhs) \
    {                                                                      \
        return static_cast<std::underlying_type<TYPE>::type>(lhs) == rhs;  \
    }                                                                      \
    inline bool operator!=(TYPE lhs, std::underlying_type<TYPE>::type rhs) \
    {                                                                      \
        return static_cast<std::underlying_type<TYPE>::type>(lhs) != rhs;  \
    }                                                                      \
    inline bool operator<=(std::underlying_type<TYPE>::type lhs, TYPE rhs) \
    {                                                                      \
        return lhs <= static_cast<std::underlying_type<TYPE>::type>(rhs);  \
    }                                                                      \
    inline bool operator<(std::underlying_type<TYPE>::type lhs, TYPE rhs)  \
    {                                                                      \
        return lhs < static_cast<std::underlying_type<TYPE>::type>(rhs);   \
    }                                                                      \
    inline bool operator>=(std::underlying_type<TYPE>::type lhs, TYPE rhs) \
    {                                                                      \
        return lhs >= static_cast<std::underlying_type<TYPE>::type>(rhs);  \
    }                                                                      \
    inline bool operator>(std::underlying_type<TYPE>::type lhs, TYPE rhs)  \
    {                                                                      \
        return lhs > static_cast<std::underlying_type<TYPE>::type>(rhs);   \
    }                                                                      \
    inline bool operator==(std::underlying_type<TYPE>::type lhs, TYPE rhs) \
    {                                                                      \
        return lhs == static_cast<std::underlying_type<TYPE>::type>(rhs);  \
    }                                                                      \
    inline bool operator!=(std::underlying_type<TYPE>::type lhs, TYPE rhs) \
    {                                                                      \
        return lhs != static_cast<std::underlying_type<TYPE>::type>(rhs);  \
    }

#define IMPL_ENUM_CLASS_OP_BITWISE_OR(TYPE)                                           \
    inline TYPE operator|(TYPE lhs, TYPE rhs)                                         \
    {                                                                                 \
        return static_cast<TYPE>(static_cast<std::underlying_type<TYPE>::type>(lhs) | \
                                 static_cast<std::underlying_type<TYPE>::type>(rhs)); \
    }                                                                                 \
    inline TYPE& operator|=(TYPE& lhs, TYPE rhs)                                      \
    {                                                                                 \
        return (lhs = lhs | rhs);                                                     \
    }

#define IMPL_ENUM_CLASS_OP_BITWISE_AND(TYPE)                                          \
    inline TYPE operator&(TYPE lhs, TYPE rhs)                                         \
    {                                                                                 \
        return static_cast<TYPE>(static_cast<std::underlying_type<TYPE>::type>(lhs) & \
                                 static_cast<std::underlying_type<TYPE>::type>(rhs)); \
    }                                                                                 \
    inline TYPE& operator&=(TYPE& lhs, TYPE rhs)                                      \
    {                                                                                 \
        return (lhs = lhs & rhs);                                                     \
    }                                                                                 \
    inline bool operator!(TYPE value)                                                 \
    {                                                                                 \
        return value == static_cast<TYPE>(0);                                         \
    }

#define IMPL_ENUM_CLASS_OP_BITWISE_AND_2(TYPE, MASK_TYPE)                                  \
    inline TYPE operator&(TYPE lhs, MASK_TYPE rhs)                                         \
    {                                                                                      \
        return static_cast<TYPE>(static_cast<std::underlying_type<TYPE>::type>(lhs) &      \
                                 static_cast<std::underlying_type<MASK_TYPE>::type>(rhs)); \
    }                                                                                      \
    inline TYPE& operator&=(TYPE& lhs, MASK_TYPE rhs)                                      \
    {                                                                                      \
        return (lhs = lhs & rhs);                                                          \
    }

#define IMPL_ENUM_CLASS_OP_BITWISE_XOR(TYPE)                                          \
    inline TYPE operator^(TYPE lhs, TYPE rhs)                                         \
    {                                                                                 \
        return static_cast<TYPE>(static_cast<std::underlying_type<TYPE>::type>(lhs) ^ \
                                 static_cast<std::underlying_type<TYPE>::type>(rhs)); \
    }                                                                                 \
    inline TYPE& operator^=(TYPE& lhs, TYPE rhs)                                      \
    {                                                                                 \
        return (lhs = lhs ^ rhs);                                                     \
    }

#define IMPL_ENUM_CLASS_OP_BITWISE_NOT(TYPE)                                             \
    inline TYPE operator~(TYPE value)                                                    \
    {                                                                                    \
        return static_cast<TYPE>(~static_cast<std::underlying_type<TYPE>::type>(value)); \
    }

#define IMPL_ENUM_CLASS_OP_INC_DEC(TYPE)                                                        \
    inline TYPE operator++(TYPE& value)                                                         \
    {                                                                                           \
        return value =                                                                          \
                   static_cast<TYPE>(static_cast<std::underlying_type<TYPE>::type>(value) + 1); \
    }                                                                                           \
    inline TYPE operator--(TYPE& value)                                                         \
    {                                                                                           \
        return value =                                                                          \
                   static_cast<TYPE>(static_cast<std::underlying_type<TYPE>::type>(value) - 1); \
    }
