/**
 * @file
 * The buffer structure and related functions
 */

#ifndef SRC_FATIPC_BUFF_H_
#define SRC_FATIPC_BUFF_H_

#include <stddef.h>

/**
 * Structure of the buffer.
 *
 * The structure is not opaque. After allocating it, you should write the
 * desired data to the @ref data member, or read it from there after receiving.
 *
 * On both cases, you have to use @ref fatipc_buffer_free do free the resources.
 */
struct FatipcBuffer {
    size_t size; //!< Buffer size
    int fd; //!< Descriptor of the backing file
    void* data; //!< Write/read the data here
};

/**
 * Allocate an empty buffer with the specified size
 * @param  size Buffer size
 * @return The buffer or NULL on error
 */
struct FatipcBuffer* fatipc_buffer_alloc(size_t size);

/**
 * Free the resources used by this buffer
 * @param  buffer Pointer to the buffer to free
 */
void fatipc_buffer_free(struct FatipcBuffer** buffer);

#endif /* SRC_FATIPC_BUFF_H_ */
