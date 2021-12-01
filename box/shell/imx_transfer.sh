set -e
CUR_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
REMOTE_IP=192.168.150.53     #要传送文件的远程ip
REMOTE_PATH=/root/         #要传送文件的远程路径
REMOTE_USER=root             #要传送文件的远程用户
 
if [ $# != 1 ] ; then
    echo "USAGE: $0 file_path"
    echo " e.g.: $0 /your/path/test.data"
    exit 1;
fi
 
if [ ! -e "$1" ]; then
    echo "FILE: $1 dose'nt exist! please check your input"
    exit 0
else
    scp "$1"  $REMOTE_USER@$REMOTE_IP:$REMOTE_PATH
    echo "transfor "$1" to $REMOTE_IP:$REMOTE_PATH done"
fi
