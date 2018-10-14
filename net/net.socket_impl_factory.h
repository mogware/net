#ifndef __NET_SOCKET_IMPL_FACTORY__
#define __NET_SOCKET_IMPL_FACTORY__

#include "net.socket_impl.h"

namespace net
{
	struct socket_impl_factory
	{
		/**
		* Creates a new socket_impl instance.
		*/
		virtual std::shared_ptr<socket_impl> create_socket_impl(void) = 0;
		virtual ~socket_impl_factory(void) {}
	};
}

#endif


