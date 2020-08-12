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

# OpenSSL (TODO: deprecate)
RUN apt-get install libssl-dev

# LMDB
RUN apt-get install liblmdb++-dev

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

# GSL
RUN git clone https://github.com/imatix/gsl.git \
    && cd gsl \
    && git checkout 20d56ca93748b12f44528f0bc2250305a95327c0 \
    && cd src \
    && make \
    && make install \
    && cd ../..

# Secp256k1
RUN git clone https://github.com/libbitcoin/secp256k1.git \
    && cd secp256k1 \
    && git checkout 1c3616f9f6f8ec4cd88eaccbae08b8cbb04ea326 \
    && ./autogen.sh \
    && ./configure --enable-module-recovery \
    && make \
    && make install \
    && cd ..

# Libbitcoin
RUN git clone https://github.com/libbitcoin/libbitcoin-system.git \
    && git clone https://github.com/libbitcoin/libbitcoin-build.git \
    && cd libbitcoin-build \
    && git checkout 9e0f971377010f92ca12b8cccbefb251bb937fb4 \
    && ./generate.sh \
    && cd ../libbitcoin-system \
    && git checkout bef130bdc8c57808dceab99d6867f3c58acf4e92 \
    && ./install.sh \
    && ./autogen.sh \
    && ./configure \
    && make \
    && make install \
    && cd ..

# CMake
ARG CMAKE_VERSION_DOT=3.17.0
ARG CMAKE_HASH=b44685227b9f9be103e305efa2075a8ccf2415807fbcf1fc192da4d36aacc9f5
RUN set -ex \
    && curl -L -o cmake-${CMAKE_VERSION_DOT}-Linux-x86_64.tar.gz \
    https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION_DOT}/cmake-${CMAKE_VERSION_DOT}-Linux-x86_64.tar.gz \
    && echo "${CMAKE_HASH}  cmake-${CMAKE_VERSION_DOT}-Linux-x86_64.tar.gz" | sha256sum -c \
    && tar -xvf cmake-${CMAKE_VERSION_DOT}-Linux-x86_64.tar.gz \
    && cp -r cmake-${CMAKE_VERSION_DOT}-Linux-x86_64/bin/* /usr/local/bin \
    && cp -r cmake-${CMAKE_VERSION_DOT}-Linux-x86_64/doc/* /usr/local/doc \
    && cp -r cmake-${CMAKE_VERSION_DOT}-Linux-x86_64/man/* /usr/local/man \
    && cp -r cmake-${CMAKE_VERSION_DOT}-Linux-x86_64/share/* /usr/local/share \
    && cd ..

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
    && cd ..

WORKDIR /conclave-build
COPY . .
RUN make && make install

