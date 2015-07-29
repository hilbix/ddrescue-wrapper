#!/bin/bash

. "${0%/*}/.inc"

check "$2"
run sudo ddrescue -fdDb4096 "$1/src" "$1/dst" "$1/log"

