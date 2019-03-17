#!/usr/bin/env bash
BASEDIR=$(dirname "$0")

LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$BASEDIR" "$BASEDIR/AdvancedSettings" "$@"
