#include<boost/asio.hpp>
#include<boost/bind.hpp>

#include<iostream>
#include<string>
#include<thread>
#include<memory>

using namespace boost::asio;
using namespace std;

ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
io_service service;
size_t read_complete(char * buf, const error_code & err, size_t bytes)
{
    if ( err) return 0;
    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    // 我们一个一个读取直到读到回车，不缓存
    return found ? 0 : 1;
}

void sync_echo(string msg)
{
    // 注意：因为我们是同步的，所以不需要调用*service.run()*。
    string str = msg + "\n";
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer(str));
    char buf[1024];
    size_t n = read(sock, buffer(buf), boost::bind(read_complete, buf, _1, _2));
    string ret(buf, n-1);
    cout<<"server echoed our "<<ret<<": "<<(msg == ret?"OK":"FAIL")<<endl;
    sock.close();
}

int main(int argc, char** argv)
{
    const char * msgs[5] = {"aaa", "bbb", "ccc", "ddd", "eee"};
    shared_ptr<thread> st[5];
    for(int i = 0; i < 5; i++) {
        st[i] = shared_ptr<thread>(new thread(sync_echo, msgs[i]));
        st[i]->join();
    }

    /*for(int i = 0; i < 5; i++) {
        st[i]->join();
    }*/
    return 0;
}
