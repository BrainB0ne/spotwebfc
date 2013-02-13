#!/bin/bash

if [ -n "$LD_LIBRARY_PATH" ]; then
    export LD_LIBRARY_PATH="./lib:$LD_LIBRARY_PATH"
else
    export LD_LIBRARY_PATH="./lib"
fi

exec "./spotwebfc"
