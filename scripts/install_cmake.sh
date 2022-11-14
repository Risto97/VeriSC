#!/bin/bash

# if ! command -v cmake &> /dev/null
# then
#     echo "Cmake could not be found, Installing cmake first"
#     wget https://github.com/Kitware/CMake/releases/download/v3.25.0-rc4/cmake-3.25.0-rc4-linux-x86_64.sh
#     chmod +x cmake-3.25.0-rc4-linux-x86_64.sh
#     ./cmake-3.25.0-rc4-linux-x86_64.sh
#     exit
# fi

echo "export PATH=$(pwd)/cmake-3.25.0-rc4-linux-x86_64/bin:\$PATH" > source.sh
