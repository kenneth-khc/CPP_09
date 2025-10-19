{
  description = "C++ devshell";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells = with pkgs; {
          default = mkShell {
            packages = with pkgs; [
              clang-tools
              gnumake
              gdb
              bear
            ];
            hardeningDisable = [ "all" ];
          };
          shellHook = ''
            export SHELL=${lib.getExe pkgs.bash};
          '';
        };
      }
    );
}
