#include<boost/asio.hpp>
#include<iostream>
#include<memory>
#include<thread>

using namespace std;
using namespace boost::asio;

void client_session(shared_ptr<ip::tcp::socket> sock) {
    try
    {
        cout<<"Receive a connection"<<endl;
        char data[512];
        while ( true) {
            if(sock->available() == 0) {
                break;
            }
            size_t len = sock->read_some(buffer(data));
            cout<<"Server receive:"<<string(data)<<endl;
            write(*sock, buffer("ok", 2));
        }
    }
    catch (std::exception &e)
	{
		std::cout <<"Exception raised, msg:"<<e.what() << std::endl;
	}
}

int main(int argc, char **argv)
{

    io_service service;
    ip::tcp::endpoint ep(ip::tcp::v4(), 2001); // listen on 2001
    ip::tcp::acceptor acc(service, ep);
    while ( true) {
        shared_ptr<ip::tcp::socket> sock(new ip::tcp::socket(service));
        acc.accept(*sock);
        thread t(client_session, sock);
        t.detach();
    }

    return 0;
}
