#ifndef __NET_SOCKET__
#define __NET_SOCKET__

#include <memory>
#include <streambuf>

#include "net.socket_address.h"
#include "net.socket_impl_factory.h"

namespace net
{
	class socket
	{
		std::shared_ptr<socket_impl> impl_;
		volatile bool is_created_;
		volatile bool is_bound_;
		volatile bool is_connected_;
		volatile bool is_closed_;
		volatile bool is_input_shutdown_;
		volatile bool is_output_shutdown_;
	private:
		static std::shared_ptr<socket_impl_factory> factory_;
	public:
		/**
		* Creates an unconnected socket:
		*/
		socket(const bool& prefer_ipv6 = false);

		/**
		* Creates a socket and connects it to the specified port number on
		* the named host.
		*/
		socket(const std::string& dstname, const std::uint16_t& dstport,
			const bool& prefer_ipv6 = false);

		/**
		* Creates a socket and connects it to the specified remote host on 
		* the specified remote port.
		*/
		socket(const std::string& dstname, const std::uint16_t& dstport,
			const std::shared_ptr<net_address>& localaddr,
			const std::uint16_t& localport);

		/**
		* Creates a socket and connects it to the specified port number
		* at the specified IP address.
		*/
		socket(const std::shared_ptr<net_address>& dstaddr,
			const std::uint16_t& dstport, const bool& prefer_ipv6 = false);

		/**
		* Creates a socket and connects it to the specified remote address
		* on the specified remote port.
		*/
		socket(const std::shared_ptr<net_address>& dstaddr,
			const std::uint16_t& dstport,
			const std::shared_ptr<net_address>& localaddr,
			const std::uint16_t& localport);
	protected:
		/**
		* Creates an unconnected socket with a user specified socket_impl.
		*/
		socket(const std::shared_ptr<socket_impl>& impl,
			const bool& prefer_ipv6 = false);
	public:
		virtual ~socket(void);
	public:
		/**
		* Sets the client socket implementation factory for the application.
		* The factory can be specified only once. 
		*/
		static void set_socket_impl_factory(
			const std::shared_ptr<socket_impl_factory>& fac);
	public:
		/**
		* Binds the socket to a local address. If the address is null, then
		* the system will pick up an ephemeral port and a valid local address
		* to bind the socket.
		*/
		virtual void bind(const socket_address& localaddr);

		/**
		* Connects this socket to the server with a specified timeout value.
		* A timeout of zero is interpreted as an infinite timeout. The connection
		* will then block until established or an error occurs.
		*/
		virtual void connect(const socket_address& remoteaddr, const int& timeout = 0);

		/**
		* Send one byte of urgent data on the socket. The byte to be sent is
		* the lowest eight bits of the data parameter.
		* The urgent byte is sent after any preceding writes to the socket and
		* before any future writes to the socket.
		*/
		virtual void send_urgent_data(const int& value);

		/**
		* Places the input stream for this socket at "end of stream". Any data
		* sent to the input stream side of the socket is acknowledged and then
		* silently discarded. 
		*/
		virtual void shutdown_input(void);

		/**
		* Disables the output stream for this socket. For the socket, any
		* previously written data will be sent followed by TCP's normal
		* connection termination sequence.
		*/
		virtual void shutdown_output(void);

		/**
		* Closes this socket.
		*/
		virtual void close(void);
	public:
		/**
		* Tests if SO_KEEPALIVE is enabled.
		*/
		virtual bool get_keep_alive(void);

		/**
		* Enable/disable SO_KEEPALIVE.
		*/
		virtual void set_keep_alive(const bool& keep_alive);

		/**
		* Returns setting for SO_LINGER. -1 returns implies that the option
		* is disabled. The setting only affects socket close.
		*/
		virtual int get_linger(void);

		/**
		* Enable/disable SO_LINGER with the specified linger time in seconds.
		* The maximum timeout value is platform specific. The setting only
		* affects socket close.
		*/
		virtual void set_linger(const bool& on, const int& timeout);

		/**
		* Gets the value of the SO_RCVBUF option for this socket, that is the
		* buffer size used by the platform for input on this socket.
		*/
		virtual int get_receive_buffer_size(void);

		/**
		* Sets the SO_RCVBUF option to the specified value for this socket.
		* The SO_RCVBUF option is used by the platform's networking code as
		* a hint for the size to set the underlying network I/O buffers. 
		*/
		virtual void set_receive_buffer_size(const int& size);

		/**
		* Get value of the SO_SNDBUF option for this socket, that is the
		* buffer size used by the platform for output on this socket.
		*/
		virtual int get_send_buffer_size(void);

		/**
		* Sets the SO_SNDBUF option to the specified value for this socket.
		* The SO_SNDBUF option is used by the platform's networking code as
		* a hint for the size to set the underlying network I/O buffers. 
		*/
		virtual void set_send_buffer_size(const int& size);

		/**
		* Returns setting for SO_RCVTIMEO. 0 returns implies that the option
		* is disabled (i.e., timeout of infinity).
		*/
		virtual int get_receive_timeout(void);

		/**
		* Sets the SO_RCVTIMEO option to the specified value for this socket.
		* Enable/disable SO_RCVTIMEO with the specified timeout, in 
		* milliseconds. With this option set to a non-zero timeout, a read
		* call on the this socket will block for only this amount of time.
		*/
		virtual void set_receive_timeout(const int& timeout);

		/**
		* Returns setting for SO_SNDTIMEO. 0 returns implies that the option
		* is disabled (i.e., timeout of infinity).
		*/
		virtual int get_send_timeout(void);

		/**
		* Sets the SO_SNDTIMEO option to the specified value for this socket.
		* Enable/disable SO_SNDTIMEO with the specified timeout, in
		* milliseconds. With this option set to a non-zero timeout, a write
		* call on the this socket will block for only this amount of time.
		*/
		virtual void set_send_timeout(const int& timeout);

		/**
		* Tests if TCP_NODELAY is enabled.
		*/
		virtual bool get_tcp_no_delay(void);

		/**
		* Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
		*/
		virtual void set_tcp_no_delay(const bool& on);

		/**
		* Tests if SO_REUSEADDR is enabled.
		*/
		virtual bool get_reuse_address(void);

		/**
		* Enable/disable the SO_REUSEADDR socket option. 
		*/
		virtual void set_reuse_address(const bool& reuse);

		/**
		* Tests if OOBINLINE is enabled.
		*/
		virtual bool get_oob_inline(void);

		/**
		* Enable/disable OOBINLINE (receipt of TCP urgent data) By default,
		* this option is disabled and TCP urgent data received on a socket
		* is silently discarded. If the user wishes to receive urgent data,
		* then this option must be enabled. When enabled, urgent data is
		* received inline with normal data. 
		*/
		virtual void set_oob_inline(const bool& oobinline);

		/**
		* Gets traffic class or type-of-service in the IP header for packets
		* sent from this socket 
		*/
		virtual int get_traffic_class(void);

		/**
		* Sets traffic class or type-of-service octet in the IP header for
		* packets sent from this socket. As the underlying network
		* implementation may ignore this value applications should consider
		* it a hint. 
		*/
		virtual void set_traffic_class(const int& value);
	public:
		/**
		* Returns the address to which the socket is connected. If the socket
		* was connected prior to being closed, then this method will continue
		* to return the connected address after the socket is closed.
		*/
		virtual std::shared_ptr<net_address> get_address(void) const;

		/**
		* Gets the local address to which the socket is bound or the wildcard
		* address if the socket is not bound yet.
		*/
		virtual std::shared_ptr<net_address> get_local_address(void) const;

		/**
		* Returns the port number to which this socket is connected. If the
		* socket was connected prior to being closed, then this method will
		* continue to return the connected port number after the socket is
		* closed.
		*/
		virtual std::uint16_t get_port(void) const;

		/**
		* Returns the local port number to which this socket is bound. 
		*/
		virtual std::uint16_t get_local_port(void) const;
	public:
		/**
		* Returns the address of the endpoint this socket is bound to,
		* or null if it is not bound yet. 
		*/
		virtual socket_address get_local_socket_address(void) const;

		/**
		* Returns the address of the endpoint this socket is connected to,
		* or null if it is unconnected. 
		*/
		virtual socket_address get_remote_socket_address(void) const;
	public:
		virtual void accepted(void);
	public:
		/**
		* Returns the connection state of the socket. 
		*/
		virtual bool is_connected(void) const;

		/**
		* Returns the binding state of the socket. 
		*/
		virtual bool is_bound(void) const;

		/**
		* Returns whether the read-half of the socket connection is closed.
		*/
		virtual bool is_input_shutdown(void) const;

		/**
		* Returns whether the write-half of the socket connection is closed.
		*/
		virtual bool is_output_shutdown(void) const;

		/**
		* Returns the closed state of the socket.
		*/
		virtual bool is_closed(void) const;
	public:
		NET_INLINE bool has_impl(void) const;
		NET_INLINE std::shared_ptr<socket_impl>& get_impl(void);
		NET_INLINE void set_impl(const std::shared_ptr<socket_impl>& impl);
	private:
		void try_all_addresses(const std::string& dstname,
			const std::uint16_t& dstport,
			const std::shared_ptr<net_address>& localaddr,
			const std::uint16_t& localport);
		void startup_socket(std::shared_ptr<net_address> dstaddr,
			const std::uint16_t& dstport,
			const std::shared_ptr<net_address>& localaddr,
			const std::uint16_t& localport);
		void check_open_and_create(const bool& create, const int& family);
		void cache_local_address(void);
	private:
		socket(const socket&);
		socket& operator=(const socket&);
		socket& operator=(const socket&&);
	private:
		class socketbuf : public std::basic_streambuf<char, std::char_traits<char>>
		{
			std::shared_ptr<socket_impl> impl_;
			char obuffer_[1024];
			char ibuffer_[1024];
		public:
			socketbuf(const std::shared_ptr<socket_impl>& impl);
		public:
			virtual ~socketbuf(void);
		public:
			NET_INLINE char* obase(int n = 0) const;
			NET_INLINE char* oend(int n = 1024 - 1) const;
			NET_INLINE char* ibase(int n = 0) const;
			NET_INLINE char* iend(int n = 1024) const;
		protected:
			typedef std::basic_streambuf<char, std::char_traits<char>>::int_type int_type;
			virtual int_type overflow(int_type c = std::char_traits<char>::eof());
			virtual int_type underflow(void);
			virtual int sync(void);
			virtual std::streamsize showmanyc(void);
		public:
			void set_socket_impl(const std::shared_ptr<socket_impl>& impl);
		};
		socketbuf socketbuf_;
	public:
		/**
		* Returns an input/output stream buffer for this socket. 
		*/
		virtual std::basic_streambuf<char, std::char_traits<char>>* get_stream(void);
	};
}

#if defined(__NET_INLINE__)
#include "net.socket.inl"
#endif

#endif

