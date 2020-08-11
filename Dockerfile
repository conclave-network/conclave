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

#WORKDIR /conclave-build
#COPY . .

#RUN make && make install
