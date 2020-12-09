
ip=222.88.2.136

iptables -A OUTPUT -d $ip -o eth0 -j ACCEPT
iptables -A OUTPUT -o eth0 -j DROP

#delete rules
iptables -D OUTPUT -d $ip -o eth0 -j ACCEPT
iptables -D OUTPUT -o eth0 -j DROP

