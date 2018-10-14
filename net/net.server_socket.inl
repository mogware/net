NET_INLINE bool net::server_socket::has_impl(void) const
{
	return impl_ != nullptr;
}

NET_INLINE std::shared_ptr<net::socket_impl>& net::server_socket::get_impl()
{
	return impl_;
}

NET_INLINE void net::server_socket::set_impl(const std::shared_ptr<socket_impl>& impl)
{
	if (impl_ != nullptr)
		throw socket_exception("Implementation already set");
	impl_ = impl;
}

NET_INLINE void net::server_socket::check_open(void) const
{
	if (is_closed())
		throw socket_exception("Socket is closed");
}
