FROM ubuntu

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    gcc \ 
    clang \ 
    make \
    cmake \
    git \
    sudo \
    valgrind \
    cppcheck \
    clang-format \
    clang-tidy \
    gdb \
    pkg-config \
    check \
    lcov \
    pip \
    && \
    pip install gcovr
