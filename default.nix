{ pkgs ? import (
  builtins.fetchTarball {
    url = "https://github.com/nixos/nixpkgs/archive/21.11.tar.gz";
    sha256 = "162dywda2dvfj1248afxc45kcrg83appjd0nmdb541hl7rnncf02";
  }
) {} }:
 
with pkgs;
 
mkShell {
  buildInputs = [
    arduino-cli
  ];
  shellHook = ''
    [ -f "$HOME/.arduino15/arduino-cli.yaml" ] || arduino-cli config init
    arduino-cli core update-index
    arduino-cli core install arduino:samd
    arduino-cli lib install "HX711 Arduino Library"@0.7.5
  '';
}
