#!/bin/bash

fname_out="${2%.icns}"
mkdir -p $fname_out.iconset
swift $(dirname "$BASH_SOURCE")/rgba2png.swift $1 $fname_out.iconset/icon_512x512@2x.png
iconutil --convert icns $fname_out.iconset
rm -r $fname_out.iconset
