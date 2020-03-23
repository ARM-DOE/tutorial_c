#!/bin/sh

# Print usage

script_name=$0

usage()
{
    cat <<EOM

DESCRIPTION

  Build script to configure, make, and install this package.

SYNOPSIS

  $script_name [--prefix=path [--destdir=path]] [--clean] [--conf]

OPTIONS

  --prefix=path   absolute path to installation directory
                  default: \$VAP_HOME

  --destdir=path  absolute path prepended to prefix 
                  used to perform a staged installation

  --clean         cleanup build directory and regenerate
                  build system files

  --conf          only install conf files

  --strip         strip debugging symbols

  --purify        build with purify

  -h, --help      display this help message

EOM
}

# Parse command line 

install="install"

for i in "$@"
do
    case $i in
        --clean)      clean=1
                      ;;
        --conf)       install="install-data"
                      ;;
        --destdir=*)  destdir="${i#*=}"
                      ;;
        --prefix=*)   prefix="${i#*=}"
                      ;;
        --purify)     purify="true"
                      ;;
        --strip)      install="install-strip"
                      ;;
        -h | --help)  usage
                      exit
                      ;;
        *)            usage
                      exit 1
                      ;;
    esac
done

if [ $destdir ] && [ ! $prefix ]; then
   usage
   exit 1
fi

# Get prefix from environemnt variables if necessary

if [ ! $prefix ]; then
    if [ $VAP_HOME ]; then
        prefix=$VAP_HOME
    else
        usage
        exit 1
    fi
fi

# Function to echo and run commands

run() {
    echo "> $1"
    $1 || exit 1
}

# autogen.sh

if [ $clean ] || [ ! -f ./configure ]; then

    echo "------------------------------------------------------------------"
    echo "prefix:  $prefix"

    if [ $destdir ]; then
        echo "destdir: $destdir"
    fi

    echo "------------------------------------------------------------------"
    run "./autogen.sh"
fi

# mkdir build; cd build

echo "------------------------------------------------------------------"
if [ $clean ] || [ ! -d ./build ]; then

    if [ -d build ]; then
        run "rm -rf build"
    fi

    run "mkdir -p build"
fi

run "cd build"

# configure

if [ $clean ] || [ ! -f ./Makefile ]; then
    echo "------------------------------------------------------------------"
    run "../configure --prefix=$prefix"
fi

# make install

if [ $purify ]; then
    export CC="purify -cache-dir=$HOME/.purecache -windows=no gcc"
    install="-e $install"
    echo "------------------------------------------------------------------"
    run "make clean"
fi

echo "------------------------------------------------------------------"
if [ $destdir ]; then
    run "make DESTDIR=$destdir $install"
else
    run "make $install"
fi

exit
