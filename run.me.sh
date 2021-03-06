#!/usr/bin/env sh
set -x
set -e

if [ ! -f ./configure ]; then 
    autoreconf --install
fi

./configure 
make all 
cd images 
./getresources 
./buildpbaroot 
./buildbios 
./buildUEFI64 
mkdir -p UEFI64 
mv UEFI64-1.15*.img.gz UEFI64 
./buildrescue Rescue32 
./buildrescue Rescue64 
cd ..
