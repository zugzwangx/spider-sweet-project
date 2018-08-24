#!/bin/bash
cd libraries
[ -d ${HOME}/Arduino/libraries ] &&
/bin/ls | while read folders; do
    cp -av ${PWD}/$folders ${HOME}/Arduino/libraries &&
    echo "Installed ${HOME}/Arduino/libraries/$folders"
done
