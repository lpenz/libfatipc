/**
 * @file
 * The buffer structure and related functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "fatipc_buffer.h"

struct FatipcBuffer* fatipc_buffer_alloc(size_t size)
{
    /* create tmp file for mmap */
    char template[] = "/tmp/sinkXXXXXX";
    int fd = mkstemp(template);
    if (fd < 0) {
        return NULL;
    }

    /* remove reference from file system */
    unlink(template);

    /* set file size */
    if (ftruncate(fd, size) < 0) {
        close(fd);
        return NULL;
    }

    /* mmap */
    void* addr = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    /* success, return buffer */
    struct FatipcBuffer* buffer = malloc(sizeof(struct FatipcBuffer));
    buffer->data = addr;
    buffer->size = size;
    buffer->fd = fd;

    return buffer;
}

void fatipc_buffer_free(struct FatipcBuffer** buffer)
{
    munmap((*buffer)->data, (*buffer)->size);
    close((*buffer)->fd);
    free(*buffer);
    *buffer = NULL;
}
