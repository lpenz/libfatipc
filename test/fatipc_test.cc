/**
 * @file
 * Tests for libfatipc
 */

#include <gtest/gtest.h>

extern "C" {
#include "fatipc_buffer.h"
}

TEST(fatipc_basic, buffer_alloc)
{
    const int size = 500;
    struct FatipcBuffer b;
    ASSERT_EQ(fatipc_alloc(size, &b), 0);
    memset(b.data, size, 0x55);
    memset(b.data, size, 0xaa);
    fatipc_free(&b);
}
