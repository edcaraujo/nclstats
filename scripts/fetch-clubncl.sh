#!/bin/bash

#
# Download all NCL applications from Clube-NCL 
#
# File: fetch-clubncl.sh
# Date: 2012/11/21
#
# More information:
# edcaraujo@telemidia.puc-rio.br
#

#
# Fetching
#
echo "[1/5] Fetching applications..."

wget -q http://clube.ncl.org.br/rss.xml -O apps.rss

#
# Preparing
#
echo "[2/5] Preparing..."

cat apps.rss | grep node | sed "s/.*\(node\/[0-9]*\).*/http:\/\/clube\.ncl\.org\.br\/\1/g" > apps.nodes
sort apps.nodes | uniq apps.nodes > apps.tmp

mv apps.tmp apps.nodes

cat apps.nodes | xargs wget -q
cat apps.nodes | sed s/[^0-9]*//g | xargs cat | grep zip | sed "s/.*href=\"\(.*\.zip\)\".*/\1/g" | sed "s/^<p>.*"//g > apps.url

#
# Downloading
#
echo "[3/5] Downloading applications..."

cat apps.url | xargs wget 

#
# Unpacking
#
echo "[4/5] Unpacking applications..."

mkdir apps

unzip -q \*zip -d apps

#
# Cleaning
#
echo "[5/5] Cleaning..."

cat apps.url | sed "s/.*\/\(.*\.zip\)$/\1/g" | xargs rm
cat apps.nodes | sed s/[^0-9]*//g | xargs rm

echo "Done!"
