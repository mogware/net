#ifndef __NET_NET6_ADDRESS__
#define __NET_NET6_ADDRESS__

#include "net.net_address.h"

namespace net
{
	class net6_address : public net_address
	{
		std::uint8_t address_[16];
	public:
		static std::shared_ptr<net_address> ANY;
		static std::shared_ptr<net_address> LOOPBACK;
	public:
		/**
		* Constructs a net6_address representing the address and
		* hostname
		*/
		net6_address(const std::uint8_t* address, const int& length,
			const std::string& hostname);
		net6_address(const std::vector<std::uint8_t>& address,
			const std::string& hostname);
	public:
		virtual ~net6_address(void);
	public:
		/**
		* Constructs an IPv6 address according to the given hostname, 
		* and address.
		*/
		static std::shared_ptr<net_address> of(
			const std::vector<std::uint8_t>& address,
			const std::string& hostname = "");
	public:
		/**
		* Utility routine to check if the address is an IP multicast
		* address.
		*/
		NET_INLINE bool is_multicast_address(void) const;

		/**
		* Utility routine to check if the address in a wildcard address.
		*/
		NET_INLINE bool is_any_local_address(void) const;

		/**
		* Utility routine to check if the address is a loopback address.
		*/
		NET_INLINE bool is_loopback_address(void) const;

		/**
		* Utility routine to check if the address is an link local address.
		*/
		NET_INLINE bool is_link_local_address(void) const;

		/**
		* Utility routine to check if the address is a site local address.
		*/
		NET_INLINE bool is_site_local_address(void) const;

		/**
		* Returns the raw IP address of this address object. The result is
		* in network byte order: the highest order byte of the address is in
		* get_address()[0].
		*/
		NET_INLINE std::vector<std::uint8_t> get_address(void) const;
	public:
		/**
		* Returns hostname in a protocol-independent manner.
		*/
		std::string get_name_info(const int& flags) const;
	public:
		std::string to_string(void) const;
	};
}

#if defined(__NET_INLINE__)
#include "net.net6_address.inl"
#endif

#endif


