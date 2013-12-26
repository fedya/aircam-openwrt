#ifndef __IP_SET_IPTREEMAP_H
#define __IP_SET_IPTREEMAP_H

#include <linux/netfilter_ipv4/ip_set.h>

#define SETTYPE_NAME "iptreemap"


struct ip_set_iptreemap {
	unsigned int gc_interval;
};

struct ip_set_req_iptreemap_create {
	unsigned int gc_interval;
};

struct ip_set_req_iptreemap {
	ip_set_ip_t start;
	ip_set_ip_t end;
};

#endif /* __IP_SET_IPTREEMAP_H */
