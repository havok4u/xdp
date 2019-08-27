#include "bpf.h"
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#define SEC(NAME) __attribute__((section(NAME), used))

#include "bpf_endian.h"
#include "bpf_helpers.h"

SEC("tcpdropper")
int dropper(struct xdp_md *ctx) {
    int nh_off = 0;
    bpf_printk("DEBUG: Entering tcpdropper\n");

    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;

    struct ethhdr *eth = data;

    nh_off = sizeof(*eth);
    if (data + nh_off > data_end) {
        bpf_printk("DEBUG: XDP_PASS on first if %d\n",100);
        return XDP_PASS;
    }

    struct iphdr *iph = data + nh_off;
    nh_off += sizeof(struct iphdr);
    if (data + nh_off > data_end) {
        return XDP_PASS;
    }
    if (iph->protocol == IPPROTO_UDP) {
        bpf_printk("DEBUG: protocol udp\n");
    }
    if (iph->protocol == IPPROTO_TCP) {
        bpf_printk("DEBUG: protocol tcp\n");
        struct tcphdr *tcph = data + nh_off;
        nh_off += sizeof(struct tcphdr);
        if (data + nh_off > data_end) {
            return XDP_PASS;
        }
        bpf_printk("DEBUG TCP: %d\n", bpf_htons(tcph->dest));
        if (tcph->dest == bpf_ntohs(80)) {
            bpf_printk("DEBUG port 80 matched XDP_DROP\n");
            return XDP_DROP;
        }
    }
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
