// Copyright (C) 2016 Leandro Lisboa Penz <lpenz@lpenz.org>
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

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
 * On both cases, you have to use @ref fatipc_free do free the resources.
 */
struct FatipcBuffer {
    size_t size;  //!< Buffer size
    int fd;       //!< Descriptor of the backing file
    void* data;   //!< Write/read the data here
};

/**
 * Allocate an uninitialized buffer with the specified size
 * @param  size Buffer size
 * @param  buffer The buffer structure to fill up
 * @return Returns 0 on success, or one of the following errors:
 * - -1: error while creating temporary file (mkstemp);
 * - -2: error unlinking file (unlink);
 * - -3: error while setting file size (ftruncate);
 * - -4: error while mapping the file (mmap).
 */
int fatipc_alloc(size_t size, struct FatipcBuffer* buffer);

/**
 * Free the resources used by a buffer
 * @param  buffer Pointer to the buffer to free
 */
void fatipc_free(struct FatipcBuffer* buffer);

#endif /* SRC_FATIPC_BUFF_H_ */
