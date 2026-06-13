#!/usr/bin/env bash
set -e

# ---- CONFIG ----
VULKAN_VERSION="1.3.296.0"
SDK_TARBALL="vulkansdk-linux-x86_64-${VULKAN_VERSION}.tar.xz"
SDK_URL="https://sdk.lunarg.com/sdk/download/${VULKAN_VERSION}/linux/${SDK_TARBALL}"

INSTALL_ROOT="/opt/vulkan"
INSTALL_DIR="${INSTALL_ROOT}/${VULKAN_VERSION}"

# ---- SYSTEM DEPS (minimal, Vulkan + X11 tools) ----
apt update
apt install -y wget xz-utils python3 ca-certificates

# ---- DOWNLOAD ----
cd /tmp
rm -f "${SDK_TARBALL}"
wget -q "${SDK_URL}" --show-progress

# ---- EXTRACT ----
tar -xf "${SDK_TARBALL}"

# ---- INSTALL SDK ----
mkdir -p "${INSTALL_ROOT}"
mv "${VULKAN_VERSION}" "${INSTALL_DIR}"

# ---- INSTALL SDK DEPS (LunarG-provided helper) ----
"${INSTALL_DIR}/vulkansdk" --maxjobs all

# ---- ENV SETUP (system-wide) ----
cat >/etc/profile.d/vulkan-sdk.sh <<EOF
# Vulkan SDK ${VULKAN_VERSION}
export VULKAN_SDK=${INSTALL_DIR}
source "\${VULKAN_SDK}/setup_env.sh"
EOF

chmod +x /etc/profile.d/vulkan-sdk.sh

# ---- CLEANUP ----
rm -f "/tmp/${SDK_TARBALL}"

echo "Vulkan SDK ${VULKAN_VERSION} installed at ${INSTALL_DIR}"
echo "Restart your shell or run: source /etc/profile.d/vulkan-sdk.sh"
