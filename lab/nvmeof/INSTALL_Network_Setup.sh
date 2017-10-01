# RUN this script with root previliges

IFACE="vmnet8"

# install brctl
apt-get install bridge-utils uml-utilities

# install qemu-kvm. This will plug the /etc/qemu-if and related scripts
apt-get install qemu-kvm

echo 1 >/sys/module/kvm/parameters/ignore_msrs

# Create a bridge
sudo ifconfig $IFACE 0.0.0.0 up
sudo brctl addbr br0
sudo brctl addif br0 $IFACE
sudo ifconfig br0 up
sudo dhclient br0

# Now we need to make tap0
tunctl -u $(whoami)
# something lik this should be printed
# "Set 'tap0' persistent and owned by uid 1001"

ip link set tap0 master br0
ip link set dev br0 up
ip link set dev tap0 up

# Now we need to make tap1
tunctl -u $(whoami)
# something lik this should be printed
# "Set 'tap1' persistent and owned by uid 1001"

ip link set tap1 master br0
ip link set dev br0 up
ip link set dev tap1 up

echo
echo
echo "Here is your bridge configuration"
brctl show
echo
echo "I have done my job; created the tap0 and tap1, configured them"
echo "Check if the initiator scripts are able to bring up the VMs"
echo "Next step is to Use following scripts:"
echo "qemu_start_initiator.sh	: 	For nvme-of Initiator"
echo "qemu_start_target.sh	:	For nvme-of Target"
echo



