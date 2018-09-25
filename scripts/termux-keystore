#!/data/data/com.termux/files/usr/bin/sh
set -e -u

readonly CMD_BASE="/data/data/com.termux/files/usr/libexec/termux-api Keystore"

SCRIPTNAME=termux-keystore
show_usage () {
    echo "Usage: $SCRIPTNAME command"
    echo "These commands are supported:"
    echo "  list [-d]"
    echo "  delete <alias>"
    echo "  generate <alias> [-a alg] [-s size] [-u validity]"
    echo "  sign <alias> <algorithm>"
    echo "  verify <alias> <algorithm> <signature>"
    echo
    echo "list: List the keys stored inside the keystore."
    echo "  -d           Detailed results (includes key parameters)."
    echo
    echo "delete: Permanently delete a given key from the keystore."
    echo "  alias        Alias of the key to delete."
    echo
    echo "generate: Create a new key inside the hardware keystore."
    echo "  alias        Alias of the key."
    echo "  -a alg       Algorithm to use (either 'RSA' or 'EC'). Defaults to RSA."
    echo "  -s size      Key size to use. For RSA, the options are 2048, 3072"
    echo "               and 4096. For EC, the options are 256, 384 and 521."
    echo "  -u validity  User validity duration in seconds. Omit to disable."
    echo "               When enabled, the key can only be used for the"
    echo "               duration specified after the device unlocks. After"
    echo "               the duration has passed, the user needs to re-lock"
    echo "               and unlock the device again to be able to use this key."
    echo
    echo "sign: Sign using the given key, the data is read from stdin and the"
    echo "signature is output to stdout."
    echo "  alias        Alias of the key to use for signing."
    echo "  algorithm    Algorithm to use, e.g. 'SHA256withRSA'. This should"
    echo "               match the algorithm of the key."
    echo
    echo "verify: Verify a signature. The data (original file) is read from stdin."
    echo "  alias        Alias of the key to use for verify."
    echo "  algorithm    Algorithm that was used to sign this data."
    echo "  signature    Signature file to use in verification."
}


check_args () {
    if [ "$2" != "$3" ]; then
        echo "$SCRIPTNAME: $1 needs exactly $2 arguments"
        exit 1
    fi
}

list_keys () {
    if [ "$#" -gt 0 ] && [ "$1" = "-d" ]; then
        $CMD_BASE -e command list --ez detailed true
    else
        $CMD_BASE -e command list
    fi
}

delete_key () {
    check_args delete 1 $# 
    $CMD_BASE -e command delete -e alias "$1"
}

sign_data () {
    check_args sign 2 $#
    $CMD_BASE -e command sign -e alias "$1" -e algorithm "$2" | base64 -d
}

verify_data () {
    check_args verify 3 $#
    $CMD_BASE -e command verify -e alias "$1" -e algorithm "$2" \
        -e signature "$(realpath "$3")"
}

generate_key () {
    if [ $# -lt 1 ]; then
        echo "$SCRIPTNAME generate: alias argument is required"
        exit 1
    fi
    ALIAS=$1; shift
    ALGORITHM=RSA; SIZE=-1; CURVE=secp256r1; VALIDITY=0
    while getopts a:s:c:u: NAME; do
        case "$NAME" in
            a) ALGORITHM=$OPTARG ;;
            s) SIZE=$OPTARG ;;
            u) VALIDITY=$OPTARG ;;
            ?) ;;
        esac
    done

    if [ "$ALGORITHM" = "RSA" ]; then
        case "$SIZE" in
            -1) SIZE=2048 ;;
            2048|3072|4096) ;;
            *) echo "$SCRIPTNAME: invalid RSA key size $SIZE"; exit 1 ;;
        esac
    elif [ "$ALGORITHM" = "EC" ]; then
        case "$SIZE" in
            -1|256) CURVE=secp256r1 ;;
            384) CURVE=secp384r1 ;;
            521) CURVE=secp521r1 ;;
            *) echo "$SCRIPTNAME: invalid EC key size $SIZE"; exit 1 ;;
        esac
    else
        echo "$SCRIPTNAME: invalid algorithm $ALGORITHM"; exit 1
    fi

    # purpose 12 is SIGN+VERIFY
    $CMD_BASE -e command generate -e alias "$ALIAS" -e algorithm "$ALGORITHM" \
        --ei purposes 12 --esa digests SHA-1,SHA-256,SHA-384,SHA-512 \
        --ei size "$SIZE" -e curve "$CURVE" --ei validity "$VALIDITY"
}

ACTION="${1-}"
if [ "$#" -gt 0 ]; then shift; fi

case "$ACTION" in
    list) list_keys "$@" ;;
    generate) generate_key "$@" ;;
    delete) delete_key "$@" ;;
    sign) sign_data "$@" ;;
    verify) verify_data "$@" ;;
    *) show_usage ;;
esac
