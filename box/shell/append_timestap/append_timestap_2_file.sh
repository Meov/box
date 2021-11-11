SIGNED_TIME=$(date "+%Y:%m:%d:%H:%M:%S")
sed -i '$s/$/'${SIGNED_TIME}'/' $1
