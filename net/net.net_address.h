#ifndef __NET_NET_ADDRESS__
#define __NET_NET_ADDRESS__

#include <memory>
#include <string>
#include <vector>

#include "sio.h"
#include "net.config.h"

namespace net
{
	class net_address
	{
		int family_;
		std::string hostname_;
		std::string hostaddr_;
	public:
		/**
		* Constructs an net_address.
		* Note: this constructor is for subclasses only.
		*/
		net_address(const int& family, const std::string& hostname = "");
	public:
		virtual ~net_address(void);
	public:
		/**
		* Returns a net_address corresponding to the given network-order
		* bytes address
		* For an IPv4 address, the byte array must be of length 4. For IPv6,
		* the byte array must be of length 16.
		*/
		NET_INLINE static std::shared_ptr<net_address> of(
			const std::vector<std::uint8_t>& address,
			const std::string& hostname = "");
	public:
		/**
		* Returns the address of a host according to the given host string name.
		* The host string may be either a machine name or a dotted string IP
		* address.
		*/
		NET_INLINE static std::shared_ptr<net_address> of(const std::string& hostname,
			const bool& prefer_ipv6 = false);

		/**
		* Returns a net_address for the local host if possible, or the
		* loopback address otherwise.
		*/
		NET_INLINE static std::shared_ptr<net_address> get_local_host(
			const bool& prefer_ipv6 = false);
	public:
		/**
		* Gets all IP addresses associated with the given host identified
		* by name or literal IP address.
		*/
		static std::vector<std::shared_ptr<net_address>> get_all_by_name(
			const std::string& hostname = "");
	public:
		/**
		* Returns whether this address is a multicast address or not.
		*/
		virtual bool is_multicast_address(void) const = 0;

		/**
		* Returns whether this is the IPv6 unspecified wildcard address or
		* the IPv4 "any" address.
		*/
		virtual bool is_any_local_address(void) const = 0;

		/**
		* Returns whether this address is a loopback address or not.
		*/
		virtual bool is_loopback_address(void) const = 0;

		/**
		* Returns whether this address is a link-local address or not.
		*/
		virtual bool is_link_local_address(void) const = 0;

		/**
		* Returns whether this address is a site-local address or not.
		*/
		virtual bool is_site_local_address(void) const = 0;

		/**
		* Returns the IP address represented by this net_address instance
		* as a byte vector. The elements are in network order (the highest order
		* address byte is in the zeroth element).
		*/
		virtual std::vector<std::uint8_t> get_address(void) const = 0;

		/**
		* Returns hostname in a protocol-independent manner.
		*/
		virtual std::string get_name_info(const int& flags) const = 0;
	public:
		NET_INLINE int get_family(void) const;
		NET_INLINE std::string to_string(void);
	public:
		/**
		* Returns the host name corresponding to this IP address. This may or
		* may not be a fully-qualified name.
		*/
		NET_INLINE const std::string& get_host_name(void) const;

		/**
		* Returns the numeric representation of this IP address
		* (such as "127.0.0.1").
		*/
		NET_INLINE const std::string& get_host_address(void) const;
	public:
		/**
		* Returns true if the string is a valid numeric IPv4 or IPv6 address.
		*/
		NET_INLINE static bool is_numeric(const std::string& address);

		/**
		* Returns a net_address corresponding to the given numeric address.
		*/
		static std::shared_ptr<net_address>
			parse_numeric_address(const std::string& address);

		/**
		* Resolves a hostname to its IP addresses.
		*/
		static std::vector<std::shared_ptr<net_address>>
			lookup_host_by_name(const std::string& hostname);
	private:
		const std::string& init_host_name(void);
		const std::string& init_host_address(void);
	private:
		static std::shared_ptr<net_address> get_by_address(
			const std::vector<std::uint8_t>& address,
			const std::string& hostname = "");
		static bool is_ipv4mapped_address(
			const std::vector<std::uint8_t>& address);
		static bool is_ipv4mapped_address(
			const std::uint8_t* address);
		static std::shared_ptr<net_address>
			get_numeric_address(const std::string& address);
		static bool disallow_deprecated_formats(
			const std::string& hostname, const std::shared_ptr<net_address>& addr);
		static std::vector<std::shared_ptr<net_address>>
			result_to_net_adresses(const sio::addrinfo_t* result);
		static std::shared_ptr<net_address> find_best_match(
			const std::vector<std::shared_ptr<net_address>>& addrs, const int& family);
		static std::vector<std::shared_ptr<net_address>>
			loopback_addresses(void);
	private:
		NET_INLINE static std::vector<std::uint8_t> ipv4mapped_to_ipv4(
			const std::vector<std::uint8_t>& address);
	};
}

#if defined(__NET_INLINE__)
#include "net.net_address.inl"
#endif

#endif


