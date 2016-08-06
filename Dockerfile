# libfatipc doesn't have any code dependencies
# This Dockerfile purpose is providing an environment for static analysis, and
# for building and testing with different compilers and instrumentation tools.

FROM ubuntu:16.04

# install debian packages:
ENV DEBIAN_FRONTEND=noninteractive
RUN set -x -e; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        cppcheck clang-format gcc g++ clang cmake make valgrind \
        doxygen graphviz linkchecker \
        cgdb \
        locales git ca-certificates vim time curl gosu sudo

# setup sudo and locale
RUN set -x -e; \
    echo 'ALL ALL=NOPASSWD:ALL' > /etc/sudoers.d/all; \
    chmod 0400 /etc/sudoers.d/all; \
    sed -i '/drop_privileges/d' /usr/bin/linkchecker; \
    sudo locale-gen en_US.UTF-8
ENV LC_ALL=en_US.UTF-8

# setup entrypoint with user UID/GID from host
RUN set -x -e; \
    (\
    echo '#!/bin/bash'; \
    echo 'MY_UID=${MY_UID:-1000}'; \
    echo 'set -x -e'; \
    echo 'useradd -M -u "$MY_UID" -o user'; \
    echo 'chown user:user /home/user'; \
    echo 'cd /home/user/work'; \
    echo 'exec gosu user "${@:-/bin/bash}"'; \
    ) > /usr/local/bin/entrypoint.sh; \
    chmod a+x /usr/local/bin/entrypoint.sh
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]

CMD ["./travis.sh"]

# If your UID is 1000, you can simply run the container as
# docker run -it --rm -v $PWD:/home/user/work ${PWD##*/}
# otherwise, run it as:
# docker run -it --rm -v $PWD:/home/user/work -e MY_UID=$UID ${PWD##*/}
