#!/data/data/com.termux/files/usr/bin/bash
set -e

SCRIPTNAME=termux-media-player

show_usage () {
    echo "Usage: $SCRIPTNAME cmd [args]"
    echo
    echo "help        Shows this help"
    echo "info        Displays current playback information"
    echo "play        Resumes playback if paused"
    echo "play <file> Plays specified media file"
    echo "pause       Pauses playback"
    echo "stop        Quits playback"
}

main() {
    /data/data/com.termux/files/usr/libexec/termux-api MediaPlayer "$@"
}

case "$1" in
    "play" )
        if [ $# -gt 2 ]; then
            echo "Error! $SCRIPTNAME can only play one file at a time!"
            exit 1
        elif [ $# -gt 1 ]; then
            if [ ! -f "$2" ]; then
                echo "Error: '$2' is not a file!"
                exit 1
            else
                main -a play --es file "$(realpath "$2")"
            fi
        else
            main -a resume
        fi
        ;;
    "info" | "pause" | "stop" )
        if [ $# -gt 1 ]; then
            echo "Error! '$1' takes no arguments!"
            exit 1
        else
            main -a "$1"
        fi
        ;;
    "help" | "" )
        show_usage
        ;;
    * )
        echo "$SCRIPTNAME: Invalid cmd: '$1'"
        show_usage
        exit 1
        ;;
esac
