#ifndef __NET_SERVER_SOCKET__
#define __NET_SERVER_SOCKET__

#include <memory>

#include "net.net_address.h"
#include "net.socket_address.h"
#include "net.socket.h"
#include "net.socket_impl.h"
#include "net.socket_impl_factory.h"

namespace net
{
	class server_socket
	{
		std::shared_ptr<socket_impl> impl_;
		volatile bool is_bound_;
		volatile bool is_closed_;
	private:
		static std::shared_ptr<socket_impl_factory> factory_;
	public:
		/**
		* Constructs a new unbound server socket.
		*/
		server_socket(const bool& prefer_ipv6 = false);

		/**
		* Constructs a new server socket instance bound to the given port.
		* The backlog is set to 50. If port is zero, a port will be assigned
		* by the OS.
		*/
		server_socket(const std::uint16_t& port,
			const bool& prefer_ipv6 = false);

		/**
		* Constructs a new server socket instance bound to the given port.
		* The backlog is set to argument specified.
		* If port is zero, a port will be assigned by the OS.
		*/
		server_socket(const std::uint16_t& port, const int& backlog,
			const bool& prefer_ipv6 = false);

		/**
		* Constructs a new server socket instance bound to the given local
		* address and port. The backlog is set to argument specified.
		* If localaddr is null, the ANY address is used.
		* If port is zero, a port will be assigned by the OS.
		*/
		server_socket(const std::uint16_t& port, const int& backlog,
			const std::shared_ptr<net_address>& localaddr);
	public:
		virtual ~server_socket(void);
	public:
		/**
		* Sets the server socket implementation factory of this instance. This
		* method may only be invoked only once during the application lifetime.
		*/
		static void set_socket_impl_factory(
			const std::shared_ptr<socket_impl_factory>& fac);
	public:
		/**
		* Waits for an incoming request and blocks until the connection is opened.
		* This method returns a socket object representing the just opened
		* connection.
		*/
		virtual std::shared_ptr<net::socket> accept(void);

		/**
		* Binds this server socket to the given local socket address with a maximum
		* backlog of 50 unaccepted connections. If the localaddr is set to null
		* the socket will be bound to an available local address on any free port
		* of the system.
		*/
		virtual void bind(const socket_address& localaddr);

		/**
		* Binds this server socket to the given local socket address. If the
		* localaddr is set to null the socket will be bound to an available local
		* address on any free port of the system.
		*/
		virtual void bind(const socket_address& localaddr, const int& backlog);

		/**
		* Closes this server socket and its implementation. Any attempt to connect
		* to this socket thereafter will fail.
		*/
		virtual void close(void);
	public:
		/**
		* Gets the value of the socket option SO_REUSEADDR.
		*/
		virtual bool get_reuse_address(void);

		/**
		* Sets the value for the socket option SO_REUSEADDR.
		*/
		virtual void set_reuse_address(const bool& reuse);

		/**
		* Gets the value of the socket option SO_RCVBUF.
		*/
		virtual int get_receive_buffer_size(void);

		/**
		* Sets the value for the socket option SO_RCVBUF.
		*/
		virtual void set_receive_buffer_size(const int& size);

		/**
		* Gets the socket accept timeout.
		*/
		virtual int get_receive_timeout(void);

		/**
		* Sets the accept timeout in milliseconds for this socket.
		* This accept timeout defines the period the socket will block waiting to
		* accept a connection before throwing an interrupted exception. The value
		* 0 (default) is used to set an infinite timeout. To have effect this option
		* must be set before the blocking method was called.
		*/
		virtual void set_receive_timeout(const int& timeout);
	public:
		/**
		* Gets the local IP address of this server socket or null if the
		* socket is unbound. This is useful for multihomed hosts.
		*/
		virtual std::shared_ptr<net_address> get_local_address(void) const;

		/**
		* Gets the local port of this server socket or -1 if the socket is
		* unbound.
		*/
		virtual std::uint16_t get_local_port(void) const;
	public:
		/**
		* Gets the local socket address of this server socket or null if
		* the socket is unbound. This is useful on multihomed hosts.
		*/
		virtual socket_address get_local_socket_address(void) const;
	public:
		/**
		* Returns whether this server socket is bound to a local address
		* and port or not.
		*/
		virtual bool is_bound(void) const;
		/**
		* Returns whether this server socket is closed or not.
		*/
		virtual bool is_closed(void) const;
	public:
		/**
		* Invokes the server socket implementation to accept a connection on the
		* given socket.
		*/
		virtual void implement_accept(const std::shared_ptr<net::socket>& sock);
	public:
		NET_INLINE bool has_impl(void) const;
		NET_INLINE std::shared_ptr<socket_impl>& get_impl(void);
		NET_INLINE void set_impl(const std::shared_ptr<socket_impl>& impl);
	private:
		NET_INLINE void check_open(void) const;
	private:
		server_socket(const server_socket&);
		server_socket& operator=(const server_socket&);
		server_socket& operator=(const server_socket&&);
	};
}

#if defined(__NET_INLINE__)
#include "net.server_socket.inl"
#endif

#endif

