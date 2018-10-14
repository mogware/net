#ifndef __NET_DEFAULT_SERVER_SOCKET_IMPL__
#define __NET_DEFAULT_SERVER_SOCKET_IMPL__

#include "net.default_socket_impl.h"
#include "net.socket_impl.h"

namespace net
{
	class default_server_socket_impl : public default_socket_impl
	{
	public:
		default_server_socket_impl(void);
		default_server_socket_impl(const sio::socket_t& sock);
	public:
		virtual ~default_server_socket_impl(void);
	public:
		static std::shared_ptr<socket_impl> of(const sio::socket_t& sock);
	public:
		void create(const int& family);
	private:
		default_server_socket_impl(const default_server_socket_impl&);
		default_server_socket_impl& operator=(const default_server_socket_impl&);
		default_server_socket_impl& operator=(const default_server_socket_impl&&);
	};
}

#endif
