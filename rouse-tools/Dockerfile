FROM ubuntu:bionic
COPY rouse-asset /rouse-asset
COPY rouse-svg   /rouse-svg
RUN apt-get update && \
    apt-get -y dist-upgrade && \
    apt-get -y install bash build-essential carton gimp gimp-python libexpat-dev libpng-dev npm potrace && \
    npm -g install svgo && \
    rm -rf /rouse-svg/local /rouse-asset/local /rouse-asset/_Inline && \
    cd /rouse-svg && carton install && \
    cd /rouse-asset && carton install && \
    perl -pe 's/\?\?\?rouse-(asset|svg)-dir\?\?\?/"\/rouse-$1"/g' \
             rouse-asset.template > /usr/local/bin/rouse-asset && \
    chmod +x /usr/local/bin/rouse-asset && \
    (carton exec ./rouse-asset; true) && \
    gimp -i -b '(gimp-quit 1)' && \
    cp rouse.py ~/.gimp-2.8/plug-ins && \
    cp rouse-assetlist /usr/local/bin
