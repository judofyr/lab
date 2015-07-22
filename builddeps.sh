git submodule update --init

HARFBUZZ="$PWD/vendor/harfbuzz"
FREETYPE2="$PWD/vendor/freetype2"
GL3W="$PWD/vendor/gl3w"
BUILD="$PWD/build"

pushd $GL3W
python gl3w_gen.py
popd

build() {
  mkdir -p "$BUILD/$TYPE"

  (
    cd $FREETYPE2
    export CFLAGS="-c $CFLAGS"
    make clean 2>/dev/null
    sh autogen.sh
    make setup ansi
    make
    cp objs/libfreetype.a "$BUILD/$TYPE"
  )

  (
    cd $HARFBUZZ
    export FREETYPE_CFLAGS="-I$FREETYPE2/include"
    export FREETYPE_LIBS="-L$FREETYPE2/objs -lfreetype"
    make clean 2>/dev/null
    sh autogen.sh --disable-shared --enable-static --with-glib=no --with-coretext=no "$HARFBUZZ_CONF"
    make
    cp src/.libs/libharfbuzz.a "$BUILD/$TYPE"
  )
}

EXTRA_FLAGS="-O3"

setup_ios() {
  SDK="$1"
  ARCH="$2"
  CARCH="${3:-$2}"
  TYPE="$SDK-$ARCH"
  HOST_FLAGS="-arch $ARCH -miphoneos-version-min=8.0 -isysroot $(xcrun -sdk ${SDK} --show-sdk-path)"
  export CC="$(xcrun -find -sdk ${SDK} cc)"
  export CXX="$(xcrun -find -sdk ${SDK} c++)"
  export CFLAGS="$HOST_FLAGS $EXTRA_FLAGS"
  export CXXFLAGS="$CFLAGS"
  export LDFLAGS="$CFLAGS"
  HARFBUZZ_CONF="--host=$CARCH-apple-darwin"
}

if [ "$1" = ios ]; then
  setup_ios iphoneos arm64 arm
  build

  setup_ios iphoneos armv7 arm
  build

  setup_ios iphonesimulator x86_64
  build
else
  TYPE="host"
  export CFLAGS="$EXTRA_FLAGS"
  build
fi
