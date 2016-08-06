/**
 * @file
 * Tests for libfatipc
 */

#include <sys/types.h>
#include <sys/socket.h>

#include <gtest/gtest.h>

extern "C" {
#include "fatipc_buffer.h"
#include "fatipc_socket.h"
}

/**
 * Test buffer allocation and deallocation
 */
TEST(fatipc_basic, buffer_alloc)
{
    const int size = 500;
    struct FatipcBuffer b;
    ASSERT_EQ(fatipc_alloc(size, &b), 0);
    memset(b.data, size, 0x55);
    memset(b.data, size, 0xaa);
    fatipc_free(&b);
}

/**
 * Test basic sending and receiving of buffers
 */
TEST(fatipc_basic, socketpair)
{
    const int size = 500;
    int sockets[2];

    /* create the connected sockets */
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_DGRAM, 0, sockets), 0);

    /* allocate buffer, send, free it */
    struct FatipcBuffer bsend;
    ASSERT_EQ(fatipc_alloc(size, &bsend), 0);
    memset(bsend.data, 0x55, size);
    ASSERT_EQ(fatipc_send(sockets[0], &bsend), 0);
    fatipc_free(&bsend);
    close(sockets[0]);

    /* receive it */
    struct FatipcBuffer brecv;
    ASSERT_EQ(fatipc_recv(sockets[1], &brecv), 0);
    close(sockets[1]);

    /* check first and last bytes */
    char* ptr = (char*)brecv.data;
    ASSERT_EQ(*ptr, 0x55);
    ASSERT_EQ(*(ptr + size - 1), 0x55);

    /* free it at last */
    fatipc_free(&brecv);
}
