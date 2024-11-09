#include "NetWriter.h"
#include <boost/asio/write.hpp>
#include <iostream>


NetWriter::NetWriter(boost::asio::ip::tcp::socket& a_socket)
    : m_socket(a_socket)
{
    // Ctor
}

void NetWriter::Write(const std::string& a_str)
{
    try
    {
        boost::asio::write(m_socket, boost::asio::buffer(a_str));
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Error writing to socket: " << e.what() << std::endl;
    }
}

void NetWriter::Write(const char a_char)
{
    try
    {
        boost::asio::write(m_socket, boost::asio::buffer(&a_char, 1));
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Error writing to socket: " << e.what() << std::endl;
    }
}

void NetWriter::Endl()
{
    Write("\r\n");

}
