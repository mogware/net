# Class library for implementing networking applications

## Usage

Example of simple client/server usage

```c
void server(const bool& ipv6)
{
	sio::socket_init();
	net::server_socket sock(7777, 5, ipv6);
	printf("server waiting for client connection on port 7777\n");
	while (true)
	{
		std::shared_ptr<net::socket> client = sock.accept();

		std::shared_ptr<net::net_address> addr = client->get_address();
		std::uint16_t port = client->get_port();
		printf("connection from %s:%d\n", addr->to_string().c_str(), port);

		std::istream is(client->get_stream());
		std::string response;
		if (std::getline(is, response))
			printf("received data: %s\n", response.c_str());

		std::ostream os(client->get_stream());
		os.write("ok", 2);
		os.flush();

		client->close();
	}
	sock.close();
	sio::socket_term();
}
```

```c
void client(const bool& ipv6)
{
	sio::socket_init();
	net::socket sock;
	net::socket_address raddr(net::net_address::get_local_host(ipv6), 7777);
	sock.connect(raddr, 5000);
	std::ostream os(sock.get_stream());
	os.write("hello world", 11);
	os.flush();
	sock.shutdown_output();
	std::istream is(sock.get_stream());
	std::string response;
	if (std::getline(is, response))
		printf("received response: %s\n", response.c_str());
	sock.close();
	sio::socket_term();
}
```
