#!/data/data/com.termux/files/usr/bin/sh
set -e -u

SCRIPTNAME=termux-speech-to-text
show_usage () {
    echo "Usage: $SCRIPTNAME"
    echo "Converts speech to text, sending partial matches to stdout."
    exit 0
}

show_progress=false
while getopts :hp option
do
    case "$option" in
        h) show_usage;;
        p) show_progress=true;;
        ?) echo "$SCRIPTNAME: illegal option -$OPTARG"; exit 1;
    esac
done
shift $((OPTIND-1))

if [ $# != 0 ]; then echo "$SCRIPTNAME: too many arguments"; exit 1; fi

CMD=/data/data/com.termux/files/usr/libexec/termux-api
if [ $show_progress = true ]; then
  $CMD SpeechToText
else
  $CMD SpeechToText | tail -1
fi
