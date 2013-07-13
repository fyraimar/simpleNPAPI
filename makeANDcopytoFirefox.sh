#!/bin/sh

cd /home/fyr/simpleNPAPI
make

sudo rm /usr/lib/mozilla/plugins/simpleNPAPIplugin.so
sudo cp ./simpleNPAPIplugin.so /usr/lib/mozilla/plugins/

