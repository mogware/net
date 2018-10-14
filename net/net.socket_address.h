#ifndef __NET_SOCKET_ADDRESS__
#define __NET_SOCKET_ADDRESS__

#include <memory>
#include <string>
#include <sstream>

#include "net.exceptions.h"
#include "net.net_address.h"
#include "net.net4_address.h"

namespace net
{
	struct socket_address
	{
		std::shared_ptr<net_address> addr_;
		std::string hostname_;
		std::uint16_t port_;
	public:
		/**
		* Creates a socket address where the IP address is the wildcard
		* address and the port number a specified value.
		*/
		socket_address(const std::uint16_t& port = 0);

		/**
		* Creates a socket address from an IP address and a port number.
		*/
		socket_address(std::shared_ptr<net_address> addr,
			const std::uint16_t& port = 0);

		/**
		* Creates a socket address from a hostname and a port number.
		*/
		socket_address(const std::string& hostname, const std::uint16_t& port,
			const bool& prefer_ipv6 = false);
	public:
		virtual ~socket_address(void);
	public:
		/**
		* Gets the port number.
		*/
		NET_INLINE const uint16_t get_port(void) const;

		/**
		* Sets the port number.
		*/
		NET_INLINE void set_port(const uint16_t& port);

		/**
		* Gets the address
		*/
		NET_INLINE std::shared_ptr<net_address> get_address(void) const;

		/**
		* Sets the address
		*/
		NET_INLINE void set_address(const std::shared_ptr<net_address>& addr);

		/**
		* Gets the hostname.
		*/
		NET_INLINE const std::string& get_host_name(void) const;

		/**
		*Constructs a string representation of this address.
		*/
		NET_INLINE const std::string to_string(void) const;
	};
}

#if defined(__NET_INLINE__)
#include "net.socket_address.inl"
#endif

#endif

