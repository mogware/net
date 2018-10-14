NET_INLINE bool net::net6_address::is_multicast_address(void) const
{
	return ((address_[0] & 0xFF) == 0xFF);
}

NET_INLINE bool net::net6_address::is_any_local_address(void) const
{
	std::uint8_t test = 0x00;
	for (int i = 0; i < 16; i++)
		test |= address_[i];
	return (test == 0x00);

}

NET_INLINE bool net::net6_address::is_loopback_address(void) const
{
	std::uint8_t test = 0x00;
	for (int i = 0; i < 15; i++)
		test |= address_[i];
	return (test == 0x00) && (address_[15] == 0x01);

}

NET_INLINE bool net::net6_address::is_link_local_address(void) const
{
	return ((address_[0] & 0xFF) == 0xFE &&
		(address_[1] & 0xC0) == 0x80);
}

NET_INLINE bool net::net6_address::is_site_local_address(void) const
{
	return ((address_[0] & 0xFF) == 0xFE &&
		(address_[1] & 0xC0) == 0xC0);
}

NET_INLINE std::vector<std::uint8_t> net::net6_address::get_address(void) const
{
	return std::vector<uint8_t>(&address_[0], &address_[16]);
}
