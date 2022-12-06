#!/bin/sh

mkdir pong.iconset
sips -z 16 16     pong1024.png --out pong.iconset/icon_16x16.png
sips -z 32 32     pong1024.png --out pong.iconset/icon_16x16@2x.png
sips -z 32 32     pong1024.png --out pong.iconset/icon_32x32.png
sips -z 64 64     pong1024.png --out pong.iconset/icon_32x32@2x.png
sips -z 128 128   pong1024.png --out pong.iconset/icon_128x128.png
sips -z 256 256   pong1024.png --out pong.iconset/icon_128x128@2x.png
sips -z 256 256   pong1024.png --out pong.iconset/icon_256x256.png
sips -z 512 512   pong1024.png --out pong.iconset/icon_256x256@2x.png
sips -z 512 512   pong1024.png --out pong.iconset/icon_512x512.png
cp pong1024.png pong.iconset/icon_512x512@2x.png
iconutil -c icns pong.iconset
rm -R pong.iconset
