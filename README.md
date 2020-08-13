# Conclave - Making Bitcoin Scale And Be Useful

Visit us on the www: http://conclave.network

![](https://img.shields.io/badge/license-GPL_3-green.svg "License")

## Disclaimer
This software is under heavy development and comes with **ABSOLUTELY NO WARRANTY**. 
Any Bitcoin or cryptocurrency lost through use of this software is not refundable. 
The authors of this software assume **ABSOLUTELY NO LIABILITY** for any form of loss 
as a result of the use of this software.

## Building

### Linux

Conclave is built using CMake out-of-source build. To build, change into the root 
directory and type:

```
make
```

This will build and run tests. To just run tests:

```
make test
```

To run individual test(s), regexs work too:

```
make test test=id_test
```

If you're developing the Conclave codebase, the `dev` build target will run the server locally with
valgrind:

```
make dev
```

To install Conclave on your local system (under `/usr/local`):

```
sudo make install
```

This will install the Conclave binaries to `/usr/local/bin` and the config files to 
`/usr/local/etc`.

## Docker

Docker support is still in the early stages however the `Dockerfile` will build the full application:

```
docker build -t conclave .
```

The daemon may then be run in a container with:

```
docker run -it conclave conclaved
```


## Running the Daemon

Once it's installed, run `conclaved` by typing:

```
conclaved
```

Or with a custom config file:

```
conclaved --config-file <config file>
```

To get help and see command line options:

```
conclaved --help
```

## Configuration

The Conclave daemon's default configuration file, `conclaved-config.json` resides in the
`etc` directory. The Configuration has a nested structure which matches the structure
of the codebase. Most of the configuration keys should be self-explanatory, but here is
a quick breakdown of the most important ones:

| Key                          |  Description                                                                |
|------------------------------|-----------------------------------------------------------------------------|
| **/PrivateKey**              | Master private key for the node. Do not use the default value.              |
| **/DisplayName**             | The name of your Conclave node. Clashes may happen.                         |
| **/RPC/NumProcessors**       | How many RPC processors to spin up on startup. Each processor has a thread. |
| **/RPC/Acceptor/IPAddress**  | IP Address the RPC acceptor listens on.                                     | 
| **/RPC/Acceptor/Port**       | Port the RPC acceptor listens on.                                          |

## Example RPC Commands

These are some example RPC commands to get you started. cURL is recommended. If you don't have cURL 
on your system, anything that can perform a HTTP POST should work.

Get some info on the node:

```
curl -v -X POST -d '{"method": "NodeInfo"}' http://127.0.0.1:8008/
```

Get balance of an address:

```
curl -v -X POST -d '{"method": "GetAddressBalance", "params": {"address": "1FeexV6bAHb8ybZjqQMjJrcCrHGW9sb6uF"}}' http://127.0.0.1:8008/
```

*Full RPC Reference can be found [here](docs/rpc/Index.md)*
