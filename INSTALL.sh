#!/bin/bash
SCRIPTPATH="${BASH_SOURCE%/*}"
cd libraries
[ -d ${HOME}/Arduino/libraries ] &&
/bin/ls | while read folders; do
    cp -av ${PWD}/$folders ${HOME}/Arduino/libraries &&
    echo "Installed ${HOME}/Arduino/libraries/$folders"
done
