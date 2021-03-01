#include "Request.h"

Request::Request(const std::string& host, unsigned int port, const std::string& path)
    : m_host(host), m_port(port), m_path(path)
{
    this->m_sock = new asio::ip::tcp::socket(this->m_ctx);
    this->m_ep = new asio::ip::tcp::endpoint(asio::ip::make_address(this->m_host, this->m_ec), this->m_port);
}

bool Request::Connect()
{
    this->m_sock->connect(*(this->m_ep), this->m_ec);
    if (!(this->m_ec))
        return true;
    else
        return false;
}

std::string Request::Send()
{
    if (this->m_sock->is_open())
    {
        std::string reqData = "GET ";
        reqData.append(this->m_path);
        reqData.append(" HTTP/1.1\r\n");
        reqData.append("Host: ");
        reqData.append(this->m_host);
        reqData.append("\r\n");
        reqData.append("Connection: close\r\n\r\n");

        this->m_sock->write_some(asio::buffer(reqData, reqData.size()), this->m_ec);

        auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;


        while (duration < std::chrono::seconds(15))
        {
            end = std::chrono::high_resolution_clock::now();
            duration = end - start;

            if (this->m_sock->available() > 0)
                break;
        }

        if (this->m_sock->available() == 0)
        {
            return (std::string)"[REQUSET TIMEOUT]: Did not receive a response for the sent request within 15 seconds.";
        }
        else
        {
            std::vector<char> responseData(this->m_sock->available());
            this->m_sock->read_some(asio::buffer(responseData.data(), responseData.size()), this->m_ec);

            std::string response(responseData.begin(), responseData.end());

            this->m_time = duration.count();

            return response;
        }
    }
    else
        return (std::string)"[ERROR]: Cannot send a request; socket is not open";
}

float Request::getTime() const
{
    return this->m_time;
}

void Request::Terminate()
{
    this->m_sock->close();
    delete m_sock;
    delete m_ep;
    this->closed = true;
}

Request::~Request()
{
    if (!this->closed)
        this->Terminate();
}