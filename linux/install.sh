#!/bin/sh
if [ "$(id -u)" -ne 0 ]; then
    echo 'Required to be run as root (sudo).'
    exit 1
fi
cd $(dirname "$0")

printUsage() {
    echo "Usage: install.sh [OPTION]...

    OPTIONS:
    -r FILE     rules file to install. Defaults to rules.yaml
    -u          uninstall
    -h          show usage

    Install requires systemd.";
}

RULES_FILE=rules.yaml
UNINSTALL=false

# https://stackoverflow.com/a/28466267
while getopts hur: OPT; do
    case "$OPT" in
        r )     RULES_FILE="$OPTARG" ;;
        u )     UNINSTALL=true ;;
        h )     printUsage; exit 0 ;;
        \? )    printUsage; exit 2 ;;
    esac
done

if ! [ -e $RULES_FILE ]; then
    echo "Install failed: rules file $RULES_FILE does not exist"
    exit 2
fi

if [ "$UNINSTALL" = true ] ; then
    systemctl disable --now dualkey-tools
    if [ -e /etc/systemd/system/dualkey-tools.service ]; then
        rm /etc/systemd/system/dualkey-tools.service
    fi

    if [ -e /usr/local/bin/dualkey-tools ]; then
        rm /usr/local/bin/dualkey-tools
    fi

    echo 'Uninstall successful.'
else 
    ( cd ../src/ && mkdir -p build )
    ( cd ../src/build && cmake .. && make )
    if ! [ -e ../src/build/dualkey-tools ]; then
        echo 'Install failed due to build error.'
        exit 1
    fi

    cp -f ../src/build/dualkey-tools /usr/local/bin/dualkey-tools
    if ! [ -e /usr/local/bin/dualkey-tools ]; then
        echo 'Install dualkey-tools failed.'
        exit 1
    fi

    # install as service
    echo 'Installing service...'
    mkdir -p /etc/dualkey-tools
    cp -f rules.yaml /etc/dualkey-tools/rules.yaml
    cp -f dualkey-tools.service /etc/systemd/system/dualkey-tools.service
    if ! [ -e /etc/dualkey-tools/rules.yaml ] || ! [ -e /etc/systemd/system/dualkey-tools.service ]; then
        echo 'Install of service failed.'
        exit 1
    fi

    systemctl enable --now dualkey-tools
    sleep 5
    systemctl restart --now dualkey-tools

    echo 'Install successful.'
fi
exit 0;
