#include "net.exceptions.h"
#include "net.net_address.h"
#include "net.net4_address.h"
#include "net.net6_address.h"

net::net_address::net_address(const int& family, const std::string& hostname)
	: family_(family)
	, hostname_(hostname)
	, hostaddr_("")
{
}

net::net_address::~net_address(void)
{
}

std::shared_ptr<net::net_address>
net::net_address::parse_numeric_address(const std::string& address)
{
	if (address.empty())
		return net6_address::LOOPBACK;
	std::shared_ptr<net::net_address> result = get_numeric_address(address);
	if (disallow_deprecated_formats(address, result))
		throw std::invalid_argument("Not a numeric address: " + address);
	return result;
}

const std::string& net::net_address::init_host_name(void)
{
	if (hostname_.empty()) {
		try {
			hostname_ = get_name_info(NI_NAMEREQD);
		}
		catch (const unknown_host_exception&) {
			hostname_ = get_name_info(NI_NUMERICHOST);
		}
	}
	return hostname_;
}

const std::string& net::net_address::init_host_address(void)
{
	if (hostaddr_.empty())
		hostaddr_ = get_name_info(NI_NUMERICHOST);
	return hostaddr_;
}

std::shared_ptr<net::net_address> net::net_address::get_by_address(
	const std::vector<std::uint8_t>& address,
	const std::string& hostname)
{
	if (address.size() == 4)
		return std::make_shared<net4_address>(address, hostname);
	if (address.size() == 16) {
		if (!is_ipv4mapped_address(address))
			return std::make_shared<net6_address>(address, hostname);
		// return as an ipv4 address
		return std::make_shared<net4_address>(
			ipv4mapped_to_ipv4(address), hostname);
	}
	throw std::length_error("Incorrect size: " + address.size());
}

bool net::net_address::is_ipv4mapped_address(
	const std::vector<std::uint8_t>& address)
{
	if (address.size() != 16)
		return false;
	for (int i = 0; i < 10; i++)
		if (address[i] != 0)
			return false;
	if (address[10] != 255 || address[11] != 255)
		return false;
	return true;
}

bool net::net_address::is_ipv4mapped_address(const std::uint8_t* address)
{
	for (int i = 0; i < 10; i++)
		if (address[i] != 0)
			return false;
	if (address[10] != 255 || address[11] != 255)
		return false;
	return true;
}

std::vector<std::shared_ptr<net::net_address>>
net::net_address::get_all_by_name(const std::string& hostname)
{
	if (hostname.empty())
		return loopback_addresses();

	// Is it a numeric address?
	std::shared_ptr<net::net_address> result = get_numeric_address(hostname);
	if (result != nullptr) {
		if (disallow_deprecated_formats(hostname, result))
			throw unknown_host_exception(hostname);
		return{ result };
	}

	return lookup_host_by_name(hostname);
}

std::shared_ptr<net::net_address>
net::net_address::get_numeric_address(const std::string& address)
{
	sio::addrinfo hints;
	hints.ai_flags = AI_NUMERICHOST;
	sio::addrinfo_t* result = nullptr;

	try {
		sio::get_addrinfo(address.c_str(), nullptr, hints, &result);

		std::vector<std::shared_ptr<net_address>> addresses =
			result_to_net_adresses(result);

		sio::free_addrinfo(result);

		return addresses.size() != 0 ? addresses[0] : nullptr;
	}
	catch (const sio::errno_exception&) {
		return nullptr;
	}
}

bool net::net_address::disallow_deprecated_formats(
	const std::string& hostname, const std::shared_ptr<net::net_address>& addr)
{
	if (addr->get_family() != AF_INET)
		return false;
	if (hostname.find(':') != std::string::npos)
		return false;
	try {
		std::uint32_t notused;
		sio::inet_pton(AF_INET, hostname.c_str(), &notused);
		return false;
	}
	catch (const sio::errno_exception&) {
		return true;
	}
}

std::vector<std::shared_ptr<net::net_address>>
net::net_address::lookup_host_by_name(const std::string& hostname)
{
	sio::addrinfo hints;
	hints.ai_flags = AI_ADDRCONFIG;
	sio::addrinfo_t* result = nullptr;

	try {
		sio::get_addrinfo(hostname.c_str(), nullptr, hints, &result);

		std::vector<std::shared_ptr<net_address>> addresses =
			result_to_net_adresses(result);

		sio::free_addrinfo(result);

		return addresses;
	}
	catch (const sio::errno_exception&) {
		throw unknown_host_exception(hostname);
	}
}

std::vector<std::shared_ptr<net::net_address>>
net::net_address::result_to_net_adresses(const sio::addrinfo_t* result)
{
	std::vector<std::shared_ptr<net_address>> addresses;
	for (const sio::addrinfo_t* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
		if (ptr->ai_family == AF_INET) {
			sio::sock_addr4 addr4(ptr->ai_addr);
			addresses.emplace_back(std::make_shared<net4_address>(
				addr4.get_addr(), ""));
		}
		else if (ptr->ai_family == AF_INET6) {
			sio::sock_addr6 addr6(ptr->ai_addr);
			const std::uint8_t* addr = addr6.get_addr();
			if (is_ipv4mapped_address(addr))
				addresses.emplace_back(std::make_shared<net4_address>(
					std::vector<uint8_t>(&addr[12], &addr[16]), ""));
			else
				addresses.emplace_back(std::make_shared<net6_address>(
					std::vector<uint8_t>(&addr[0], &addr[16]), ""));
		}
	}
	return addresses;
}

std::shared_ptr<net::net_address> net::net_address::find_best_match(
	const std::vector<std::shared_ptr<net::net_address>>& addrs, const int& family)
{
	bool match = true;
	for (int n = 0; n < 2; ++n) {
		for (std::size_t i = 0; i < addrs.size(); ++i) {
			std::shared_ptr<net_address> addr = addrs[i];
			if ((addr->get_family() == family) == match)
				return addr;
		}
		match = false;
	}
	throw unknown_host_exception("Host not found");
}

std::vector<std::shared_ptr<net::net_address>>
net::net_address::loopback_addresses(void)
{
	return{ net6_address::LOOPBACK, net4_address::LOOPBACK };
}

#if !defined(__NET_INLINE__)
#include "net.net_address.inl"
#endif
