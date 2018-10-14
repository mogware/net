NET_INLINE bool net::net4_address::is_multicast_address(void) const
{
	return ((address_ & 0xF0000000) == 0xE0000000);
}

NET_INLINE bool net::net4_address::is_any_local_address(void) const
{
	return address_ == 0;
}

NET_INLINE bool net::net4_address::is_loopback_address(void) const
{
	return ((address_ >> 24) & 0xFF) == 127;
}

NET_INLINE bool net::net4_address::is_link_local_address(void) const
{
	// 169.254.0.0/16
	return (((address_ >> 24) & 0xFF) == 169) &&
		(((address_ >> 16) & 0xFF) == 254);
}

NET_INLINE bool net::net4_address::is_site_local_address(void) const
{
	// 10/8, 172.16/12, 192.168/16
	return (((address_ >> 24) & 0xFF) == 10)
		|| ((((address_ >> 24) & 0xFF) == 172)
			&& (((address_ >> 16) & 0xF0) == 16))
		|| ((((address_ >> 24) & 0xFF) == 192)
			&& (((address_ >> 16) & 0xFF) == 168));
}

NET_INLINE std::vector<std::uint8_t> net::net4_address::get_address(void) const
{
	return{
		(std::uint8_t)((address_ >> 24) & 0xFF),
		(std::uint8_t)((address_ >> 16) & 0xFF),
		(std::uint8_t)((address_ >> 8) & 0xFF),
		(std::uint8_t)((address_ >> 0) & 0xFF)
	};
}

NET_INLINE std::string net::net4_address::to_string(void) const
{
	std::stringstream stream;
	stream << ((address_ >> 24) & 0xFF) << "."
		<< ((address_ >> 16) & 0xFF) << "."
		<< ((address_ >> 8) & 0xFF) << "."
		<< ((address_ >> 0) & 0xFF);
	return stream.str();
}
