#include "net.exceptions.h"
#include "net.net4_address.h"

std::shared_ptr<net::net_address> net::net4_address::ANY =
		net::net4_address::of({ 0, 0, 0, 0 });
std::shared_ptr<net::net_address> net::net4_address::ALL =
		net::net4_address::of({ 255, 255, 255, 255 });
std::shared_ptr<net::net_address> net::net4_address::LOOPBACK =
		net::net4_address::of({ 127, 0, 0, 1 }, "localhost");

net::net4_address::net4_address(const std::uint32_t& address,
	const std::string& hostname)
	: net_address(AF_INET, hostname)
	, address_(address)
{
}

net::net4_address::net4_address(const std::vector<std::uint8_t>& address,
	const std::string& hostname)
	: net_address(AF_INET, hostname)
	, address_(0)
{
	if (address.size() == 4) {
		address_ = address[3] & 0xFF;
		address_ |= ((address[2] << 8) & 0xFF00);
		address_ |= ((address[1] << 16) & 0xFF0000);
		address_ |= ((address[0] << 24) & 0xFF000000);
	}
}

net::net4_address::~net4_address(void)
{
}

std::shared_ptr<net::net_address> net::net4_address::of(
	const std::vector<std::uint8_t>& address, const std::string& hostname)
{
	return std::make_shared<net4_address>(address, hostname);
}


std::string net::net4_address::get_name_info(const int& flags) const
{
	sio::sock_addr4 sa(address_);
	char hostname[1025];

	try {
		sio::get_nameinfo(sa, sizeof(sa), hostname, 1025, nullptr, 0, flags);
		return hostname;
	}
	catch (const sio::errno_exception&) {
		throw unknown_host_exception(to_string());
	}
}

#if !defined(__NET_INLINE__)
#include "net.net4_address.inl"
#endif
