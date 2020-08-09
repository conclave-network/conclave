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
        git

# OpenSSL (TODO: deprecate)
RUN apt-get install libssl-dev

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

#WORKDIR /conclave-build
#COPY . .

#RUN make && make install
