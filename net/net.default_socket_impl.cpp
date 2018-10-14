#include <ios>
#include <vector>

#include "net.exceptions.h"
#include "net.net_address.h"
#include "net.net4_address.h"
#include "net.net6_address.h"
#include "net.default_socket_impl.h"

net::default_socket_impl::default_socket_impl(void)
	: default_socket_impl(sio::invalid_socket)
{
}

net::default_socket_impl::default_socket_impl(const sio::socket_t& sock)
	: default_socket_impl(sock, 0, nullptr, 0)
{
}

net::default_socket_impl::default_socket_impl(const sio::socket_t& sock,
	const std::uint16_t& localport,
	const std::shared_ptr<net_address>& addr,
	const std::uint16_t& port)
	: socket_impl(sock, localport, addr, port)
	, shutdown_input_(false)
{
}

net::default_socket_impl::~default_socket_impl(void)
{
	close();
}

std::shared_ptr<net::socket_impl> net::default_socket_impl::of(const sio::socket_t& sock)
{
	return std::make_shared<default_socket_impl>(sock);
}

std::shared_ptr<net::socket_impl> net::default_socket_impl::of(const sio::socket_t& sock,
	const std::uint16_t& localport, const std::shared_ptr<net::net_address>& addr,
	const std::uint16_t& port)
{
	return std::make_shared<default_socket_impl>(sock, localport, addr, port);
}

void net::default_socket_impl::accept(std::shared_ptr<net::socket_impl>& new_impl)
{
	try {
		socket_address peer_address;
		sio::socket_t sock = accept(sock_, peer_address, localaddr_->get_family());
		new_impl->set_native_socket(sock);
		new_impl->set_address(peer_address.get_address());
		new_impl->set_port(peer_address.get_port());
	}
	catch (const sio::errno_exception& e) {
		if (e.get_errno() == EAGAIN || e.get_errno() == EWOULDBLOCK)
			throw socket_timeout_exception(e.what());
		throw socket_exception(e.what());
	}
	new_impl->set_local_port(get_socket_local_port(
		new_impl->get_native_socket(), localaddr_->get_family()));
	new_impl->set_local_address(get_socket_local_address(
		new_impl->get_native_socket(), localaddr_->get_family()));
}

int net::default_socket_impl::available(void) const
{
	if (shutdown_input_)
		return 0;
	try {
		int n = 0;
		sio::ioctlsocket(sock_, sio::sio_nread, &n);
		return n;
	}
	catch (const sio::errno_exception&) {
		return 0;
	}
}

void net::default_socket_impl::bind(const std::shared_ptr<net::net_address>& addr,
	const std::uint16_t& port)
{
	try {
		bind(sock_, addr, port);
		addr_ = addr;
		if (port != 0)
			localport_ = port;
		else
			localport_ = get_socket_local_port(sock_, addr->get_family());
		localaddr_ = get_socket_local_address(sock_, addr->get_family());
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

void net::default_socket_impl::close(void)
{
	if (sock_ == sio::invalid_socket)
		return;
	try {
		sio::closesocket(sock_);
		sock_ = sio::invalid_socket;
	}
	catch (const sio::errno_exception&) {
		// TODO: are there any cases in which we should throw?
	}
}

void net::default_socket_impl::connect(const std::string& hostname,
	const std::uint16_t& port)
{
	connect(net_address::of(hostname), port);
}

void net::default_socket_impl::connect(const std::shared_ptr<net::net_address>& addr,
	const std::uint16_t& port)
{
	connect(addr, port, 0);
}

void net::default_socket_impl::connect(const std::shared_ptr<net::net_address>& addr,
	const std::uint16_t& port, const int& timeout)
{
	std::shared_ptr<net::net_address> normal = addr->is_any_local_address() ?
		addr->get_local_host() : addr;
	try {
		connect(sock_, normal, port, timeout);
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
	catch (const socket_timeout_exception& e) {
		throw e;
	}
	catch (const socket_exception& e) {
		throw e;
	}
	catch (const std::exception&) {
		throw socket_exception("unknown exception");
	}
}

void net::default_socket_impl::create(const int& family)
{
	try {
		sock_ = sio::socket(family, SOCK_STREAM, 0);
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

int net::default_socket_impl::read(std::uint8_t* buffer, const int& nbytes)
{
	if (nbytes == 0)
		return 0;
	if (shutdown_input_)
		return -1;
	try {
		int read_count = recv(sock_, buffer, nbytes, 0);
		if (read_count == -1)
			shutdown_input_ = true;	// peer closed
		return read_count;
	}
	catch (const sio::errno_exception& e) {
		if (e.get_errno() == EAGAIN || e.get_errno() == EWOULDBLOCK)
			return 0;
		throw std::ios_base::failure(e.what());
	}
}

void net::default_socket_impl::write(const std::uint8_t* buffer, const int& nbytes)
{
	if (nbytes == 0)
		return;
	try {
		send(sock_, buffer, nbytes, 0);
	}
	catch (const sio::errno_exception& e) {
		throw std::ios_base::failure(e.what());
	}
}

bool net::default_socket_impl::supports_urgent_data() const
{
	return true;
}

void net::default_socket_impl::send_urgent_data(const int& value)
{
	try {
		std::uint8_t data = (std::uint8_t) value;
		sio::send(sock_, &data, 1, MSG_OOB);
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

void net::default_socket_impl::listen(const int& backlog)
{
	try {
		sio::listen(sock_, backlog);
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

void net::default_socket_impl::shutdown_input()
{
	shutdown_input_ = true;
	try {
		sio::shutdown(sock_, SIO_SHUTDOWN_READ);
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

void net::default_socket_impl::shutdown_output()
{
	try {
		sio::shutdown(sock_, SIO_SHUTDOWN_WRITE);
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

bool net::default_socket_impl::get_option_bool(const int& id)
{
	try {
		int optval;
		int size = sizeof(optval);
		sio::getsockopt(sock_, SOL_SOCKET, id, &optval, &size);
		return optval != 0;
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

void net::default_socket_impl::set_option_bool(const int& id, const bool& val)
{
	try {
		int optval = val ? 1 : 0;
		int size = sizeof(optval);
		sio::setsockopt(sock_, SOL_SOCKET, id, &optval, size);
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

int net::default_socket_impl::get_option_int(const int& id)
{
	try {
		int optval;
		int size = sizeof(optval);
		sio::getsockopt(sock_, SOL_SOCKET, id, &optval, &size);
		return optval;
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}

void net::default_socket_impl::set_option_int(const int& id, const int& val)
{
	try {
		int optval = val;
		int size = sizeof(optval);
		sio::setsockopt(sock_, SOL_SOCKET, id, &optval, size);
	}
	catch (const sio::errno_exception& e) {
		throw socket_exception(e.what());
	}
}


sio::socket_t net::default_socket_impl::accept(const sio::socket_t& sockfd,
	socket_address& addr, const int& family)
{
	sio::socket_t sock = sio::invalid_socket;
	if (family == AF_INET) {
		sio::sock_addr4 sa;
		int salen = sizeof(sa);
		sock = sio::accept(sockfd, sa, &salen);
		addr.set_port(sa.get_port());
		addr.set_address(std::make_shared<net4_address>(sa.get_addr(), ""));
	}
	else if (family == AF_INET6) {
		sio::sock_addr6 sa;
		int salen = sizeof(sa);
		sock = sio::accept(sockfd, sa, &salen);
		addr.set_port(sa.get_port());
		addr.set_address(std::make_shared<net6_address>(sa.get_addr(), 16, ""));
	}
	return sock;
}

void net::default_socket_impl::bind(const sio::socket_t& sockfd,
	const std::shared_ptr<net::net_address>& addr, const std::uint16_t& port)
{
	if (addr->get_family() == AF_INET) {
		sio::sock_addr4 sa(addr->get_address(), port);
		sio::bind(sockfd, sa, sizeof(sa));
	}
	else if (addr->get_family() == AF_INET6) {
		sio::sock_addr6 sa(addr->get_address(), port);
		sio::bind(sockfd, sa, sizeof(sa));
	}
}

void net::default_socket_impl::connect(const sio::socket_t& sockfd,
	const std::shared_ptr<net_address>& addr, const std::uint16_t& port,
	const int& timeout)
{
	if (addr->get_family() == AF_INET) {
		sio::sock_addr4 sa(addr->get_address(), port);
		connect(sockfd, sa, sizeof(sa), timeout);
	}
	else if (addr->get_family() == AF_INET6) {
		sio::sock_addr6 sa(addr->get_address(), port);
		connect(sockfd, sa, sizeof(sa), timeout);
	}
}

void net::default_socket_impl::connect(const sio::socket_t& sockfd,
	const sio::sockaddr_t* addr, const int& len, const int& timeout)
{
	if (timeout == 0)
		sio::connect(sockfd, addr, len);
	else {
		int on = 1;
		sio::ioctlsocket(sockfd, sio::sio_nbio, &on);
		on = 0;
		std::uint64_t finish = now_millis() + timeout;
		try {
			sio::connect(sockfd, addr, len);
			sio::ioctlsocket(sockfd, sio::sio_nbio, &on);
			return;
		}
		catch (const sio::errno_exception& e) {
			if (!sio::inprogress(e))
				throw e;
		}

		int remaining;
		do {
			remaining = static_cast<int>(finish - now_millis());
			if (remaining <= 0)
				throw socket_timeout_exception("connect timed out");
		} while (!is_connected(sockfd, addr, timeout, remaining));
		sio::ioctlsocket(sockfd, sio::sio_nbio, &on);
	}
}

bool net::default_socket_impl::is_connected(const sio::socket_t& sockfd,
	const sio::sockaddr_t* addr, const int& timeout, const int& remaining)
{
	try {
		sio::reactor sr;
		sr.select(sockfd, sio::reactor::out);
		sio::reactor::event ev[1], *pev = ev;
		int rc = sr.poll(pev, 1, remaining);
		if (rc == 0)
			return false;
		int error;
		int size = sizeof(error);
		sio::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &size);
		if (error == 0)
			return true;
		throw sio::errno_exception("is_connected", sio::socket_errno(error));
	}
	catch (const sio::errno_exception& e) {
		if (e.get_errno() == EINTR)
			return false;
		else if (e.get_errno() == ETIMEDOUT)
			throw socket_timeout_exception("connect timed out");
		else
			throw socket_exception(e.what());
	}
}

int net::default_socket_impl::recv(const sio::socket_t& sockfd,
	std::uint8_t* buffer, const int& bytes, const int& flags)
{
	return sio::recv(sockfd, buffer, bytes, flags);
}

void net::default_socket_impl::send(const sio::socket_t& sockfd,
	const std::uint8_t* buffer, const int& bytes, const int& flags)
{
	std::uint8_t* pbuf = const_cast<std::uint8_t*>(buffer);
	int count = bytes;
	while (count > 0) {
		int written = sio::send(sockfd, pbuf, count, flags);
		count -= written;
		pbuf += written;
	}
}

std::uint16_t net::default_socket_impl::get_socket_local_port(
	const sio::socket_t& sockfd, const int& family)
{
	if (family == AF_INET) {
		sio::sock_addr4 sa;
		int salen = sizeof(sa);
		sio::getsockname(sockfd, sa, &salen);
		return sa.get_port();
	}
	else if (family == AF_INET6) {
		sio::sock_addr6 sa;
		int salen = sizeof(sa);
		sio::getsockname(sockfd, sa, &salen);
		return sa.get_port();
	}
	return 0;
}

std::shared_ptr<net::net_address>
net::default_socket_impl::get_socket_local_address(const sio::socket_t& sockfd,
	const int& family)
{
	if (family == AF_INET) {
		sio::sock_addr4 sa;
		int salen = sizeof(sa);
		sio::getsockname(sockfd, sa, &salen);
		return std::make_shared<net4_address>(sa.get_addr(), "");
	}
	else if (family == AF_INET6) {
		sio::sock_addr6 sa;
		int salen = sizeof(sa);
		sio::getsockname(sockfd, sa, &salen);
		return std::make_shared<net6_address>(sa.get_addr(), 16, "");
	}
	return nullptr;
}

std::uint64_t net::default_socket_impl::now_millis(void)
{
	sio::timeval_t tv;
	sio::gettimeofday(&tv, nullptr);
	return static_cast<std::uint64_t>(tv.tv_sec) * 1000 +
		tv.tv_usec / 1000;
}