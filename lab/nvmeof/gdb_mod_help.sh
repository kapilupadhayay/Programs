#!/bin/bash 
#MODULES='nvme_rdma ib_uverbs rdma_cm iw_cm ib_cm rdma_rxe nvme_fabrics ib_core configfs ip6_udp_tunnel udp_tunnel nvme_core nvme'
MODULES="drivers/nvme/target/nvme-loop.ko drivers/nvme/target/nvmet.ko drivers/nvme/host/nvme-core.ko drivers/nvme/host/nvme-rdma.ko drivers/nvme/host/nvme.ko drivers/nvme/host/nvme-fabrics.ko drivers/infiniband/core/ib_uverbs.ko drivers/infiniband/core/rdma_cm.ko drivers/infiniband/core/iw_cm.ko drivers/infiniband/core/ib_cm.ko drivers/infiniband/sw/rxe/rdma_rxe.ko drivers/infiniband/core/ib_core.ko "
mod_substr="nvme"
mod_substr_substr="-"
for module in $MODULES;
do
    #get the module directory from .ko path
    mod_bn=`basename $module`  
    mod_bn_rstriped=${mod_bn%".ko"}
    mod="${mod_bn_rstriped/-/_}"
    if test -f "/sys/module/$mod/sections/.data"; then 
	TEXT_ADD=`cat /sys/module/$mod/sections/.text`
	DATA_ADDR=`cat /sys/module/$mod/sections/.data`
	BSS_ADDR=`cat /sys/module/$mod/sections/.bss`
	echo "add-symbol-file $module $TEXT_ADD -s .data $DATA_ADDR  -s .bss $BSS_ADDR"
    fi
done
