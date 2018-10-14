NET_INLINE const uint16_t net::socket_address::get_port(void) const
{
	return port_;
}

NET_INLINE void net::socket_address::set_port(const uint16_t& port)
{
	port_ = port;
}

NET_INLINE std::shared_ptr<net::net_address> net::socket_address::get_address(void) const
{
	return addr_;
}

NET_INLINE void net::socket_address::set_address(const std::shared_ptr<net::net_address>& addr)
{
	addr_ = addr;
}

NET_INLINE const std::string& net::socket_address::get_host_name(void) const
{
	return (addr_ != nullptr) ? addr_->get_host_name() : hostname_;
}

NET_INLINE const std::string net::socket_address::to_string(void) const
{
	std::ostringstream stream;
	stream << static_cast<int>(port_);
	return ((addr_ != nullptr) ? addr_->to_string() : hostname_) +
		"(" + stream.str() + ")";
}

