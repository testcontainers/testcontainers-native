{
  description = "Testcontainers Native";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs { inherit system; };
  in {
    devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
            clang-tools
            cmake
            gcc15
            gdb
            go
            ninja
            pre-commit
        ];
    };
  };
}
