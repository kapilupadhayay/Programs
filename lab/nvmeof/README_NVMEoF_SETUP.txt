This doc describes the procedure to make a NVMEoF setup using two VMs:
    a.  Initiator : a qemu based bridged VM
    b.  Target    : same as a.

Before we proceed we need to have follwing on the host system:
    1.  A linux system. I did all this on my Ubuntu 14.04
    2.  Root privileges
    3.  Internet Connection. 
    4.  Atleast 1GB of extra RAM to provide 512MB / VM. Though 256MB / VM
        also works.
    5.  (optional) vmware vmplayer bundle. I used VMware-Player-12.5.1-4542065.x86_64.bundle, this
        can be freely downloaded from vmware site. We dont intend to use vmplayer, but only
        the vmnet8 part of it to support our networking hassle free.

Please follow the following setup instructions:
 
1.  Download qemu that can emulate nvme device and compile it.
        https://github.com/OpenChannelSSD/qemu-nvme
    
    compile qeme using following instructions:
        http://openchannelssd.readthedocs.io/en/latest/gettingstarted/

    Compilation needs certains libraries like pixman, depending upon
    your OS, please search and install the dev versions of the libraries.

2.  Get the linux kernel sources 

        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git

    I have done on 4.13, you need your sources to be > 4.8 at least.

    check out:
        https://github.com/kapilupadhayay/Programs.git
        
        and copy the Programs/lab/nvmeof/config_qemu_nvme file and save it as 
        .config under top directory of linux source downloaded just above.

3.  cd to parent directory of linux sources downloaded in setp 3 and issue 
        %   make -j4
    This will compile the kernel, the nvme driver modules, nvmeof modules and 
    rdma for nvmeof modules


4. After 3 success fully completes you have a qemu binary which can emulate
    nvme, the kernel and the kernel driver modules to make a nvmeof VM setup.


5.  Now you need to make a root file system that you will provide to qemu and
    boot your kernel with. Please install debootstrap for ubuntu.
    To create a root file system please create a 4GB file using following command:
    (Your rootfs will be 4GB then, you wish to make your root file system of a 
        different size provide a different value to count)

    %   dd if=/dev/zero of=Ubuntu-16.04-rootfs-I.img bs=1024 count=4194304
    %   dd if=/dev/zero of=Ubuntu-16.04-rootfs-T.img bs=1024 count=4194304

    Note: i have create 2 different root files systems one for initiator and
    other for target. But thats not required strictly, you may choose to have 
    only one.

5.i Create a empty file to act as a nvme storage device. I specify 4GB file:

    %   dd if=/dev/zero of=./emudevs/blknvme.img bs=1024 count=4194304


6.   Now create a file system on this file. use following command:
       % mkfs.ext4 ./Ubuntu-16.04-rootfs-I.img

7.  Mount the newly created file system at any convenient location. 
    I just created a directory named rootfs and mounted it there:

    % mount -t ext4 -o loop  ./rootfs_xenial_I.img ./rootfs

8.i Now your rootfs is ready to be populated. For ubuntu, I used debootstrap command and
    created ubuntu's Xenial (i.e 16.04) root filesystem like this:

    % debootstrap --arch amd64 xenial ./rootfs http://archive.ubuntu.com/ubuntu

    Sit back and relax or grab a coffee, this will take some time (15-20 min in case 
    of a good internet connection)
    
8.ii Copy the modules to the respective <rootfs>. The modules are mostly common to
      initiator and target but couple of modules are target specific starting with
      nvmet*. You need to make a drectory <rootfs>/root/drivers in both initiator and target
      root filesystems.
      For initiator/Target you need to copy following drivers here. I copied all the files to
       both.
        configfs.ko
        ib_core.ko    
        ip6_udp_tunnel.ko  
        nvme-rdma.ko  
        nvmet-rdma.ko  
        rdma_rxe.ko  
        rdmavt.ko ib_cm.ko     
        ib_uverbs.ko  
        iw_cm.ko           
        nvme-fabrics.ko  
        nvme-loop.ko  
        nvmet.ko      
        rdma_cm.ko     
        rdma_ucm.ko  
        udp_tunnel.ko 
        crc32_generic.ko 

    Note1: probably some modules like nvme-loop.ko are redundant. And the main code i.e nvme.ko and nvme-core.ko
    is statically built into the kernel image. Its mandatory for both Initiator and Target.
    
    Note2: There is a script called "populate_drivers.sh", that can mount the file systems locate the drives 
            copy them over to the respective root file systems. In case you can modify as per your needs, 
            just go ahead and make the changes. It would make your like bit easy for modify-compile-test cycles.
            I just execute the script in a dry-run fashion, it spits all the commands on the shell and then I manually
            do them. I like simple but not easy :)

8.iii Now copy the files:
        configure_roce_target.sh    to <Target rootfs>/root
        configure_roce_initiator.sh to <Inititor rootfs>/root

    These scripts will load the kernel modules in the VMs after they are up.
            
8.iv Finally set the password. 
     cd to top of the mounted rootfs directory
        % cd rootfs
        % chroot .
        % passwd
       follow insteructions.
        

9.  Once 8. successfully completes, unmount rootfs. Your initiator rootfs is ready.

10. For creating target root file system either use same root file system as created
    in 9 or repeat the steps from 6 to 9.

11. Before you can start booting your VMs, you need to configure your host to enable
    the VMs to come up as Bridged VMs. For this you either create an interface that
    runs a dhcp server and include this interface in the bridge. If you know how to
    do it, then do it now. Otherwise just install vmware vmplayer bundle. What ever is
    the case lets call this interface as IFACE

    Our aim is to create tow tun/tap devices i.e 
    tap0    :   for Target
    tap1    :   for initiator
    IFACE   :   provides ip adress to the two VMs when they come up. Also applies the
                forwarding rules for host-guest packets. 
    
    Note: I doubt simply providing a dhcp server to an arbitrary interface (IFACE) and including
          in the bridge will enable the VMs to connect to internet.

    Advice: just install the vmplayer bundle. To install, 

        % sudo ./VMware-Player-12.5.1-4542065.x86_64.bundle

    After installation, ifconfig -a will list an interface names vmnet8

12. Checkout and open the script INSTALL_Network_Setup.sh and set this IFACE name.
    Put the name of the interface where dhcp is running. In case you installed
    vmplayer, the IFACE name is already initialised to vmnet8.
    Execute with root priveleges.

    This will install following packages to your system:
    1.  bridge-utils 
    2.  uml-utilities
    3.  qemu-kvm
    
    then it will configure the network interfaces by creating a bridge br0 and
    putting vmnet8, tap0 and tap1 under it.

    execute ifconfig -a to verify the inteface list. 

13. Enough of hardwork, time to boot up the VMs.
    
        Boot Target:
                % ./qemu_start_target.sh
        
        Boot Initiator:
                % ./qemu_start_initiator.sh
    
    Note1: the above steps need ./qemu-nvme/x86_64-softmmu/qemu-system-x86_64 binary,
    this was compiled in step 1. Make sure the path is correct, otherwise modify it.

    Note 2: There was a file blknvme.img that was created in step 5.1. Before starting
    target VM, open qemu_start_target.sh and make sure the path is correct.


14. If its your day today, you should have both the VMs up and running and their respective
    networks properly configured. Make sure they can reach eachother.

15 Next steps:
    1.  configure RDMA, select the interface and rdam-core
    2.  insmod the modules
    3.  configure nvme target






















    
