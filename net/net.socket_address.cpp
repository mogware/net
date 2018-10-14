#include "net.socket_address.h"

net::socket_address::socket_address(const std::uint16_t& port)
	: addr_(nullptr)
	, hostname_("")
	, port_(port)
{
}

net::socket_address::socket_address(std::shared_ptr<net::net_address> addr,
	const std::uint16_t& port)
	: addr_(nullptr)
	, hostname_("")
	, port_(port)
{
	addr_ = (addr == nullptr) ? net4_address::ANY : addr;
}

net::socket_address::socket_address(const std::string& hostname,
	const std::uint16_t& port, const bool& prefer_ipv6)
	: addr_(nullptr)
	, hostname_(hostname)
	, port_(port)
{
	try {
		addr_ = net_address::of(hostname, prefer_ipv6);
		hostname_.clear();
	}
	catch (const unknown_host_exception&) {
	}
}

net::socket_address::~socket_address(void)
{
}

#if !defined(__NET_INLINE__)
#include "net.socket_address.inl"
#endif