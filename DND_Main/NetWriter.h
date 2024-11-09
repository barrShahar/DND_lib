#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // For Windows 10
#endif

#pragma once
#include "DND_lib/Writer.h"
#include <boost/asio.hpp>
#include <memory>
#include <string>

class NetWriter : public dnd_game::Writer
{
public:
    explicit NetWriter(boost::asio::ip::tcp::socket& a_socket);
    NetWriter(const NetWriter& a_other) = delete;
    NetWriter& operator=(const NetWriter& a_other) = delete;
    virtual ~NetWriter() = default;

    // Implement virtual functions from Writer
    virtual void Write(const std::string& a_str) override;
    virtual void Write(const char a_char) override;
    virtual void Endl() override;

private:
    boost::asio::ip::tcp::socket& m_socket;
};
