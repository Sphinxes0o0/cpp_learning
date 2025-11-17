#include <asio.hpp>
#include <iostream>
#include <string>
#include <memory>

using namespace std;

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
    HttpConnection(asio::io_context& io_context) : socket_(io_context) { }

    asio::ip::tcp::socket& socket() { return socket_; }

    void start() {
        auto p = shared_from_this();
        asio::async_read_until(socket_, asio::dynamic_buffer(request_), "\r\n\r\n",
            [p, this](const asio::error_code& ec, size_t len) {
                if (ec) {
                    cout<<"recv err:"<<err.message()<<"\n";
                    return;
                }

                string first_line = request_.substr(0, request_.find("\r\n"));//
                cout << "Received request: " << first_line << endl;

                char str[] = "HTTP/1.0 200 OK\r\n\r\n"
                    "<html>hello from http server</html>";
                asio::async_write(socket_, asio::buffer(str), 
                    [p, this](const asio::error_code& err, size_t len) {
                        socket_.close();
                    }
                );
            }
        );
    }
private:
    asio::ip::tcp::socket socket_;
    string request_;
}

class HttpServer {
public:
    HttpServer(asio::io_context& io_context, asio::ip::tcp::endpoint endpoint)
        : io_(io), acceptor_(io, ep) { }

    void start() {
        auto p = std::make_shared<HttpConnection>(io_);
        acceptor_.async_accept(p->socket(),
            [this, p](const asio::error_code& ec) {
                if (ec) {
                    cout<<"accept err:"<<ec.message()<<"\n";
                } else {
                    p->start();
                }
                start();
            }
        );
    }
private:
    asio::io_context& io_;
    asio::ip::tcp::acceptor acceptor_;
};

int main() {
    std::cout << "Starting ASIO HTTP client example..." << std::endl;

    return 0;
}