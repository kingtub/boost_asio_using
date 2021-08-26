#include<boost/asio.hpp>
#include<boost/bind.hpp>

#include<iostream>
#include<string>
#include<thread>
#include<memory>

using namespace boost::asio;
using namespace std;

size_t read_complete(char * buf, const error_code & err, size_t bytes)
{
    if ( err) return 0;
    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    // 我们一个一个读取直到读到回车，不缓存
    return found ? 0 : 1;
}

void handle_connections()
{
    io_service service;
    ip::tcp::endpoint ep(ip::tcp::v4(), 8001);
    ip::tcp::acceptor acc(service, ep);
    char chs[1024];
    while(true) {
        ip::tcp::socket sock(service);
        acc.accept(sock);
        size_t n = read(sock, buffer(chs), boost::bind(read_complete, chs, _1, _2));
        sock.write_some(buffer(string(chs, n)));
        sock.close();
        // 因为我们是同步的，所以不需要调用*service.run()*。
    }
}

int main(int argc, char** argv)
{
    handle_connections();
    return 0;
}
