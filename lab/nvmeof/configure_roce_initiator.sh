#!/bin/bash

cd /root/drivers

insmod	nvme-core.ko
insmod  udp_tunnel.ko
insmod  ip6_udp_tunnel.ko
insmod  configfs.ko
insmod	ib_core.ko 
insmod	nvme-fabrics.ko

# while moving up from 4.8 to 4.13
# this dependancy creeped in. Required by rdma_rxe
insmod crc32_generic.ko

insmod	rdma_rxe.ko
insmod  ib_cm.ko
insmod	iw_cm.ko
insmod	rdma_cm.ko
insmod	ib_uverbs.ko
#initiator specific
insmod	nvme.ko
insmod	nvme-rdma.ko

echo "insmoded all the Drivers for nmve-rdma and ib-rdam for INITIATOR "

#echo "proceeding to configure name space and network infra"
#mkdir /sys/kernel/config/nvmet/subsystems/nvme-subsystem-name
#cd /sys/kernel/config/nvmet/subsystems/nvme-subsystem-name
#echo 1 > attr_allow_any_host
#mkdir namespaces/10
#cd namespaces/10
#echo -n /dev/nvme0n1> device_path
#echo 1 > enable
#mkdir /sys/kernel/config/nvmet/ports/1
#cd /sys/kernel/config/nvmet/ports/1

echo "Adding NIC ens3 to do rdma"
/root/rdma-core/providers/rxe/rxe_cfg add ens3
/root/rdma-core/providers/rxe/rxe_cfg status


echo "Executing: ibv_rc_pingpong -d rxe0 -g 0 192.168.35.129"
ibv_rc_pingpong -d rxe0 -g 0 192.168.35.129

