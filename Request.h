#pragma     once

#include    <iostream>
#include    <thread>
#include    <chrono>

#ifdef      _WIN32
#define     _WIN32_WINN 0x0A00
#endif

#define     Log(x) std::cout << x
#define     ASIO_STANDALONE
#include    <asio.hpp>
#include    <asio/ts/buffer.hpp>
#include    <asio/ts/internet.hpp>
#include    <asio/ip/tcp.hpp>
#include    <asio/io_context.hpp>

class Request
{
    std::string m_host = "";
    unsigned int m_port = 80;
    std::string m_path = "/";

    asio::error_code m_ec;
    asio::io_context m_ctx;

    asio::ip::tcp::endpoint* m_ep;
    asio::ip::tcp::socket* m_sock;

    float m_time = -1.0f;
    bool closed = false;

public:

    Request(const std::string& host, unsigned int port, const std::string& path);
    ~Request();

    bool Connect();
    std::string Send();
    float getTime() const;
    void Terminate();
};