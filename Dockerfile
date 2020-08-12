# Build stage
FROM ubuntu:18.04 as build
WORKDIR /root

RUN set -ex \
    && apt-get update \
    && apt-get --no-install-recommends --yes install \
        ca-certificates \
        build-essential \
        autoconf \
        automake \
        libtool \
        pkg-config \
        libpcre* \
        zlib* \
        libpq-dev \
        libpqxx-dev \
        curl \
        git \
        valgrind

# CMake
ARG CMAKE_VERSION_DOT=3.17.0
ARG CMAKE_HASH=b44685227b9f9be103e305efa2075a8ccf2415807fbcf1fc192da4d36aacc9f5
RUN set -ex \
    && curl -L -o cmake-${CMAKE_VERSION_DOT}-Linux-x86_64.tar.gz \
    https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION_DOT}/cmake-${CMAKE_VERSION_DOT}-Linux-x86_64.tar.gz \
    && echo "${CMAKE_HASH}  cmake-${CMAKE_VERSION_DOT}-Linux-x86_64.tar.gz" | sha256sum -c \
    && tar -xvf cmake-${CMAKE_VERSION_DOT}-Linux-x86_64.tar.gz \
    && cd cmake-${CMAKE_VERSION_DOT}-Linux-x86_64 \
    && cp -r bin/* /usr/local/bin/ \
    && cp -r doc/* /usr/local/doc/ \
    && cp -r man/* /usr/local/man/ \
    && cp -r share/* /usr/local/share/ \
    && cd ..

# OpenSSL (TODO: deprecate)
RUN apt-get --no-install-recommends --yes install libssl-dev

# Boost
ARG BOOST_VERSION=1_72_0
ARG BOOST_VERSION_DOT=1.72.0
ARG BOOST_HASH=59c9b274bc451cf91a9ba1dd2c7fdcaf5d60b1b3aa83f2c9fa143417cc660722
RUN set -ex \
    && curl -L -o boost_${BOOST_VERSION}.tar.bz2 \
    https://dl.bintray.com/boostorg/release/${BOOST_VERSION_DOT}/source/boost_${BOOST_VERSION}.tar.bz2 \
    && echo "${BOOST_HASH}  boost_${BOOST_VERSION}.tar.bz2" | sha256sum -c \
    && tar -xvf boost_${BOOST_VERSION}.tar.bz2 \
    && cd boost_${BOOST_VERSION}/tools/build \
    && ./bootstrap.sh \
    && ./b2 install --prefix=/usr/local \
    && cd ../.. \
    && b2 --build-dir=/tmp/build-boost toolset=gcc stage \
    && b2 install
ENV BOOST_ROOT /usr/local/boost_${BOOST_VERSION}

# Poco
ARG POCO_VERSION_DOT=1.10.1
ARG POCO_HASH=7f5931e0bb06bc2880a0f3867053a2fddf6c0d3e5dd96342a665460301fc34ca
RUN set -ex \
    && curl -L -o poco-${POCO_VERSION_DOT}-all.tar.gz \
    https://pocoproject.org/releases/poco-${POCO_VERSION_DOT}/poco-${POCO_VERSION_DOT}-all.tar.gz \
    && echo "${POCO_HASH}  poco-${POCO_VERSION_DOT}-all.tar.gz" | sha256sum -c \
    && tar -xvf poco-${POCO_VERSION_DOT}-all.tar.gz \
    && cd poco-${POCO_VERSION_DOT}-all \
    && ./configure \
    && make \
    && make install \
    && ldconfig \
    && cd ..

# LMDB
RUN apt-get --no-install-recommends --yes install liblmdb++-dev

# Secp256k1
RUN git clone https://github.com/libbitcoin/secp256k1.git \
    && cd secp256k1 \
    && git checkout 1c3616f9f6f8ec4cd88eaccbae08b8cbb04ea326 \
    && ./autogen.sh \
    && ./configure --enable-module-recovery \
    && make \
    && make install \
    && ldconfig \
    && cd ..

# Libbitcoin system
RUN git clone https://github.com/libbitcoin/libbitcoin-system.git \
    && cd libbitcoin-system \
    && git checkout bef130bdc8c57808dceab99d6867f3c58acf4e92 \
    && ./autogen.sh \
    && ./configure \
    && make \
    && make install \
    && ldconfig \
    && cd ..

# Conclave build
WORKDIR /conclave-build
COPY . .
RUN set -ex \
    && make \
    && make test \
    && make install
