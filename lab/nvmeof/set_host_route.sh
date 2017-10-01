#!/bin/sh

netstat  -rn
route del default gw 192.168.207.2
route add default gw 192.168.207.2 dev br0

ping -c 10 www.google.com

