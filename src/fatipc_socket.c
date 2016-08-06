/**
 * @file
 * send and recv functions that work with sockets
 *
 * Use these functions when the socket is managed elsewhere.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>

#include "fatipc_socket.h"

int fatipc_recv(int socket, struct FatipcBuffer* buffer)
{
    /*
     * recv fd through socket, from
     * http://stackoverflow.com/questions/28003921/sending-file-descriptor-by-linux-socket
     */
    struct msghdr msg;
    memset(&msg, 0, sizeof msg);
    char m_buffer[256];
    struct iovec io = {.iov_base = m_buffer, .iov_len = sizeof(m_buffer) };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    char c_buffer[256];
    msg.msg_control = c_buffer;
    msg.msg_controllen = sizeof(c_buffer);

    if (recvmsg(socket, &msg, 0) < 0) {
        return -1;
    }

    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    unsigned char* data = CMSG_DATA(cmsg);

    int fd = *((int*)data);

    /* got fd, now map new buffer onto it */
    struct stat stat;
    if (fstat(fd, &stat) < 0) {
        close(fd);
        return -3;
    }

    /* mmap */
    void* addr = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        return -4;
    }

    /* alloc and fill up buffer */
    buffer->data = addr;
    buffer->size = stat.st_size;
    buffer->fd = fd;

    return 0;
}

int fatipc_send(int socket, struct FatipcBuffer* buffer)
{
    /*
     * send fd through socket, from
     * http://stackoverflow.com/questions/28003921/sending-file-descriptor-by-linux-socket
     */
    struct msghdr msg;
    memset(&msg, 0, sizeof msg);
    char buf[CMSG_SPACE(sizeof(buffer->fd))];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = {.iov_base = "ABC", .iov_len = 3 };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(buffer->fd));

    *((int*)CMSG_DATA(cmsg)) = buffer->fd;
    msg.msg_controllen = cmsg->cmsg_len;

    if (sendmsg(socket, &msg, 0) < 0) {
        return -1;
    }

    return 0;
}
