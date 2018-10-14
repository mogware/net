#ifndef __NET_EXCEPTIONS__
#define __NET_EXCEPTIONS__

#include <stdexcept>
#include <string>

namespace net
{
	class socket_exception : public std::logic_error
	{
	public:
		explicit socket_exception(const std::string& what)
			: std::logic_error(what) { }
	};
	class socket_timeout_exception : public socket_exception
	{
	public:
		explicit socket_timeout_exception(const std::string& what)
			: socket_exception(what) { }
	};

	class unknown_host_exception : public socket_exception
	{
	public:
		explicit unknown_host_exception(const std::string& what)
			: socket_exception(what) { }
	};
}

#endif


