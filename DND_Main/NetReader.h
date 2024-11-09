#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // For Windows 10
#endif

#pragma once
#include "DND_lib/Reader.h"
#include <boost/asio.hpp>

class NetReader : public dnd_game::Reader
{
public:
    explicit NetReader(boost::asio::ip::tcp::socket& a_socket);
    NetReader(const NetReader& a_other) = delete;
    NetReader& operator=(const NetReader& a_other) = delete;
    ~NetReader() = default;

    std::string ReadLine() const override;

private:
    boost::asio::ip::tcp::socket& m_socket;
};