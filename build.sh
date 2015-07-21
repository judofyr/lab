set -e

NANOVG="vendor/nanovg"
GL3W="vendor/gl3w"
HARFBUZZ="vendor/harfbuzz"
FREETYPE2="vendor/freetype2"

CXX="clang++"
CC="clang"

LIBS="-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo"
LIBS="$LIBS `pkg-config --libs glfw3`"
LIBS="$LIBS -lharfbuzz -lfreetype -lglib-2.0"

CFLAGS="-I. -I$NANOVG/src -I$GL3W/include -I$HARFBUZZ/src -I$FREETYPE2/include -g -Wall -pedantic"
CFLAGS="$CFLAGS -Wno-nested-anon-types -Wno-gnu-anonymous-struct"

CCFLAGS="-std=c11 $CFLAGS"
CXXFLAGS="-std=c++14 $CFLAGS"

CXXFLAGS="$CXXFLAGS `pkg-config --cflags glfw3`"

LDFLAGS=""
LDFLAGS="$LDFLAGS -L$HARFBUZZ/src/.libs -L$FREETYPE2/objs"

LDFLAGS="$LDFLAGS $LIBS"
DYLIBFLAGS="-dynamiclib -undefined dynamic_lookup"
DYLIBEXT="dylib"

mkdir -p build

OBJS=()

compile_cc() {
  local base="${1##*/}"

  local dir="build/$2"
  mkdir -p "$dir"
  local obj="$dir/${base%.*}.o"

  OBJS+=($obj)
  $CC $CCFLAGS -c -o "$obj" "$1"
}

compile_cxx() {
  local base="${1##*/}"

  local dir="build/$2"
  mkdir -p "$dir"
  local obj="$dir/${base%.*}.o"

  OBJS+=($obj)
  $CXX $CXXFLAGS -c -o "$obj" "$1"
}

clear_objs() {
  OBJS=()
}

compile_cc "$GL3W/src/gl3w.c"
compile_cc "$NANOVG/src/nanovg.c"
compile_cxx ui/canvas.cc
compile_cxx ui/atom.cc
compile_cxx ui/fonts.cc
compile_cxx ui/text.cc
compile_cxx ui/math.cc
compile_cxx main.cc

$CXX $CXXFLAGS $LDFLAGS -o build/lab ${OBJS[@]}
clear_objs

compile_cxx physics/spring.cc physics
$CXX $CXXFLAGS $LDFLAGS $DYLIBFLAGS -o build/spring.$DYLIBEXT ${OBJS[@]}

