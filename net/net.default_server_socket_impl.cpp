#include "net.default_server_socket_impl.h"

net::default_server_socket_impl::default_server_socket_impl(void)
	: default_socket_impl()
{
}

net::default_server_socket_impl::default_server_socket_impl(
		const sio::socket_t& sock)
	: default_socket_impl(sock)
{
}

net::default_server_socket_impl::~default_server_socket_impl(void)
{
}

std::shared_ptr<net::socket_impl> net::default_server_socket_impl::of(
	const sio::socket_t& sock)
{
	return std::make_shared<default_server_socket_impl>(sock);
}

void net::default_server_socket_impl::create(const int& family)
{
	default_socket_impl::create(family);
	set_option_bool(SO_REUSEADDR, true);
}
