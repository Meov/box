#/bin/bash
set -e
platform=$1

base_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)
target_tools_chains=$base_dir/build_tools/build_chains
cross_tools_chains=gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu

function download_dependency() {
    local depend_name=$1
    echo "depend_name: $depend_name"
    if [ $depend_name = $cross_tools_chains ]; then
        if [ ! -d /opt/$cross_tools_chains ]; then
            echo "found $cross_tools_chains in /opt"
            ln -sf /opt/$cross_tools_chains $target_tools_chains
        else
            wget http://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/aarch64-linux-gnu/$depend_name.tar.xz \
                -O $target_tools_chains/$depend_name.tar.xz
            tar zxf $target_tools_chains/$depend_name.tar.xz -C $target_tools_chains 
        fi
    fi
}

function check_dependency() {
    local dependencies=$1
    echo "dependencies: $dependencies"
    for name in "${dependencies[@]}"; do
        if [ ! -d $target_tools_chains/${name} ]; then
            echo ${name} not found
            download_dependency ${name}
        else
            echo ${name} found
        fi
    done
}

function setup(){
    if [ ! -n "$platform" ]; then
        echo "command should like this"
        echo "./build.sh <platform>"
        echo "platform include : <linux-x86_64> <linux-aarch64>"
        exit 1
    fi 
    
    if [ $platform == "linux-x86_64" ]; then
        echo "x86_64 is going to be compiled"
    elif [ $platform == "linux-aarch64" ]; then
        mkdir -p $target_tools_chains
        check_dependency $cross_tools_chains 
    else 
        echo "plat $platform: not recognized"
        echo "platform include : <linux-x86_64> <linux-aarch64>"
        exit 1
    fi
}

function build(){
    local target=$1
    echo "target: $target"
    make $target
}

setup
build app
