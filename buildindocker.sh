#!/bin/bash

docker build -t sedutilladarbuiler docker
# need to run a privileged container to "mount" and create sys images
docker run --rm -it -v $PWD/:/sedutil --privileged sedutilladarbuiler