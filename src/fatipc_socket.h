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
 * @return @ref FatipcBuffer pointer or NULL on error
 */
struct FatipcBuffer* fatipc_recv(int socket);

/**
 * \brief  Send a @ref FatipcBuffer through a socket
 * \param  socket Socket to send to
 * \param  buffer @ref FatipcBuffer to send
 * \return true if success, false if error
 */
bool fatipc_send(int socket, struct FatipcBuffer* buffer);

#endif /* _SRC_FATIPC_SOCKET_H_ */
