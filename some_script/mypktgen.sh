#! /bin/sh
 
# FileName: pktgen-eth5-eth6.conf
# modprobe pktgen
 
pgset() {
    local result
 
    echo $1 > $PGDEV
 
    result=`cat $PGDEV | fgrep "Result: OK:"`
    if [ "$result" = "" ]; then
         cat $PGDEV | fgrep Result:
    fi
}
 
pg() {
    echo inject > $PGDEV
    cat $PGDEV
}
 
# Config Start Here -----------------------------------------------------------
echo "config start."
#sudo modprobe pktgen

#each cpu core has one thread 0~N.
PGDEV=/proc/net/pktgen/kpktgend_0

#remove all device
pgset "rem_device_all"

pgset "add_device eth0"

PGDEV=/proc/net/pktgen/eth0
#device config
pgset "pkt_size 1500"

#0 means forever
pgset "count 1000000"

pgset "delay 0"

pgset "dst 222.88.2.136"

pgset "dst_mac B4:82:C5:00:0C:4A"

#start to sending 
echo "Runing..."
PGDEV=/proc/net/pktgen/pgctrl
pgset "start"

echo "Done..."

echo "Result..."
cat /proc/net/pktgen/eth0
