#!/bin/bash 
#network speed 
#by wyyj12@163.com 
#release 2012-11-13 
 
main() 
{ 
while : 
do 
time=`date +%m"-"%d" "%k":"%M":"%S` 
rx_before=`ifconfig $eth_name|grep "RX bytes"|awk '{print $2}'|cut -d ':' -f 2` 
tx_before=`ifconfig $eth_name|grep "TX bytes"|awk '{print $6}'|cut -d ':' -f 2` 
sleep 2 
rx_after=`ifconfig $eth_name|grep "RX bytes"|awk '{print $2}'|cut -d ':' -f 2` 
tx_after=`ifconfig $eth_name|grep "TX bytes"|awk '{print $6}'|cut -d ':' -f 2` 
 
rx_result=$(( (rx_after-rx_before)/256 ))
tx_result=$(( (tx_after-tx_before)/256 ))

echo -e "$time In_Speed: "$(( rx_result/speed ))" $unit Out_Speed: "$((tx_result/speed))" $unit" 

#echo -e "$time In_Speed:`tput rev`"$((rx_result/speed))"`tput sgr0` $unit Out_Speed: `tput rev`"$((tx_result/speed))"`tput sgr0` $unit" #反向色 
 
sleep 2 
done 
} 
 
#default 
eth_name="eth0" 
unit="Kbps" 
speed="1" 
 
while getopts   hmMgGi: opt  
do 
case $opt in  
    i) 
        eth_name=$OPTARG 
    ;; 
    m|M) 
        speed="1024" 
        unit="Mbps" 
#       echo -e  "33[5m Count $eth_name use Mbps 33[0m" 
    ;; 
    g|G) 
        speed="1048576" 
        unit="Gbps" 
#       echo -e  "33[5m Count $eth_name use Gbps 33[0m" 
    ;; 
    h) 
        echo  -e "  Usage:t netspeed {-m|-M|-g|-G} [-i [interface]]" 
                echo  -e "  Default:t Count eth0 use kbps " 
        exit 0; 
    ;; 
    ?) 
        echo "Usage: netspeed {-m|-M|-g|-G} [-i [interface]]" 
        echo "Default : Count eth0 use kbps " 
    ;; 
esac 
done 
 
echo -e  " Count $eth_name speed ! " 
main 
