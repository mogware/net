#ifndef __NET_SOCKET_IMPL__
#define __NET_SOCKET_IMPL__

#include <memory>
#include <string>

#include "sio.h"
#include "net.config.h"
#include "net.net_address.h"

namespace net
{
	class socket_impl
	{
	protected:
		std::shared_ptr<net_address> addr_;
		std::uint16_t port_;
		sio::socket_t sock_;
		std::shared_ptr<net_address> localaddr_;
		std::uint16_t localport_;
	public:
		socket_impl(const sio::socket_t& sock, const std::uint16_t& local_port,
			const std::shared_ptr<net_address>& addr, const std::uint16_t& port);
		virtual ~socket_impl(void);
	public:
		/**
		* Waits for an incoming request and blocks until the connection is opened
		* on the given socket.
		*/
		virtual void accept(std::shared_ptr<socket_impl>& new_socket) = 0;

		/**
		* Returns the available number of bytes which are readable from this socket
		* without blocking.
		*/
		virtual int available(void) const = 0;

		/**
		* Binds this socket to the specified local host address and port number.
		*/
		virtual void bind(const std::shared_ptr<net_address>& addr,
			const std::uint16_t& port) = 0;

		/**
		* Closes this socket. This makes later access invalid.
		*/
		virtual void close(void) = 0;

		/**
		* Connects this socket to the specified remote host and port number.
		*/
		virtual void connect(const std::string& hostname,
			const std::uint16_t& port) = 0;

		/**
		* Connects this socket to the specified remote host address and port
		* number.
		*/
		virtual void connect(const std::shared_ptr<net_address>& addr,
			const std::uint16_t& port) = 0;

		/**
		* Connects this socket to the remote host address and port number specified
		* and with the given timeout. This method will block indefinitely if the
		* timeout is set to zero.
		*/
		virtual void connect(const std::shared_ptr<net_address>& addr,
			const std::uint16_t& port, const int& timeout) = 0;

		/**
		* Creates a new unconnected socket.
		*/
		virtual void create(const int& family) = 0;

		/**
		* Read bytes from socket
		*/
		virtual int read(std::uint8_t* buffer, const int& nbytes) = 0;

		/**
		* Write bytes to socket
		*/
		virtual void write(const std::uint8_t* buffer, const int& nbytes) = 0;

		/**
		* Returns whether the socket supports urgent data or not.
		*/
		virtual bool supports_urgent_data() const = 0;

		/**
		* Sends the single byte of urgent data on the socket.
		*/
		virtual void send_urgent_data(const int& value) = 0;

		/**
		* Listens for connection requests on this streaming socket. Incoming
		* connection requests are queued up to the limit specified by backlog.
		*/
		virtual void listen(const int& backlog) = 0;

		/**
		* Closes the input channel of this socket.
		*/
		virtual void shutdown_input(void) = 0;

		/**
		* Closes the output channel of this socket.
		*/
		virtual void shutdown_output(void) = 0;

	public:
		/**
		* Gets the value for the specified socket option.
		*/
		virtual bool get_option_bool(const int& id) = 0;
		virtual int get_option_int(const int& id) = 0;

		/**
		* Sets the value of the specified socket option.
		*/
		virtual void set_option_bool(const int& id, const bool& val) = 0;
		virtual void set_option_int(const int& id, const int& val) = 0;
	public:
		/**
		* Gets the native socket handle of this socket.
		*/
		NET_INLINE sio::socket_t get_native_socket(void) const;

		/**
		* Sets the native socket handle of this socket.
		*/
		NET_INLINE void set_native_socket(const sio::socket_t& sock);

		/**
		* Gets the remote address this socket is connected to.
		*/
		NET_INLINE std::shared_ptr<net_address> get_address(void) const;

		/**
		* Sets the remote address this socket is connected to.
		*/
		NET_INLINE void set_address(const std::shared_ptr<net_address>& addr);

		/**
		* Gets the remote port number of this socket.
		*/
		NET_INLINE std::uint16_t get_port(void) const;

		/**
		* Gets the remote port number of this socket.
		*/
		NET_INLINE void set_port(const std::uint16_t& port);

		/**
		* Gets the local address this socket is connected to.
		*/
		NET_INLINE std::shared_ptr<net_address> get_local_address(void) const;

		/**
		* Sets the local address this socket is connected to.
		*/
		NET_INLINE void set_local_address(const std::shared_ptr<net_address>& addr);

		/**
		* Gets the local port number of this socket.
		*/
		NET_INLINE std::uint16_t get_local_port(void) const;

		/**
		* Sets the local port number of this socket.
		*/
		NET_INLINE void set_local_port(const std::uint16_t& port);
	private:
		socket_impl(const socket_impl&);
		socket_impl& operator=(const socket_impl&);
		socket_impl& operator=(const socket_impl&&);
	};
}

#if defined(__NET_INLINE__)
#include "net.socket_impl.inl"
#endif

#endif

