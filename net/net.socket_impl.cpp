#include "net.socket_impl.h"

net::socket_impl::socket_impl(const sio::socket_t& sock, const std::uint16_t& local_port,
	const std::shared_ptr<net::net_address>& addr, const std::uint16_t& port)
	: addr_(addr)
	, port_(port)
	, sock_(sock)
	, localaddr_(nullptr)
	, localport_(local_port)
{
}

net::socket_impl::~socket_impl(void)
{
}

#if !defined(__NET_INLINE__)
#include "net.socket_impl.inl"
#endif
