// Copyright (C) 2016 Leandro Lisboa Penz <lpenz@lpenz.org>
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

/**
 * @file
 * send and recv functions that work with sockets
 *
 * Use these functions when the socket is managed elsewhere.
 */

#ifndef _SRC_FATIPC_SOCKET_H_
#define _SRC_FATIPC_SOCKET_H_

#include <stdbool.h>

#include "fatipc_buffer.h"

/**
 * Receive a @ref FatipcBuffer from a socket
 * @param  socket Socket to read from
 * @param  buffer Buffer to use - must not have been allocated yet
 * @return Returns 0 on success, or one of the following errors:
 * - -1: error receiving fd (recvmsg);
 * - -3: error while getting file size (fstat);
 * - -4: error while mapping the file (mmap).
 */
int fatipc_recv(int socket, struct FatipcBuffer* buffer);

/**
 * Send a @ref FatipcBuffer through a socket
 * @param  socket Socket to send to
 * @param  buffer @ref FatipcBuffer to send
 * @return Returns 0 on success, or -1 on error (sendmsg).
 */
int fatipc_send(int socket, struct FatipcBuffer* buffer);

#endif /* _SRC_FATIPC_SOCKET_H_ */
