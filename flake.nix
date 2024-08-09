{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    treefmt-nix.url = "github:numtide/treefmt-nix";
  };

  outputs = {
    nixpkgs,
    treefmt-nix,
    systems,
    ...
  }: let
    pkgs = nixpkgs.legacyPackages."x86_64-linux";
    eachSystem = f:
      nixpkgs.lib.genAttrs (import systems) (system: f nixpkgs.legacyPackages.${system});
    treefmtEval = eachSystem (pkgs:
      treefmt-nix.lib.evalModule pkgs ({pkgs, ...}: {
        projectRootFile = "flake.nix";
        programs.clang-format.enable = true;
        settings.formatter = {
          "qmlformat" = {
            command = "${pkgs.qt6.qtdeclarative}/bin/qmlformat";
            options = [
              "-i"
            ];
            includes = ["*.qml"];
          };
        };
      }));
  in {
    formatter."x86_64-linux" = treefmtEval.${pkgs.system}.config.build.wrapper;
    devShells."x86_64-linux" = {
      #default = pkgs.mkShell {
      default = pkgs.pkgsLLVM.stdenv.mkDerivation {
        name = "shell";
        nativeBuildInputs = builtins.attrValues {
          inherit
            (pkgs)
            cmake
            ninja
            pkg-config
            clang-tools_18
            python3
            cppcheck
            include-what-you-use
            valgrind
            qtcreator
            vulkan-headers
            gdb
            appimagekit
            ;
        };
        propagatedBuildInputs = builtins.attrValues {
          inherit (pkgs) pulseaudio glslang vulkan-loader vulkan-tools vulkan-validation-layers;
          inherit
            (pkgs.qt6)
            qtbase
            qtmultimedia
            qtdeclarative
            qtwebsockets
            qtsvg
            ;
          inherit
            (pkgs.xorg)
            libX11
            libXt
            libXtst
            ;
        };
        #VULKAN_SDK = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";
        QT_VULKAN_LIB = "${pkgs.vulkan-loader}/lib/libvulkan.so";
        shellHook = ''
          LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${pkgs.libGL}/lib:${pkgs.vulkan-loader}/lib"

          # read the setting for nullglob ('on' or 'off')
          nullglob=$(shopt nullglob | cut -f 2)
          # turn on nullglob
          if [ $nullglob = "off" ]; then
              shopt -s nullglob
          fi

          QML2_IMPORT_PATH=
          for v in $(echo "$CMAKE_PREFIX_PATH" | tr : '\n' | sort | uniq); do
              list=($v/lib/*/qml)
              for f in "''${list[@]}"; do
                  if [ -z "$QML2_IMPORT_PATH" ]; then
                      QML2_IMPORT_PATH=$f
                  else
                      QML2_IMPORT_PATH=$QML2_IMPORT_PATH:$f
                  fi
              done
          done
          export QML2_IMPORT_PATH

          QT_PLUGIN_PATH=
          for v in $(echo "$CMAKE_PREFIX_PATH" | tr : '\n' | sort | uniq); do
              list=($v/lib/*/plugins)
              for f in "''${list[@]}"; do
                  if [ -z "$QT_PLUGIN_PATH" ]; then
                      QT_PLUGIN_PATH=$f
                  else
                      QT_PLUGIN_PATH=$QT_PLUGIN_PATH:$f
                  fi
              done
          done
          export QT_PLUGIN_PATH

          # turn nullglob back to the initial value
          if [ $nullglob = "off" ]; then
              shopt -u nullglob
          fi

          PS1="DEVELOP $PS1"
        '';
      };
    };
  };
}
