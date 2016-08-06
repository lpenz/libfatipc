[![Build Status](https://travis-ci.org/lpenz/libfatipc.svg?branch=master)](https://travis-ci.org/lpenz/libfatipc)
[![codecov](https://codecov.io/gh/lpenz/libfatipc/branch/master/graph/badge.svg)](https://codecov.io/gh/lpenz/libfatipc)

# libfatipc

**libfatipc** sends big chunks of memory efficiently between two
processes on the same host.

It does that by using a buffer backed by a memory-mapped file, whose file
descriptor is sent to the other process by using the technique described in
§15.7 Passing Descriptors of [Stevens' UNIX Network Programming, Vol 1]
(https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551).
That means that zero copies are performed between the processes and the kernel
while preserving the move semantics - no locks are required, even though we are
using a shared memory.


## Using the library


### No socket abstraction

At this moment, this libraries provides no socket abstraction. We feel that
there is already enough documentation, socket tutorials and even libraries that
do a great job providing the abstraction, and you can use any of them.
You can take a look at
http://stackoverflow.com/questions/118945/best-c-c-network-library for
options. For instance, https://github.com/dermesser/libsocket looks really
nice and is compatible with this library.


### Creating and sending a buffer

```.c
#include "fatipc_buffer.h"
#include "fatipc_socket.h"

/* ... */

    /* Allocate the buffer, check for error: */
    struct FatipcBuffer buff;
    if (fatipc_alloc(50, &buff) != 0)
        return -1;

    /* Write to it... */
    memset(buff.data, 0, 50);

    /* Send it through sock, check error: */
    if (fatipc_send(sock, &buff) != 0) {
        fatipc_free(&buff); /* Have to free it on error */
        return -1;
    }

    /*
     * We can now read from the buffer, but not write to it, as that leads to
     * undefined behaviour, and might affect the other process.
     */

    /* But we have to free the current process' link to it: */
    fatipc_free(&buff);
```


### Receiving the buffer

```.c
#include "fatipc_buffer.h"
#include "fatipc_socket.h"

/* ... */

    /*
     * Receive the buffer.
     * This call blocks if there is no data in the socket.
     */
    struct FatipcBuffer buff;
    if (fatipc_recv(sock, &buff) != 0)
        return -1;

    /*
     * We can now read and write to the buffer, and even forward
     * it through another socket.
     */

    /* After we are done with it, we have to free it: */
    fatipc_free(&buff);
```


## Building and running the code

The code and the examples depend only on `gcc` and `cmake`. To build it:
```.sh
cmake .
make
```

To run the tests:
```.sh
./fatipc_test
```

A Dockerfile that creates a
[docker container](https://docker.io) is present in this repository. You can
use it to build everything - you might not have google test library and doxygen
installed, for instance. To create the container, under Linux:
```.sh
docker build -t ${PWD##*/}
```

To enter the container:
```.sh
docker run -it --rm -v $PWD:/home/user/work -e MY_UID=$UID ${PWD##*/} /bin/bash
```

You can then run the build commands inside the container.

To build the documentation, run `doxygen`.
