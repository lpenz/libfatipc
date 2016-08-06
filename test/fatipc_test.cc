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
    struct FatipcBuffer* b = fatipc_buffer_alloc(size);
    ASSERT_NE(b, (void*)NULL);
    memset(b->data, size, 0x55);
    memset(b->data, size, 0xaa);
    fatipc_buffer_free(&b);
    ASSERT_EQ(b, (void*)NULL);
}
