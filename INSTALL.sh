#!/bin/bash
# Install script will copy the SpiderSweet libraries into
# the user's Arduino library, located at
# ${HOME}/Arduino/libraries

cd libraries
[ -d ${HOME}/Arduino/libraries ] &&
/bin/ls | while read folders; do
    cp -av ${PWD}/$folders ${HOME}/Arduino/libraries &&
    echo "Installed ${HOME}/Arduino/libraries/$folders"
done
