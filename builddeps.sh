git submodule update --init

HARFBUZZ="$PWD/vendor/harfbuzz"
FREETYPE2="$PWD/vendor/freetype2"
GL3W="$PWD/vendor/gl3w"

if [ "$1" = release ]; then
  export CFLAGS="-c -O3"
else
  export CFLAGS="-c -g -O0"
fi

pushd $FREETYPE2
sh autogen.sh
make setup ansi
make
popd

pushd $HARFBUZZ
FREETYPE_CFLAGS="-I$FREETYPE2/include" \
FREETYPE_LIBS="-L$FREETYPE2/objs -lfreetype" \
sh autogen.sh --disable-shared --enable-static
make
popd

pushd $GL3W
python gl3w_gen.py
popd

