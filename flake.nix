{
  description = "C++ development environment with Clang, CMake, and vcpkg";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default = pkgs.mkShell {
          name = "cpp-dev-shell";

          buildInputs = [
            pkgs.clang
            pkgs.cmake
            pkgs.git
            pkgs.python3
            pkgs.gnumake
            pkgs.vim
            pkgs.vcpkg
            pkgs.portaudio
            pkgs.alsa-lib
            pkgs.pkg-config
            pkgs.mesa  
            pkgs.freetype  
            pkgs.gdb  
            pkgs.lldb
            pkgs.glfw
            pkgs.glew
            pkgs.imgui
            pkgs.implot
            pkgs.boost
          ];

        };
      });
}
