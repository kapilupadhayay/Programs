#!/bin/bash
# should be run from /opt/mydev/guest

#./qemu-nvme/x86_64-softmmu/qemu-system-x86_64 -nographic -no-reboot -m 512M -smp cpus=2 -kernel linux-stable/arch/x86/boot/bzImage -drive file=./backups/Ubuntu-16.04-rootfs-T.img,if=ide -netdev tap,id=hostnet0,ifname=tap0 -device virtio-net-pci,netdev=hostnet0,id=net0,mac=52:54:00:61:d2:ab,bus=pci.0 -drive file=./emudevs/blknvme.img,if=none,id=mynvme -device nvme,drive=mynvme,serial=deadbeef,namespaces=1,lver=1,nlbaf=5,lba_index=3,mdts=10 -append "console=ttyS0 root=/dev/sda panic=1 earlyprintk=serial,ttyS0,115200" -gdb tcp::1233

./qemu-nvme/x86_64-softmmu/qemu-system-x86_64 --enable-kvm -nographic -no-reboot -m 512M -smp cpus=2 -kernel linux-stable/arch/x86/boot/bzImage -drive file=./backups/Ubuntu-16.04-rootfs-T.img,if=ide -netdev tap,id=hostnet0,ifname=tap0 -device virtio-net-pci,netdev=hostnet0,id=net0,mac=52:54:00:61:d2:ab,bus=pci.0 -drive file=./emudevs/blknvme.img,if=none,id=mynvme -device nvme,drive=mynvme,serial=deadbeef,namespaces=1,lver=1,nlbaf=5,lba_index=3,mdts=10 -append "console=ttyS0 root=/dev/sda panic=1 earlyprintk=serial,ttyS0,115200" -gdb tcp::1233
