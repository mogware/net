NET_INLINE sio::socket_t net::socket_impl::get_native_socket(void) const
{
	return sock_;
}

NET_INLINE void net::socket_impl::set_native_socket(const sio::socket_t& sock)
{
	sock_ = sock;
}

NET_INLINE std::shared_ptr<net::net_address> net::socket_impl::get_address(void) const
{
	return addr_;
}

NET_INLINE void net::socket_impl::set_address(const std::shared_ptr<net::net_address>& addr)
{
	addr_ = addr;
}

NET_INLINE std::uint16_t net::socket_impl::get_port(void) const
{
	return port_;
}

NET_INLINE void net::socket_impl::set_port(const std::uint16_t& port)
{
	port_ = port;
}

NET_INLINE std::shared_ptr<net::net_address> net::socket_impl::get_local_address(void) const
{
	return localaddr_;
}

NET_INLINE void net::socket_impl::set_local_address(const std::shared_ptr<net::net_address>& addr)
{
	localaddr_ = addr;
}

NET_INLINE std::uint16_t net::socket_impl::get_local_port(void) const
{
	return localport_;
}

NET_INLINE void net::socket_impl::set_local_port(const std::uint16_t& port)
{
	localport_ = port;
}

