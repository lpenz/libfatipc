# libfatipc doesn't have any code dependencies
# This Dockerfile purpose is providing an environment for static analysis, and
# for building and testing with different compilers and instrumentation tools.

FROM debian:bullseye

# install debian packages:
ENV DEBIAN_FRONTEND=noninteractive
RUN set -e -x; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        cppcheck clang-format gcc g++ clang cmake make valgrind \
        doxygen graphviz linkchecker \
        cgdb \
        locales git ca-certificates vim time curl gosu sudo

# setup su and locale
RUN set -e -x; \
    sed -i '/pam_rootok.so$/aauth sufficient pam_permit.so' /etc/pam.d/su; \
    echo 'en_US.UTF-8 UTF-8' >> /etc/locale.gen; locale-gen
ENV LC_ALL=en_US.UTF-8

CMD ["./travis.sh"]

