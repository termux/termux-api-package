#!/data/data/com.termux/files/usr/bin/bash
set -e

SCRIPTNAME=termux-microphone-record

show_usage () {
    echo "Usage: $SCRIPTNAME [args]"
    echo "Record using microphone on your device"
    echo
    echo "-h           Shows this help"
    echo "-d           Start recording w/ defaults"
    echo "-f <file>    Start recording to specific file"
    echo "-l <limit>   Start recording w/ specified limit (in seconds, unlimited for 0)"
    echo "-e <encoder> Start recording w/ specified encoder (aac, amr_wb, amr_nb)"
    echo "-b <bitrate> Start recording w/ specified bitrate (in kbps)"
    echo "-r <rate>    Start recording w/ specified sampling rate (in Hz)"
    echo "-c <count>   Start recording w/ specified channel count (1, 2, ...)"
    echo "-i           Get info about current recording"
    echo "-q           Quits recording"
}

usage_error () {
    echo
    show_usage
    exit 1
}

sole() {
    if [ "$1 $2" != "-$option $OPTARG" ] || [ $# -gt 2 ]; then
        echo "ERROR: No other options should be specified with -$option!"
        usage_error
    fi
}

add_params() {
    for i in "$@"; do
        params[index++]="$i"
    done
}

call_api () {
    /data/data/com.termux/files/usr/libexec/termux-api MicRecorder "$@"
}

while getopts h,f:,l:,e:,b:,r:,c:,d,i,q option
do
    case "$option" in
        h) sole "$@"
           ;;
        f) record=yes
           dir="$(dirname "$OPTARG")"
           file="$(basename "$OPTARG")"
           dir="$(realpath "$dir")"
           add_params --es file "$dir/$file"
           ;;
        # API takes limit in milliseconds
        l) record=yes
           add_params --ei limit $((OPTARG * 1000))
           ;;
        d) record=yes
           ;;
        e) record=yes
           add_params --es encoder "$OPTARG"
           ;;
        # API takes bitrate in bps
        b) record=yes
           add_params --ei bitrate $((OPTARG * 1000))
           ;;
        r) record=yes
           add_params --ei srate $((OPTARG))
           ;;
        c) record=yes
           add_params --ei channels $((OPTARG))
           ;;
        i) sole "$@"
           add_params -a info
           ;;
        q) sole "$@"
           add_params -a quit
           ;;
        *) usage_error
           ;;
    esac
done

if [ -v record ]; then
    add_params -a record
fi

if [ -v params ]; then
    call_api "${params[@]}"
else
    show_usage
fi
