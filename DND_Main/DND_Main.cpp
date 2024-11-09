
#include <iostream>
#include "DND_lib/GameController.h"
#include "DND_lib/Player.h"
#include "CinReader.h"
#include "CoutWriter.h"
#include "GameServer.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std::chrono_literals;
using ThreadContainer = std::vector<std::thread>;
static constexpr size_t MAX_USER_NUM = 4;
#include <codecvt>  // For std::wstring_convert

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // For Windows 10
#endif
#include <boost/asio.hpp>
#include <thread>
#include "GameServer.h"

static const int PORT = 4010;
static const int MAX_THREAD_NUMBER = 4;
std::wstring GetPublicIp();

int main() {
    try {
        // Create an io_context object for handling asynchronous operations
        boost::asio::io_context io_context;

        // Create a GameServer object
        GameServer server(io_context, PORT);

#ifdef _DEBUG
        //std::cout << "telnet 192.168.68.117 4010\n";
        std::wstring ip = GetPublicIp();
        std::wcout << "telnet " << ip << " 4010" << std::endl;
#endif // _DEBUG

        // Create threads and run io_context in each
        std::vector<std::thread> thread_container;
        for (int i = 0; i < MAX_THREAD_NUMBER; ++i) {
            thread_container.emplace_back([&io_context, &server]() {
                // Start accepting client connections
                server.WaitForClient();
                io_context.run();
                });
        }

        // Join threads to main thread
        for (auto& thread : thread_container) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}



std::wstring GetPublicIp()
{
    WSADATA wsaData;
    char hostname[256];
    struct addrinfo *result = nullptr, hints;
    WCHAR ipStringBuffer[46];  // To hold the IP address in string form (IPv4 or IPv6)

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
        return L"WSAStartup failed.";  // Return wide string literal
    }

    // Get the local hostname
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
    {
        std::cerr << "Error: Could not retrieve hostname." << std::endl;
        WSACleanup();
        return L"Error: Could not retrieve hostname.";  // Return wide string literal
    }

    //std::cout << "Hostname: " << hostname << std::endl;

    // Set up the hints structure for getaddrinfo()
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // Specify IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Get the address info
    if (getaddrinfo(hostname, nullptr, &hints, &result) != 0)
    {
        std::cerr << "Error: Could not retrieve IP address." << std::endl;
        WSACleanup();
        return L"Error: Could not retrieve IP address.";  // Return wide string literal
    }

    // Convert the binary IP address to a string using InetNtop
    struct sockaddr_in *sockaddr_ipv4 = ( struct sockaddr_in * ) result->ai_addr;
    if (InetNtop(AF_INET, &sockaddr_ipv4->sin_addr, ipStringBuffer, sizeof(ipStringBuffer)) == nullptr)
    {
        std::cerr << "Error: Could not convert IP address." << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return L"Error: Could not convert IP address.";  // Return wide string literal
    }

    // Set the locale to support wide characters
    std::wcout.imbue(std::locale(""));

    // Use std::wcout to print wide characters
    std::wcout << L"IP Address: " << ipStringBuffer << std::endl;

    // Cleanup
    freeaddrinfo(result);
    WSACleanup();

    // Return the IP address as a std::wstring
    return std::wstring(ipStringBuffer);
}
