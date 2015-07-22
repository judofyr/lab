git submodule update --init

HARFBUZZ="$PWD/vendor/harfbuzz"
FREETYPE2="$PWD/vendor/freetype2"
GL3W="$PWD/vendor/gl3w"

export CFLAGS="-c -O3"
pushd $FREETYPE2
sh autogen.sh
make setup ansi
make
popd

export CFLAGS="-O3"
pushd $HARFBUZZ
FREETYPE_CFLAGS="-I$FREETYPE2/include" \
FREETYPE_LIBS="-L$FREETYPE2/objs -lfreetype" \
sh autogen.sh --disable-shared --enable-static --with-glib=no --with-coretext=no
make
popd

pushd $GL3W
python gl3w_gen.py
popd

