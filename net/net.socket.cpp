#include "net.exceptions.h"
#include "net.net4_address.h"
#include "net.net6_address.h"
#include "net.socket.h"
#include "net.default_socket_impl.h"

#include <iostream>
#include <sstream>

std::shared_ptr<net::socket_impl_factory> net::socket::factory_;

net::socket::socket(const bool& prefer_ipv6)
	: impl_(nullptr)
	, is_created_(false)
	, is_bound_(false)
	, is_connected_(false)
	, is_closed_(false)
	, is_input_shutdown_(false)
	, is_output_shutdown_(false)
	, socketbuf_(nullptr)
{
	impl_ = factory_ != nullptr ? factory_->create_socket_impl() :
		std::make_shared<default_socket_impl>();
	socketbuf_.set_socket_impl(impl_);
	impl_->set_local_address(prefer_ipv6 ? net6_address::ANY : net4_address::ANY);
}

net::socket::socket(const std::string& dstname, const std::uint16_t& dstport,
	const bool& prefer_ipv6)
	: socket(prefer_ipv6)
{
	try_all_addresses(dstname, dstport, nullptr, 0);
}

net::socket::socket(const std::string& dstname, const std::uint16_t& dstport,
	const std::shared_ptr<net::net_address>& localaddr,
	const std::uint16_t& localport)
	: socket()
{
	try_all_addresses(dstname, dstport, localaddr, localport);
}

net::socket::socket::socket(const std::shared_ptr<net::net_address>& dstaddr,
	const std::uint16_t& dstport, const bool& prefer_ipv6)
	: socket(prefer_ipv6)
{
	startup_socket(dstaddr, dstport, nullptr, 0);
}

net::socket::socket(const std::shared_ptr<net_address>& dstaddr,
	const std::uint16_t& dstport,
	const std::shared_ptr<net::net_address>& localaddr,
	const std::uint16_t& localport)
	: socket()
{
	startup_socket(dstaddr, dstport, localaddr, localport);
}

net::socket::socket(const std::shared_ptr<net::socket_impl>& impl,
	const bool& prefer_ipv6)
	: impl_(impl)
	, is_created_(false)
	, is_bound_(false)
	, is_connected_(false)
	, is_closed_(false)
	, is_input_shutdown_(false)
	, is_output_shutdown_(false)
	, socketbuf_(impl)
{
	if (impl_ == nullptr)
		return;
	impl_->set_local_address(prefer_ipv6 ? net6_address::ANY : net4_address::ANY);
}

net::socket::~socket(void)
{
}

void net::socket::bind(const socket_address& localaddr)
{
	std::shared_ptr<net_address> addr;
	if ((addr = localaddr.get_address()) == nullptr)
		throw unknown_host_exception(localaddr.get_host_name());
	std::uint16_t port = localaddr.get_port();

	check_open_and_create(true, addr->get_family());
	if (is_bound())
		throw socket_exception("Socket is already bound");
	impl_->bind(addr, port);
	is_bound_ = true;
}

void net::socket::connect(const socket_address& remoteaddr, const int& timeout)
{
	std::shared_ptr<net_address> dstaddr;
	if ((dstaddr = remoteaddr.get_address()) == nullptr)
		throw unknown_host_exception(remoteaddr.get_host_name());
	std::uint16_t dstport = remoteaddr.get_port();

	std::shared_ptr<net_address> addr = (dstaddr->get_family() == AF_INET6) ?
		net6_address::ANY : net4_address::ANY;

	check_open_and_create(true, addr->get_family());
	if (is_connected())
		throw socket_exception("Socket is already connected");

	if (!is_bound()) {
		impl_->bind(addr, 0);
		is_bound_ = true;
	}
	impl_->connect(dstaddr, dstport, timeout);
	is_connected_ = true;
}

void net::socket::send_urgent_data(const int& value)
{
	if (value < 0 || value > 255)
	{
		std::ostringstream stream;
		stream << value;
		throw std::invalid_argument("Doesn't fit in a byte: " + stream.str());
	}
	impl_->send_urgent_data(value);
}

void net::socket::shutdown_input(void)
{
	if (is_input_shutdown())
		throw socket_exception("Socket input is shutdown");
	check_open_and_create(false, 0);
	impl_->shutdown_input();
	is_input_shutdown_ = true;
}

void net::socket::shutdown_output(void)
{
	if (is_output_shutdown())
		throw socket_exception("Socket input is shutdown");
	check_open_and_create(false, 0);
	impl_->shutdown_output();
	is_output_shutdown_ = true;
}

void net::socket::close(void)
{
	is_closed_ = true;
	impl_->close();
}

bool net::socket::get_keep_alive(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_bool(SO_KEEPALIVE);
}

void net::socket::set_keep_alive(const bool& keep_alive)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	impl_->set_option_bool(SO_KEEPALIVE, keep_alive);
}

int net::socket::get_linger(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_int(SO_LINGER);
}

void net::socket::set_linger(const bool& on, const int& timeout)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	if (on && timeout < 0)
		throw std::invalid_argument("timeout < 0");
	if (on)
		impl_->set_option_int(SO_LINGER, timeout);
	else
		impl_->set_option_bool(SO_LINGER, false);
}

int net::socket::get_receive_buffer_size(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_int(SO_RCVBUF);
}

void net::socket::set_receive_buffer_size(const int& size)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	if (size < 1)
		throw std::invalid_argument("size < 1");
	impl_->set_option_int(SO_RCVBUF, size);
}

int net::socket::get_send_buffer_size(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_int(SO_SNDBUF);
}

void net::socket::set_send_buffer_size(const int& size)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	if (size < 1)
		throw std::invalid_argument("size < 1");
	impl_->set_option_int(SO_SNDBUF, size);
}

int net::socket::get_receive_timeout(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_int(SO_RCVTIMEO);
}

void net::socket::set_receive_timeout(const int& timeout)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	if (timeout < 0)
		throw std::invalid_argument("timeout < 0");
	impl_->set_option_int(SO_RCVTIMEO, timeout);
}

int net::socket::get_send_timeout(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_int(SO_SNDTIMEO);
}

void net::socket::set_send_timeout(const int& timeout)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	if (timeout < 0)
		throw std::invalid_argument("timeout < 0");
	impl_->set_option_int(SO_SNDTIMEO, timeout);
}

bool net::socket::get_tcp_no_delay(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_bool(TCP_NODELAY);
}

void net::socket::set_tcp_no_delay(const bool& on)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	impl_->set_option_bool(TCP_NODELAY, on);
}

bool net::socket::get_reuse_address(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_bool(SO_REUSEADDR);
}

void net::socket::set_reuse_address(const bool& reuse)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	impl_->set_option_bool(SO_REUSEADDR, reuse);
}

bool net::socket::get_oob_inline(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_bool(SO_OOBINLINE);
}

void net::socket::set_oob_inline(const bool& oobinline)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	impl_->set_option_bool(SO_OOBINLINE, oobinline);
}

int net::socket::get_traffic_class(void)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	return impl_->get_option_int(IP_TOS);
}

void net::socket::set_traffic_class(const int& value)
{
	check_open_and_create(true, impl_->get_local_address()->get_family());
	if (value < 0 || value > 255)
	{
		std::ostringstream stream;
		stream << value;
		throw std::invalid_argument("Doesn't fit in a byte: " + stream.str());
	}
	impl_->set_option_int(IP_TOS, value);
}

std::shared_ptr<net::net_address> net::socket::get_address(void) const
{
	if (!is_connected())
		return nullptr;
	return impl_->get_address();
}

std::shared_ptr<net::net_address> net::socket::get_local_address(void) const
{
	if (!is_bound())
		return nullptr;
	return impl_->get_local_address();
}

std::uint16_t net::socket::get_port(void) const
{
	if (!is_connected())
		return 0;
	return impl_->get_port();
}

std::uint16_t net::socket::get_local_port(void) const
{
	if (!is_bound())
		return 0;
	return impl_->get_local_port();
}

net::socket_address net::socket::get_local_socket_address(void) const
{
	return socket_address(get_local_address(), get_local_port());
}

net::socket_address net::socket::get_remote_socket_address(void) const
{
	return socket_address(get_address(), get_port());
}

void net::socket::set_socket_impl_factory(
	const std::shared_ptr<net::socket_impl_factory>& fac)
{
	if (factory_ != nullptr)
		throw socket_exception("Factory already set");
	factory_ = fac;
}

void net::socket::accepted(void)
{
	is_created_ = is_bound_ = is_connected_ = true;
}

bool net::socket::is_connected(void) const
{
	return is_connected_;
}

bool net::socket::is_bound(void) const
{
	return is_bound_;
}

bool net::socket::is_input_shutdown(void) const
{
	return is_input_shutdown_;
}

bool net::socket::is_output_shutdown(void) const
{
	return is_output_shutdown_;
}

bool net::socket::is_closed(void) const
{
	return is_closed_;
}

std::basic_streambuf<char, std::char_traits<char>>*
net::socket::get_stream(void)
{
	return &socketbuf_;
}

void net::socket::try_all_addresses(const std::string& dstname,
	const std::uint16_t& dstport,
	const std::shared_ptr<net::net_address>& localaddr,
	const std::uint16_t& localport)
{
	std::shared_ptr<net::net_address> addr = (localaddr != nullptr) ?
		localaddr : impl_->get_local_address();

	std::vector<std::shared_ptr<net_address>> dstaddrs =
		net_address::get_all_by_name(dstname);

	bool match = true;
	for (int n = 0; n < 2; ++n) {
		for (std::size_t i = 0; i < dstaddrs.size(); ++i) {
			std::shared_ptr<net_address> dstaddr = dstaddrs[i];
			if ((addr->get_family() == dstaddr->get_family()) != match)
				continue;
			try {
				startup_socket(dstaddr, dstport, localaddr, localport);
				return;
			}
			catch (const std::exception&) {
			}
		}
		match = false;
	}

	throw socket_exception("Cannot connect to " + dstname);
}

void net::socket::startup_socket(std::shared_ptr<net::net_address> dstaddr,
	const std::uint16_t& dstport,
	const std::shared_ptr<net::net_address>& localaddr,
	const std::uint16_t& localport)
{
	std::shared_ptr<net::net_address> addr = (localaddr != nullptr) ?
		localaddr : (dstaddr->get_family() == AF_INET6) ?
		net6_address::ANY : net4_address::ANY;
	try {
		impl_->create(addr->get_family());
		is_created_ = true;
		impl_->bind(addr, localport);
		is_bound_ = true;
		impl_->connect(dstaddr, dstport);
		is_connected_ = true;
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

void net::socket::check_open_and_create(const bool& create, const int& family)
{
	if (is_closed())
		throw socket_exception("Socket is closed");
	if (!create) {
		if (!is_connected())
			throw socket_exception("Socket is not connected");
		return; // a possible bug?
	}
	if (is_created_)
		return;
	try {
		impl_->create(family);
	}
	catch (const socket_exception& e) {
		throw e;
	}
	is_created_ = true;
}

net::socket::socketbuf::socketbuf(const std::shared_ptr<net::socket_impl>& impl)
	: impl_(impl)
{
	setp(obase(), oend());
	setg(iend(), iend(), iend());
}

net::socket::socketbuf::~socketbuf(void)
{
}

void net::socket::socketbuf::set_socket_impl(
	const std::shared_ptr<net::socket_impl>& impl)
{
	if (impl_ != nullptr)
		throw socket_exception("Implementation already set");
	impl_ = impl;
}

net::socket::socketbuf::int_type net::socket::socketbuf::overflow(
	net::socket::socketbuf::int_type c)
{
	int_type eof = std::char_traits<char>::eof();
	if (c != eof)
		*pptr() = c, pbump(1);
	return sync() == -1 ? eof : c;
}

net::socket::socketbuf::int_type net::socket::socketbuf::underflow(void)
{
	if (gptr() < egptr()) // buffer not exhausted
		return std::char_traits<char>::to_int_type(*gptr());
	char* base = ibase();
	char* start = base;
	if (eback() == base) {
		// make room for 2 putback characters
		std::memmove(base, egptr() - 2, 2);
		start += 2;
	}
	try {
		int size = impl_->read((std::uint8_t*) start, (int)(iend() - start));
		if (size < 1)
			return std::char_traits<char>::eof();
		setg(base, start, start + size);
	}
	catch (const std::ios_base::failure&) {
		return std::char_traits<char>::eof();
	}
	return std::char_traits<char>::to_int_type(*gptr());
}

int net::socket::socketbuf::sync(void)
{
	try {
		impl_->write((std::uint8_t*) pbase(), (int)(pptr() - pbase()));
		setp(obase(), oend());
	}
	catch (const std::ios_base::failure&) {
		return -1;
	}
	return 0;
}

std::streamsize net::socket::socketbuf::showmanyc(void)
{
	return impl_->available();
}

#if !defined(__NET_INLINE__)
#include "net.socket.inl"
#endif

