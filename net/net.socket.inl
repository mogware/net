NET_INLINE bool net::socket::has_impl(void) const
{
	return impl_ != nullptr;
}

NET_INLINE std::shared_ptr<net::socket_impl>& net::socket::get_impl(void)
{
	return impl_;
}

NET_INLINE void net::socket::set_impl(const std::shared_ptr<socket_impl>& impl)
{
	if (impl_ != nullptr)
		throw socket_exception("Implementation already set");
	impl_ = impl;
	socketbuf_.set_socket_impl(impl);
}

NET_INLINE char* net::socket::socketbuf::obase(int n) const
{
	return (char*)&obuffer_[n];
}

NET_INLINE char* net::socket::socketbuf::oend(int n) const
{
	return (char*)&obuffer_[n];
}

NET_INLINE char* net::socket::socketbuf::ibase(int n) const
{
	return (char*)&ibuffer_[n];
}

NET_INLINE char* net::socket::socketbuf::iend(int n) const
{
	return (char*)&ibuffer_[n];
}
