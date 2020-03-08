DIR=/usr/local/sbin

#if [ $(id -u) -ne "0" ]
#then
#    echo "ERROR: Please run as root"
#    exit 1
#fi

sudo install -m 755 -g root -o root -T gwrst.sh $DIR/gwrst
sudo gwrst
make clean
make
