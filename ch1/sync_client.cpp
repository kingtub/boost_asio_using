#include<boost/asio.hpp>
#include<iostream>

using namespace std;
using namespace boost::asio;

int main(int argc, char **argv)
{
    try
	{
		io_service service;
		ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
		ip::tcp::socket sock(service);
		sock.connect(ep);

		sock.write_some(buffer("Message form client, Hello server!"));
        char data[512];
        size_t len = sock.read_some(buffer(data));
        cout<<"Server send:"<<string(data)<<endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
    return 0;
}
