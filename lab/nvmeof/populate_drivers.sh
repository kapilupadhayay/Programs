#!/bin/bash -e
#files="configfs.ko ib_core.ko udp_tunnel.ko ib_cm.ko iw_cm.ko rdma_rxe.ko rdmavt.ko rdma_cm.ko nvme-rdma.ko nvmet-rdma.ko nvme-fabrics.ko nvmet.ko nvme-loop.ko "

initiator_files="configfs.ko  ib_core.ko    ip6_udp_tunnel.ko  nvme-core.ko     nvme.ko       nvme-rdma.ko  nvmet-rdma.ko  rdma_rxe.ko  rdmavt.ko ib_cm.ko     ib_uverbs.ko  iw_cm.ko           nvme-fabrics.ko  nvme-loop.ko  nvmet.ko      rdma_cm.ko     rdma_ucm.ko  udp_tunnel.ko crc32_generic.ko"

target_files="configfs.ko  ib_core.ko    ip6_udp_tunnel.ko  nvme-core.ko     nvme.ko       nvme-rdma.ko  nvmet-rdma.ko  rdma_rxe.ko  rdmavt.ko ib_cm.ko     ib_uverbs.ko  iw_cm.ko           nvme-fabrics.ko  nvme-loop.ko  nvmet.ko      rdma_cm.ko     rdma_ucm.ko  udp_tunnel.ko crc32_generic.ko"

ROOT_DIR="/opt/dev/linux_dev/guest"

LINUX_DIR="$ROOT_DIR/linux-stable"
TMP_MOUNT_DIR_INITIATOR="$ROOT_DIR/backups/rootfs_I"
TMP_MOUNT_DIR_TARGET="$ROOT_DIR/backups/rootfs_T"
ROOTFS_IMG_INITIATOR="$ROOT_DIR/backups/Ubuntu-16.04-rootfs-I.img"
ROOTFS_IMG_TARGET="$ROOT_DIR/backups/Ubuntu-16.04-rootfs-T.img"

#mount -t ext3 -o loop "$ROOTFS_IMG_INITIATOR" "$TMP_MOUNT_DIR_INITIATOR" || { echo -e "\n mount failure initiator \n"; exit 1; }
#mount -t ext3 -o loop "$ROOTFS_IMG_TARGET" "$TMP_MOUNT_DIR_TARGET" || { echo -e "\n mount failure target \n"; exit 1; }



#echo "Removing following modules for Initiator"
#for file in $initiator_files; do find $LINUX_DIR -name $file -print -exec rm {} \;;  done
#echo "Removing following modules for Target"
#for file in $target_files; do find $LINUX_DIR -name $file -print -exec rm {} \;;  done
#cd $LINUX_DIR
#make -j 4 || { echo -e "\n\n UPDATION ABORTED DUE TO ERRORS IN COMPILATION; OLD MODULES NO LONGER EXIST IN ROOTFS"; exit 1; }
echo "copy following files to NVME INITIATOR"
echo "======================================"
for file in $initiator_files;
    do 
    new_file=`find $LINUX_DIR -name $file`
    if [ -e "${new_file}" ]; then
	echo "cp $new_file $TMP_MOUNT_DIR_INITIATOR/root/drivers"
    fi
    #cp $new_file "$TMP_MOUNT_DIR_INITIATOR/root/drivers/"
    done
echo "copy followig files to NVME TARGET"
echo "=================================="
for file in $target_files;
    do 
    new_file=`find $LINUX_DIR -name $file`
    if [ -e "${new_file}" ]; then
	echo "cp $new_file $TMP_MOUNT_DIR_TARGET/root/drivers/"
    #cp $new_file "$TMP_MOUNT_DIR_TARGET/root/drivers/"
    fi 
    done

echo 
echo
echo "mount -t ext3 -o loop $ROOTFS_IMG_INITIATOR $TMP_MOUNT_DIR_INITIATOR"
echo "mount -t ext3 -o loop $ROOTFS_IMG_TARGET $TMP_MOUNT_DIR_TARGET"

echo 
echo
#umount "$TMP_MOUNT_DIR_INITIATOR"
#umount "$TMP_MOUNT_DIR_TARGET"
