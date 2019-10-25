#!/data/data/com.termux/files/usr/bin/bash
set -e -u

SCRIPTNAME=termux-usb
show_usage () {
    echo "Usage: $SCRIPTNAME [-l | [-r] [-e command] device]"
    echo "List or access USB devices. Devices cannot be accessed directly,"
    echo "                 only using $SCRIPTNAME."
    echo "  -l           list available devices"
    echo "  -r           show permission request dialog if necessary"
    echo "  -e command   execute the specified command with a file descriptor"
    echo "                 referring to the device as its argument"
    exit 0
}

ACTION="permission"
PARAMS=""
MASK=0
while getopts :hlre: option
do
    case "$option" in
        h) show_usage;;
        l) ACTION="list"; ((MASK |= 1));;
        r) PARAMS="$PARAMS --ez request true"; ((MASK |= 2));;
        e) ACTION="open"; export TERMUX_CALLBACK="$OPTARG"; ((MASK |= 2));;
        ?) echo "$SCRIPTNAME: illegal option -$OPTARG"; exit 1;
    esac
done
shift $((OPTIND-1))

if [ $MASK -eq 3 ]; then echo "$SCRIPTNAME: -l cannot be combined with other options"; exit 1; fi

if [ "$ACTION" == "list" ]
then
    if [ $# -gt 0 ]; then echo "$SCRIPTNAME: too many arguments"; exit 1; fi
else
    if [ $# -gt 1 ]; then echo "$SCRIPTNAME: too many arguments"; exit 1; fi
    if [ $# -lt 1 ]; then echo "$SCRIPTNAME: missing -l or device path"; exit 1; fi
    PARAMS="$PARAMS --es device $1"
fi

CMD="/data/data/com.termux/files/usr/libexec/termux-api Usb -a $ACTION $PARAMS"

if [ "$ACTION" == "permission" ]
then
    if [ "$($CMD)" == "yes" ]
    then
        echo "Access granted."
        exit 0
    else
        echo "Access denied."
        exit 1
    fi
else
    $CMD
fi

