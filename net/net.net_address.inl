NET_INLINE std::shared_ptr<net::net_address> net::net_address::of(
	const std::string& hostname, const bool& prefer_ipv6)
{
	return find_best_match(get_all_by_name(hostname),
		prefer_ipv6 ? AF_INET6 : AF_INET);
}

NET_INLINE std::shared_ptr<net::net_address> net::net_address::get_local_host(
	const bool& prefer_ipv6)
{
	return find_best_match(lookup_host_by_name(sio::uname_nodename()),
		prefer_ipv6 ? AF_INET6 : AF_INET);
}

NET_INLINE const std::string& net::net_address::get_host_name(void) const
{
	return const_cast<net_address*>(this)->init_host_name();
}

NET_INLINE const std::string& net::net_address::get_host_address(void) const
{
	return const_cast<net_address*>(this)->init_host_address();
}

NET_INLINE bool net::net_address::is_numeric(const std::string& address)
{
	std::shared_ptr<net::net_address> result = get_numeric_address(address);
	return result != nullptr && !disallow_deprecated_formats(address, result);
}

NET_INLINE std::vector<std::uint8_t> net::net_address::ipv4mapped_to_ipv4(
	const std::vector<std::uint8_t>& address)
{
	return{ address[12], address[13], address[14], address[15] };
}

NET_INLINE int net::net_address::get_family(void) const
{
	return family_;
}

NET_INLINE std::string net::net_address::to_string(void)
{
	return get_host_name() + "/" + get_host_address();
}

NET_INLINE std::shared_ptr<net::net_address> net::net_address::of(
	const std::vector<std::uint8_t>& address,
	const std::string& hostname)
{
	return get_by_address(address, hostname);
}

