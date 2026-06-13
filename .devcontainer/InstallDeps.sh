apt update
apt install -y build-essential cmake git \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
    libgl1-mesa-dev libglu1-mesa-dev \
     bzip2  clang clangd gdb

apt install -y wget gnupg lsb-release software-properties-common

apt update
apt install -y clang-14

#wget https://apt.llvm.org/llvm.sh
#chmod +x llvm.sh
#./llvm.sh 14
