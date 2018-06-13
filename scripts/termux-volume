#!/data/data/com.termux/files/usr/bin/bash
set -e -u

SCRIPTNAME=termux-volume
show_usage () {
    echo "Usage: $SCRIPTNAME stream volume"
    echo "Change volume of audio stream"
    echo "Valid audio streams are: alarm, music, notification, ring, system, call"
    echo "Call w/o arguments to show information about each audio stream"
    exit 0
}

ARGS=""

# If we have arguments, we must have 2
# Otherwise we will just return JSON info to user
if [ $# -gt 0 ]; then
    if [ $# -ne 2 ]; then
        echo "Invalid argument count"
        show_usage
    fi

    if ! [[ $2 =~ ^[0-9]+$ ]]; then
        echo "ERROR: Volume must be a number"
        show_usage
    fi
    ARGS="-a set-volume --es stream $1 --ei volume $2"
fi

/data/data/com.termux/files/usr/libexec/termux-api Volume $ARGS
