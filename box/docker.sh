#!/bin/bash
set -e
platform=$2

base_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd)
docker_image="meov01/ubuntu:18.04"
#create_time=`date +%F-%H-%m-%S-%N`
container_name="ubuntu_18_04$create_time"
uid=$UID
if [[ $uid -ne 1016 && $uid -ne 1017 && $uid -ne 1018 && $uid -ne 1019 && $uid -ne 1020 ]]; then
    user="root"
else
    user=$USER
fi

echo "user $user uid $uid"
source_dir="$base_dir/../box"
target_dir="/home/$user/box"

if [ $# != 1 ] ; then
    echo "USAGE: $0 options"
    echo "  e.g. $0 start                  --> to enter docker"
    echo "       $0 build [platform]       --> to biuld project"
    exit 1;
fi

function check_container(){
    docker image ls
    docker ps -a

    num=`docker ps -a|grep ubuntu_18_04|wc -l`
    echo "INFO: found wmpki container: $num"
    if [ $num -eq 0 ];then
        echo "INFO: uds_crt container not running, run it!"
        docker run \
            -v $source_dir:$target_dir \
            --name $container_name \
            --net=host \
            -u $user \
            -dit $docker_image \
            /bin/bash
    else
        Up_num=`docker ps -a|grep ubuntu_18_04|grep Up|wc -l`
        container_id=`docker ps -a|grep ubuntu_18_04|awk -F' ' '{print $1}'`
        echo "INFO: Up_num=$Up_num"
        echo "INFO: container_id=$container_id"
        if [ $Up_num -eq 0 ];then
            echo "INFO: uds_container is running but need restart container_id=$container_id"
            docker restart $container_id
        fi
    fi
}

function docker_build(){
    local build_cmd="cd $target_dir && ./build.sh $platform"
    echo "build_cmd $build_cmd"
    if [ ! -d /opt/$cross_tools_chains ]; then
        echo "found $cross_tools_chains in /opt copying it to /opt"
        docker cp /opt/$cross_tools_chains ubuntu_18_04:/opt -r
    fi 
    
    docker exec \
        -it \
        $container_name \
        bash -c "cd $target_dir && $build_cmd"
}

function docker_entering(){
    docker exec \
        -it \
        $container_name \
        bash -c "cd $target_dir && bash"
}


function docker_del {
    num=`docker ps | grep -w ${container_name} | wc -l`
    num2=`docker ps -a | grep -w ${container_name} | wc -l`
    if [ $num -eq 1 ];then
        docker stop ${container_name}
        docker rm ${container_name}
    elif [ $num -eq 0 ];then
        if [ $num2 -eq 1 ]; then
            docker rm ${container_name}
        fi
        if [ $num2 -gt 1 ]; then
            docker ps -a | grep -w ${container_name}
            echo "please delete unused docker container"
        fi
    else
        docker ps -a | grep -w ${container_name}
        echo "please delete unused docker container"
    fi
}

case $1 in
start)
    check_container
    docker_entering
    ;;
build)
    shift
    check_container
    docker_build
#    docker_del
    ;;
esac
