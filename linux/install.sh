#!/bin/sh
if [ "$(id -u)" -ne 0 ]; then
    echo 'Required to be run as root (sudo).'
    exit 1
fi
cd $(dirname "$0")

printUsage() {
    echo "Usage: install.sh RULE... [OPTION]...

    RULES:
    1, enter-ctrl        enables enter to right-ctrl on hold
    2, caps-esc-ctrl     enables capslock to esc on tap, left-ctrl on hold
    3, ctrl-caps         enables left-ctrl to capslock on tap
    4, shift-parentheses enables left shift and right shift to parenthesis on tap
    5, deadkey-german    enables extra iso key to german deadkey
    std                  same as 1234
    all                  same as 12345

    OPTIONS:
    --no-x11                install without x11 bindings (default if -5 or --deadkey-german is omitted)
    -h, --help              show usage";
}

RULE1=false
RULE2=false
RULE3=false
RULE4=false
RULE5=false
NOX11=false
# https://stackoverflow.com/a/28466267
needs_arg() { if [ -z "$OPTARG" ]; then echo "No arg for --$OPT option"; printUsage; exit 2; fi; }

echo $@
while true; do
    case "$1" in
        r | rules )     echo $OPT; echo $OPTARG; needs_arg; RULES="$OPTARG" ;;
        no-x11 )        NOX11=true ;;
        h | help )      printUsage; exit 0 ;;
        ??* )           echo "Illegal option --$OPT"; printUsage; exit 2 ;;
        \? )            printUsage; exit 2 ;;
    esac
done

echo $@
echo $RULES

exit 0;

# make and install plugin
TARGET=dualkey-anti-rsi
if  [ "$NOX11" = true ]; then
    TARGET=dualkey-anti-rsi_nox11
fi

( cd ../interception-plugin/ && mkdir -p build )
( cd ../interception-plugin/build && cmake .. && make $TARGET )
if ! [ -e ../interception-plugin/build/$TARGET ]; then
    echo 'Install failed due to build error.'
    exit 1
fi

cp -f ../interception-plugin/build/$TARGET /usr/local/bin/dualkey-anti-rsi
if ! [ -e /usr/local/bin/dualkey-anti-rsi ]; then
    echo 'Install dualkey-anti-rsi failed.'
    exit 1
fi

# install udevmon config and service
cp -f udevmon.yaml /etc/udevmon.yaml
cp -f udevmon.service /etc/systemd/system/udevmon.service
if ! [ -e /etc/udevmon.yaml ] || ! [ -e /etc/systemd/system/udevmon.service ]; then
    echo 'Install udevmon failed.'
    exit 1
fi
systemctl enable --now udevmon

echo 'Install successful.'
exit 0;
