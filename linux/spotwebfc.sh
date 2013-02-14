#!/bin/bash

if [ -L $0 ] ; then
    SWFCDIR=$(dirname $(readlink -f $0)) ;
else
    SWFCDIR=$(dirname $0) ;
fi

if [ -n "$LD_LIBRARY_PATH" ]; then
    export LD_LIBRARY_PATH="$SWFCDIR/lib:$LD_LIBRARY_PATH"
else
    export LD_LIBRARY_PATH="$SWFCDIR/lib"
fi

exec "$SWFCDIR/spotwebfc"
