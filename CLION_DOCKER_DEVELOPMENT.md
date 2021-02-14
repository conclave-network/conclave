# CLion / Docker Development

The quickest way of getting started developing Concalve is with the following setup which involves Docker and CLion.
Conclave has a lot of dependencies and you may not want to clog up your base system with these, so Docker fixes this
problem, however, some extra setup is required for CLion to "see into" the docker container and resolve toolchains,
libraries, header files, and so forth.

The following setup uses CLion's remote feature which makes CLion behave as if it's running inside the container.

Currently the docker image is based on Ubuntu 18.04. In future we plan to target multiple Unices, Windows, and possibly
other OSes.

This guide assumes you already have Docker and CLion installed.

## Step 0: Get the Conclave Codebase

*You may have done this already. Skip this step if so.*

Clone the Conclave codebase from Github:

```bash
git clone https://github.com/conclave-network/conclave.git
```

## Step 1: Open the Codebase in CLion

1. Open CLion.
2. *File* > *Open*.
3. Navigate to the `conclave` directory.
4. Click **OK**.

## Step 2: Build the CLion Development Image

Open a the terminal tab in CLion (located at the bottom of the CLion window) or an external terminal program if you
prefer. Execute the following docker build command, which will build the `clion-dev` stage, creating an image called
`conclave-clion`:

```bash
docker build --target clion-dev -t conclave-clion .
```

This command may take a while - possibly over an hour.

## Step 3: Create Development Container

Create a container called `conclave-clion-ctr` based on the `conclave-clion` image:

```bash
docker create \
  -it \
  --cap-add sys_ptrace \
  -p 127.0.0.1:2222:22 \
  -p 127.0.0.1:8008:8008 \
  --name conclave-clion-ctr \
  conclave-clion
```

## Step 4: Start Development Container

Start the container:

```bash
docker start conclave-clion-ctr
```

### Test SSH Connection

The CLion development container contains an SSH server listening on port 22, which is exposed on the docker host as port

2222. It also has a user called `conclave` with password `bitcoin`. Ensure you can SSH into the container:

```bash
ssh conclave@localhost -p 2222
```

You may need to run the following command to remove cached SSH keys:

```bash
ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[localhost]:2222"
```

Once you've successfully logged into the container, you can run `whoami` to ensure you are the `conclave` user:

```bash
$ whoami
conclave
```

## Step 5: Enable Remote Development in CLion

1. *File* > *Settings* > *Build, Execution, Deployment* > *Toolchains*.
2. Click **+** to add a new configuration and choose **Remote Host**.
3. Set **Name** to `conclave-docker`.
4. Press the gear icon beside **Credentials** to create a new SSH config.
    1. Press **+** to add a new SSH config.
    2. Set **Host** to `localhost`.
    3. Set **Port** to `2222`.
    3. Set **User name** to `conclave`.
    4. Set **Authentication type** to *Password*
    5. Set **Password** to `bitcoin`.
    6. Click **Test connection**. Ensure it works.
5. Set **CMake** to `/usr/local/bin/cmake`
6. Set **C Compiler** to *Let Cmake detect*
7. Set **C++ Compiler** to *Let Cmake detect*
8. Wait a few seconds until C and C++ compilers are detected.
9. Click **Apply**.
10. Click **OK**.

## Step 6: Create CMake Profile

1. *File* > *Settings* > *Build, Execution, Deployment* > *CMake*.
2. Highlight the existing **Debug** profile.
3. Set **Toolchain** to *conclave-docker*.
9. Click **Apply**.
10. Click **OK**.

## Step 7: Wait for Sync

CLion will now start copying files into the container. Wait until it's finished.

---

## Stopping / Deleting the Container

Stop the container:

```bash
docker stop conclave-clion-ctr
```

Delete the container:

```bash
docker rm conclave-clion-ctr
```

## Deleting the Image

```bash
docker rmi conclave-clion
```