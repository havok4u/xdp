# xdp

Test Case 1
=========================================================================================
```
+-------------+              +-------------+
|             |              |             |
|   Myvm1     |              |   MyVm2     |
|             |              |             |
+------+------+              +------+------+
       | vnet1                      | vnet3
       |                            |
       |                            |
       |                            |
+------+----------------------------+------+
|               Bridge1                    |
+------------------------------------------+```

Insert bpf code on vnet1:
sudo ip link set dev vnet1 xdpgeneric obj tcp_port.o sec tcpdropper

Outcome:
Drop on TCP port 80 worked.

Logs:
     vhost-14658-14673 [020] .... 257740.928266: 0x00000001: DEBUG: Entering tcpdropper
     vhost-14658-14673 [020] .... 257740.928287: 0x00000001: DEBUG: protocol tcp
     vhost-14658-14673 [020] .... 257740.928289: 0x00000001: DEBUG TCP: 80
     vhost-14658-14673 [020] .... 257740.928291: 0x00000001: DEBUG port 80 matched XDP_DROP




Test Case 2
=========================================================================================
```
   Namespace                    Namespace
+-------------+              +-------------+
|             |              |             |
|   myapp1    |              |    myapp2   |
|             |              |             |
+------+------+              +------+------+
       | veth2                      | veth4
       |                            |
       |                            |
       | veth1                      | veth3
+------+----------------------------+------+
|                 Bridge0                  |
+------------------------------------------+
```

2a) Insert bpf code on veth1
sudo ip link set dev veth1 xdpgeneric obj tcp_port.o sec tcpdropper


Outcome:
Drop did not work

Logs:
  iperf3-14425 [015] ..s1 256966.478443: 0x00000001: DEBUG: Entering tcpdropper
  iperf3-14425 [015] ..s1 256966.478443: 0x00000001: DEBUG: protocol tcp
  iperf3-14425 [015] ..s1 256966.478444: 0x00000001: DEBUG TCP: 80
  iperf3-14425 [015] ..s1 256966.478444: 0x00000001: DEBUG port 80 matched XDP_DROP

2b) Insert bpf code on veth2 in the namespace
sudo ip netns exec myapp1 ip link set dev veth2 xdpgeneric obj tcp_port.o sec tcpdropper

Outcome:
Drop did not work

Logs:
  iperf3-14425 [015] ..s1 256966.478443: 0x00000001: DEBUG: Entering tcpdropper
  iperf3-14425 [015] ..s1 256966.478443: 0x00000001: DEBUG: protocol tcp
  iperf3-14425 [015] ..s1 256966.478444: 0x00000001: DEBUG TCP: 80
  iperf3-14425 [015] ..s1 256966.478444: 0x00000001: DEBUG port 80 matched XDP_DROP


