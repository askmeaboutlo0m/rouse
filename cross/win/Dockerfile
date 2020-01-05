FROM alpine:20191219
RUN apk update && \
    apk add autoconf automake cmake git libtool make mingw-w64-gcc ninja && \
    mkdir -p /deps /deps/prefix /deps/prefix/bin /deps/prefix/include /deps/prefix/lib && \
    cd /deps && \
    git clone https://github.com/recp/cglm && \
    cd cglm && \
    sh autogen.sh && \
    ./configure --host=x86_64-w64-mingw32 --disable-dependency-tracking --prefix=/deps/prefix && \
    make && \
    make install && \
    cd /deps && \
    wget -O SDL2.tar.gz https://www.libsdl.org/release/SDL2-devel-2.0.10-mingw.tar.gz && \
    tar xzf SDL2.tar.gz && \
    rm SDL2.tar.gz && \
    cp SDL2-*/x86_64-w64-mingw32/bin/*.dll prefix/bin && \
    cp SDL2-*/x86_64-w64-mingw32/lib/*.* prefix/lib && \
    cp -r SDL2-*/x86_64-w64-mingw32/include/SDL2 prefix/include && \
    wget -O SDL2_image.tar.gz https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz && \
    tar xzf SDL2_image.tar.gz && \
    rm SDL2_image.tar.gz && \
    cp SDL2_image-*/x86_64-w64-mingw32/bin/* prefix/bin && \
    cp SDL2_image-*/x86_64-w64-mingw32/lib/*.* prefix/lib && \
    cp SDL2_image-*/x86_64-w64-mingw32/include/SDL2/* prefix/include/SDL2
