#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/asio/error.hpp>
#include<iostream>
#include<memory>
#include<thread>

using namespace std;
using namespace boost::asio;

void handle_accept(shared_ptr<ip::tcp::socket> sock, const boost::system::error_code &err);
void start_accept(shared_ptr<ip::tcp::socket> sock);

io_service service;
ip::tcp::endpoint ep( ip::tcp::v4(), 2001); // 监听端口2001
ip::tcp::acceptor acc(service, ep);

int main(int argc, char** argv)
{
    shared_ptr<ip::tcp::socket> sock(new ip::tcp::socket(service));
    start_accept(sock);
    service.run();

    return 0;
}

void start_accept(shared_ptr<ip::tcp::socket> sock) {
    acc.async_accept(*sock, boost::bind(handle_accept, sock, _1) );
}

void handle_accept(shared_ptr<ip::tcp::socket> sock, const boost::system::error_code &err) {
    if (err) return;
    // 从这里开始, 你可以从socket读取或者写入
    auto action = [](shared_ptr<ip::tcp::socket> sock, const boost::system::error_code &err) {
		    try
            {
                cout<<"Receive a connection"<<endl;
                while ( true) {
                    char data[512];
                    size_t len = sock->read_some(buffer(data));
                    if ( len > 0)
                        write(*sock, buffer("ok", 2));
                }
            }
            catch (std::exception &e)
            {
                std::cout <<"Exception raised, msg:"<<e.what() << std::endl;
            }
	};
	thread t(action, sock, err);
	t.detach();


	shared_ptr<ip::tcp::socket> sock1(new ip::tcp::socket(service));
	start_accept(sock1);
}
