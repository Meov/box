if [ $# != 1 ] ; then 
echo "USAGE: $0 APPNAME" 
echo " e.g.: $0 rsa_test" 
exit 1; 
fi

#using openssl to create public and private keys!
#openssl genrsa -out rsa_key.private
#sleep 1
#openssl rsa -pubout -in rsa_key.private -out rsa_key.public

openssl dgst -sha1 -out $1.sig -sign rsa_key.private $1

openssl dgst -sha1 -verify rsa_key.public -signature $1.sig $1
