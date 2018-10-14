#include <memory>
#include <sstream>
#include <iomanip> 

#include "net.exceptions.h"
#include "net.net6_address.h"

//TODO scope_id

std::shared_ptr<net::net_address> net::net6_address::ANY =
	net::net6_address::of({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
std::shared_ptr<net::net_address> net::net6_address::LOOPBACK =
	net::net6_address::of({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, "localhost");

net::net6_address::net6_address(const std::uint8_t* address, const int& length,
		const std::string& hostname)
	: net_address(AF_INET6, hostname)
{
	bool valid = length == 16;
	for (int i = 0; i < 16; i++)
		address_[i] = valid ? address[i] : 0;
}

net::net6_address::net6_address(const std::vector<std::uint8_t>& address,
		const std::string& hostname)
	: net_address(AF_INET6, hostname)
{
	bool valid = address.size() == 16;
	for (int i = 0; i < 16; i++)
		address_[i] = valid ? address[i] : 0;
}

net::net6_address::~net6_address(void)
{
}

std::shared_ptr<net::net_address> net::net6_address::of(
	const std::vector<std::uint8_t>& address, const std::string& hostname)
{
	return 	std::make_shared<net6_address>(address, hostname);
}

std::string net::net6_address::get_name_info(const int& flags) const
{
	sio::sock_addr6 sa(address_);
	char hostname[1025];

	try {
		sio::get_nameinfo(sa, sizeof(sa), hostname, 1025, nullptr, 0, flags);
		return hostname;
	}
	catch (const sio::errno_exception&) {
		throw unknown_host_exception(to_string());
	}
}

std::string net::net6_address::to_string(void) const
{
	std::stringstream stream;
	for (std::size_t i = 0; i < 16; ++i) {
		if (i != 0 && i % 2 == 0)
			stream << ":";
		stream << std::setfill('0') << std::setw(2)
			<< std::hex << (address_[i] & 0xFF);
	}
	return stream.str();
}

#if !defined(__NET_INLINE__)
#include "net.net6_address.inl"
#endif
