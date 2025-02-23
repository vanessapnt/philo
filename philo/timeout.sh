#!/bin/bash
# timeout.sh

# Usage: ./timeout.sh <seconds> <command>

TIMEOUT=$1
shift
COMMAND="$@"

(
  sleep $TIMEOUT
  kill $! 2>/dev/null
) &

exec $COMMAND
