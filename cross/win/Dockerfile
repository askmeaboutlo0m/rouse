FROM alpine:3.12

RUN apk update && \
    apk add autoconf automake cmake git libtool make mingw-w64-gcc ninja openal-soft-dev bison && \
    mkdir -p /deps /deps/prefix /deps/prefix/bin /deps/prefix/include /deps/prefix/lib

RUN cd /deps && \
    git clone https://github.com/cmusphinx/sphinxbase.git && \
    cd sphinxbase && \
    ./autogen.sh --host=x86_64-w64-mingw32 --prefix=/deps/prefix --with-python=no --without-lapack && \
    make && \
    make install

RUN cd /deps && \
    git clone https://github.com/cmusphinx/pocketsphinx.git && \
    cd pocketsphinx && \
    ./autogen.sh --host=x86_64-w64-mingw32 --prefix=/deps/prefix --with-python=no && \
    make && \
    make install

RUN cd /deps && \
    git clone https://github.com/recp/cglm && \
    cd cglm && \
    sh autogen.sh && \
    ./configure --host=x86_64-w64-mingw32 --disable-dependency-tracking --prefix=/deps/prefix && \
    make && \
    make install

RUN cd /deps && \
    git clone https://github.com/erincatto/box2d.git && \
    cd box2d && \
    git checkout v2.4.1 && \
    cmake -B build -G Ninja \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_SYSTEM_VERSION=1 \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=OFF \
        -DCMAKE_C_COMPILER="$(which x86_64-w64-mingw32-gcc)" \
        -DCMAKE_C_COMPILER_AR="$(which x86_64-w64-mingw32-gcc-ar)" \
        -DCMAKE_C_COMPILER_RANLIB="$(which x86_64-w64-mingw32-gcc-ranlib)" \
        -DCMAKE_AR="$(which x86_64-w64-mingw32-ar)" \
        -DBOX2D_BUILD_UNIT_TESTS=OFF \
        -DBOX2D_BUILD_TESTBED=OFF && \
    cmake --build build && \
    cmake --install build --prefix /deps/prefix

RUN cd /deps && \
    wget -O SDL2.tar.gz https://www.libsdl.org/release/SDL2-devel-2.0.10-mingw.tar.gz && \
    tar xzf SDL2.tar.gz && \
    rm SDL2.tar.gz && \
    cp SDL2-*/x86_64-w64-mingw32/bin/*.dll prefix/bin && \
    cp SDL2-*/x86_64-w64-mingw32/lib/*.* prefix/lib && \
    cp -r SDL2-*/x86_64-w64-mingw32/include/SDL2 prefix/include

RUN cd /deps && \
    wget -O SDL2_image.tar.gz https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz && \
    tar xzf SDL2_image.tar.gz && \
    rm SDL2_image.tar.gz && \
    cp SDL2_image-*/x86_64-w64-mingw32/bin/* prefix/bin && \
    cp SDL2_image-*/x86_64-w64-mingw32/lib/*.* prefix/lib && \
    cp SDL2_image-*/x86_64-w64-mingw32/include/SDL2/* prefix/include/SDL2
