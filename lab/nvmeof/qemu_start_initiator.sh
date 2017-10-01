#!/bin/bash
# should be run from /opt/mydev/guest
#./qemu-nvme/x86_64-softmmu/qemu-system-x86_64 -nographic -no-reboot -m 512M -smp cpus=2 -no-kvm -kernel linux-stable/arch/x86/boot/bzImage -drive file=./backups/Ubuntu-16.04-rootfs-I.img,if=ide -netdev tap,id=hostnet0,ifname=tap1 -device virtio-net-pci,netdev=hostnet0,id=net0,mac=52:54:00:61:d2:aa,bus=pci.0 -append "console=ttyS0 root=/dev/sda panic=1 earlyprintk=serial,ttyS0,115200" -s

./qemu-nvme/x86_64-softmmu/qemu-system-x86_64 -nographic -no-reboot -m 512M -smp cpus=2 --enable-kvm -kernel linux-stable/arch/x86/boot/bzImage -drive file=./backups/Ubuntu-16.04-rootfs-I.img,if=ide -netdev tap,id=hostnet0,ifname=tap1 -device virtio-net-pci,netdev=hostnet0,id=net0,mac=52:54:00:61:d2:aa,bus=pci.0 -append "console=ttyS0 root=/dev/sda panic=1 earlyprintk=serial,ttyS0,115200" -gdb tcp::1234
