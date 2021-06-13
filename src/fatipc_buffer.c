// Copyright (C) 2016 Leandro Lisboa Penz <lpenz@lpenz.org>
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

/**
 * @file
 * The buffer structure and related functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "fatipc_buffer.h"

int fatipc_alloc(size_t size, struct FatipcBuffer* buffer)
{
    /* create tmp file for mmap */
    char template[] = "/tmp/sinkXXXXXX";
    int fd = mkstemp(template);
    if (fd < 0) {
        return -1;
    }

    /* remove reference from file system */
    if (unlink(template)) {
        /* something is wrong, we might have a security breach;
         * return error instead of allowing the use of the buffer. */
        close(fd);
        return -2;
    }

    /* set file size */
    if (ftruncate(fd, size) < 0) {
        close(fd);
        return -3;
    }

    /* mmap */
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        close(fd);
        return -4;
    }

    /* success, fill up buffer */
    buffer->data = addr;
    buffer->size = size;
    buffer->fd = fd;

    return 0;
}

void fatipc_free(struct FatipcBuffer* buffer)
{
    munmap(buffer->data, buffer->size);
    close(buffer->fd);
    memset(buffer, 0, sizeof(*buffer));
}
