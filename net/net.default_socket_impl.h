#ifndef __NET_DEFAULT_SOCKET_IMPL__
#define __NET_DEFAULT_SOCKET_IMPL__

#include <string>
#include <memory>

#include "net.net_address.h"
#include "net.socket_address.h"
#include "net.socket_impl.h"

namespace net
{
	class default_socket_impl : public socket_impl
	{
		bool shutdown_input_;
	public:
		default_socket_impl(void);
		default_socket_impl(const sio::socket_t& sock);
		default_socket_impl(const sio::socket_t& sock, const std::uint16_t& localport,
			const std::shared_ptr<net_address>& addr, const std::uint16_t& port);
	public:
		virtual ~default_socket_impl(void);
	public:
		static std::shared_ptr<socket_impl> of(const sio::socket_t& sock);
		static std::shared_ptr<socket_impl> of(const sio::socket_t& sock, const std::uint16_t& localport,
			const std::shared_ptr<net_address>& addr, const std::uint16_t& port);
	public:
		void accept(std::shared_ptr<socket_impl>& new_socket);
		int available(void) const;
		void bind(const std::shared_ptr<net_address>& addr,
			const std::uint16_t& port);
		void close(void);
		void connect(const std::string& hostname, const std::uint16_t& port);
		void connect(const std::shared_ptr<net_address>& addr,
			const std::uint16_t& port);
		void connect(const std::shared_ptr<net_address>& addr,
			const std::uint16_t& port, const int& timeout);
		void create(const int& family);
		int read(std::uint8_t* buffer, const int& nbytes);
		void write(const std::uint8_t* buffer, const int& nbytes);
		bool supports_urgent_data(void) const;
		void send_urgent_data(const int& value);
		void listen(const int& backlog);
		void shutdown_input();
		void shutdown_output();
	public:
		bool get_option_bool(const int& id);
		void set_option_bool(const int& id, const bool& val);
		int get_option_int(const int& id);
		void set_option_int(const int& id, const int& val);
	private:
		static sio::socket_t accept(const sio::socket_t& sockfd, socket_address& addr,
			const int& family);
		static void bind(const sio::socket_t& sockfd, const std::shared_ptr<net_address>& addr,
			const std::uint16_t& port);
		static void connect(const sio::socket_t& sockfd, const std::shared_ptr<net_address>& addr,
			const std::uint16_t& port, const int& timeout);
		static void connect(const sio::socket_t& sockfd, const sio::sockaddr_t* addr,
			const int& len, const int& timeout);
		static bool is_connected(const sio::socket_t& sockfd, const sio::sockaddr_t* addr,
			const int& timeout, const int& remaining);
		static int recv(const sio::socket_t& sockfd, std::uint8_t* buffer,
			const int& bytes, const int& flags);
		static void send(const sio::socket_t& sockfd, const std::uint8_t* buffer,
			const int& bytes, const int& flags);
		static std::uint16_t get_socket_local_port(const sio::socket_t& sockfd,
			const int& family);
		static std::shared_ptr<net_address> get_socket_local_address(
			const sio::socket_t& sockfd, const int& family);
		static std::uint64_t now_millis(void);
	private:
		default_socket_impl(const default_socket_impl&);
		default_socket_impl& operator=(const default_socket_impl&);
		default_socket_impl& operator=(const default_socket_impl&&);
	};
}

#endif
