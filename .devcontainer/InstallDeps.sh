apt update
apt install -y build-essential cmake git \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
    libgl1-mesa-dev libglu1-mesa-dev \
     bzip2  clang clangd lldb gdb

apt install -y wget gnupg lsb-release software-properties-common
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo gpg --dearmor -o /usr/share/keyrings/llvm-archive-keyring.gpg

echo "deb [signed-by=/usr/share/keyrings/llvm-archive-keyring.gpg] http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye-14 main" | sudo tee /etc/apt/sources.list.d/llvm.list

apt update
apt install -y clang-14 lldb-14 lld-14

#wget https://apt.llvm.org/llvm.sh
#chmod +x llvm.sh
#./llvm.sh 14
