FROM nvidia/cuda:12.4.1-devel-ubuntu25.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"


# Install build essentials + newer GCC
RUN apt update && \
    apt install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt update && \
    apt install -y \
        build-essential \
        gcc-11 g++-11 clang-15 lldb lld\
        cmake git pkg-config mesa-utils x11-apps \
        && rm -rf /var/lib/apt/lists/*

# Set gcc-11 / g++-11 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100

COPY InstallDeps.sh /usr/local/bin/InstallDeps.sh
RUN chmod +x /usr/local/bin/InstallDeps.sh
RUN bash /usr/local/bin/InstallDeps.sh
# 
# COPY Boost.sh /usr/local/bin/Boost.sh
# RUN chmod +x /usr/local/bin/Boost.sh
# RUN bash /usr/local/bin/Boost.sh
# 
# COPY PortAudio.sh /usr/local/bin/PortAudio.sh
# RUN chmod +x /usr/local/bin/PortAudio.sh
# RUN bash /usr/local/bin/PortAudio.sh
# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>
