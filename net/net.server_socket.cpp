#include "net.exceptions.h"
#include "net.net4_address.h"
#include "net.net6_address.h"
#include "net.server_socket.h"
#include "net.default_server_socket_impl.h"

std::shared_ptr<net::socket_impl_factory> net::server_socket::factory_;

net::server_socket::server_socket(const bool& prefer_ipv6)
	: impl_(nullptr)
	, is_bound_(false)
	, is_closed_(false)
{
	impl_ = factory_ != nullptr ? factory_->create_socket_impl() :
		std::make_shared<default_server_socket_impl>();

	std::shared_ptr<net_address> addr = prefer_ipv6 ?
		net6_address::ANY : net4_address::ANY;

	impl_->set_local_address(addr);

	try {
		impl_->create(addr->get_family());
	}
	catch (const socket_timeout_exception& e) {
		impl_->close();
		throw e;
	}
	catch (const socket_exception& e) {
		impl_->close();
		throw e;
	}
}

net::server_socket::server_socket(const std::uint16_t& port,
	const bool& prefer_ipv6)
	: server_socket(port, 50, prefer_ipv6)
{
}

net::server_socket::server_socket(const std::uint16_t& port,
	const int& backlog, const bool& prefer_ipv6)
	: server_socket(port, backlog,
		prefer_ipv6 ? net6_address::ANY : net4_address::ANY)
{
}

net::server_socket::server_socket(const std::uint16_t& port, const int& backlog,
		const std::shared_ptr<net::net_address>& localaddr)
	: impl_(nullptr)
	, is_bound_(false)
	, is_closed_(false)
{
	impl_ = factory_ != nullptr ? factory_->create_socket_impl() :
		std::make_shared<default_server_socket_impl>();

	std::shared_ptr<net_address> addr = (localaddr == nullptr) ?
		net4_address::ANY : localaddr;

	impl_->set_local_address(addr);

	try {
		impl_->create(addr->get_family());
		impl_->bind(addr, port);
		is_bound_ = true;
		impl_->listen(backlog > 0 ? backlog : 50);
	}
	catch (const socket_timeout_exception& e) {
		impl_->close();
		throw e;
	}
	catch (const socket_exception& e) {
		impl_->close();
		throw e;
	}
}

net::server_socket::~server_socket(void)
{
}

std::shared_ptr<net::socket> net::server_socket::accept(void)
{
	check_open();
	if (!is_bound())
		throw socket_exception("Socket is not bound");
	std::shared_ptr<net::socket> sock = std::make_shared<net::socket>();
	try {
		implement_accept(sock);
	}
	catch (const socket_timeout_exception& e) {
		sock->close();
		throw e;
	}
	catch (const socket_exception& e) {
		sock->close();
		throw e;
	}
	return sock;
}

void net::server_socket::bind(const net::socket_address& localaddr)
{
	bind(localaddr, 50);
}

void net::server_socket::bind(const net::socket_address& localaddr,
	const int& backlog)
{
	check_open();
	if (is_bound())
		throw socket_exception("Socket is already bound");

	std::shared_ptr<net_address> addr;
	if ((addr = localaddr.get_address()) == nullptr)
		throw unknown_host_exception(localaddr.get_host_name());
	std::uint16_t port = localaddr.get_port();

	try {
		impl_->bind(addr, port);
		is_bound_ = true;
		impl_->listen(backlog > 0 ? backlog : 50);
	}
	catch (const socket_exception& e) {
		impl_->close();
		throw e;
	}
}

void net::server_socket::close(void)
{
	is_closed_ = true;
	impl_->close();
}

bool net::server_socket::get_reuse_address(void)
{
	check_open();
	return impl_->get_option_bool(SO_REUSEADDR);
}

void net::server_socket::set_reuse_address(const bool& reuse)
{
	check_open();
	impl_->set_option_bool(SO_REUSEADDR, reuse);
}

int net::server_socket::get_receive_buffer_size(void)
{
	check_open();
	return impl_->get_option_int(SO_RCVBUF);
}

void net::server_socket::set_receive_buffer_size(const int& size)
{
	check_open();
	if (size < 1)
		throw std::invalid_argument("size < 1");
	impl_->set_option_int(SO_RCVBUF, size);
}

int net::server_socket::get_receive_timeout(void)
{
	check_open();
	return impl_->get_option_int(SO_RCVTIMEO);
}

void net::server_socket::set_receive_timeout(const int& timeout)
{
	check_open();
	if (timeout < 0)
		throw std::invalid_argument("timeout < 0");
	impl_->set_option_int(SO_RCVTIMEO, timeout);
}

std::shared_ptr<net::net_address> net::server_socket::get_local_address(void) const
{
	if (!is_bound())
		return nullptr;
	return impl_->get_local_address();
}

std::uint16_t net::server_socket::get_local_port(void) const
{
	if (!is_bound())
		return 0;
	return impl_->get_local_port();
}

net::socket_address net::server_socket::get_local_socket_address(void) const
{
	return socket_address(get_local_address(), get_local_port());
}

void net::server_socket::set_socket_impl_factory(
	const std::shared_ptr<net::socket_impl_factory>& fac)
{
	if (factory_ != nullptr)
		throw socket_exception("Factory already set");
	factory_ = fac;
}

bool net::server_socket::is_bound(void) const
{
	return is_bound_;
}

bool net::server_socket::is_closed(void) const
{
	return is_closed_;
}

void net::server_socket::implement_accept(const std::shared_ptr<net::socket>& sock)
{
	impl_->accept(sock->get_impl());
	sock->accepted();
}

#if !defined(__NET_INLINE__)
#include "net.server_socket.inl"
#endif