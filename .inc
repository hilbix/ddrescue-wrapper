#!/bin/bash

OOPS()
{
echo "OOPS: $*" >&2
exit 2
}

WARN()
{
echo "WARN: $*" >&2
}

case "$1" in
*[./]*)		OOPS "$1";;
'')		OOPS "missing arg $1";;
esac

echo

case "$1" in
(*/*)	OOPS "no path, please";;
(.|..)	OOPS "no relative path, please";;
(*[^-._a-zA-Z0-9]*)	OOPS "sane names, please";;
esac

[ -d "$1" ] || OOPS "not dir $1"

[ -w "$1/src" ] && WARN "src is writeable"
a="$(blkid "$1/src")" || OOPS "blkid src"

[ -w "$1/dst" ] || OOPS "dst not writeable"

TARGET="$1"

check()
{
if	b="$(blkid "$TARGET/dst")"
then
	[ ".${a#$TARGET/src}" = ".${b#$TARGET/dst}" ] || OOPS "blkid mismatch: '$a' != '$b'"
	[ -n "$1" ] || OOPS 'safety stop: dst not empty, give nonempty 2nd arg to override'
fi
}

run()
{
{
flock -x 6 || OOPS "cannot lock $TARGET/.lock"
{ "$@"; } 6<&-
} 6>> "$TARGET/.lock"
}

