#include <iostream>
#include "CinReader.h"
#include "CoutWriter.h"

#define _WIN32_WINNT 0x0A00 // For Windows 10
#include <boost/asio.hpp>

#include <thread>
#include "DND_lib/GameController.h"
#include "DND_lib/Player.h"
#include "CinReader.h"
#include "CoutWriter.h"
#include "GameServer.h"

using namespace std::chrono_literals;
using boost::asio::ip::tcp;
using ThreadContainer = std::vector<std::thread>;
static constexpr size_t MAX_USER_NUM = 4;

// Server Configuration
const int PORT = 4010;

std::string GetPublicIp(boost::asio::io_context& io_context)
{
    try
    {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(boost::asio::ip::host_name(), "");

        for (const auto& endpoint : endpoints)
        {
            if (endpoint.endpoint().address().is_v4())
            {
                return endpoint.endpoint().address().to_string();
            }
        }
        return "Unable to retrieve IP address.";
    }
    catch (std::exception& e)
    {
        return std::string("Error: ") + e.what();
    }
}

void NetServer(GameServer& a_gameServer, boost::asio::io_context& io_context)
{
    try
    {
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), PORT));
        std::cout << "Server started on port: " << PORT << std::endl;

        while (true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::make_shared<std::thread>([&a_gameServer, sock = std::move(socket)]() mutable
                {
                    try
                    {
                        NetWriter netWriter(sock);
                        NetReader netReader(sock);
                        netWriter.Write("Welcome! Please enter player name\n");
                        std::string userName = netReader.ReadLine();

                        // Create a Player instance
                        dnd_game::Player player{ userName, a_gameServer.GetDungeon().GetEntryRoom(), netWriter };
                        
                        // Register the player to the dungeon
                        a_gameServer.GetDungeon().RegisterPlayer(player);
                        netWriter.Write("\nWelcome " + userName + "!\n");

                        dnd_game::GameController gameController{ netWriter, netReader, a_gameServer.GetDungeon(), player };
                        // gameController.Start(); // Start game loop for player
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                })->detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception in server: " << e.what() << std::endl;
    }
}

int main() {
    try {
        // Create an io_context object for handling asynchronous operations
        boost::asio::io_context io_context;

        // Create a GameServer object
        GameServer server(io_context, PORT);


        // Get and display public IP
        std::string ip = GetPublicIp(io_context);
        std::cout << "Public IP Address: " << ip << std::endl;
        std::cout << "telnet " << ip << " " << PORT << std::endl;

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


/*

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

void NetServer(GameServer& a_gameServer)
{

    return;
}

int main()
{
    GameServer gameServer;

    ThreadContainer threads;

    for (size_t i = 0; i < MAX_USER_NUM; ++i)
    {
        threads.push_back(std::thread { [&gameServer]()
        {
            gameServer.WaitForClient();
        } });
    }

#ifdef _DEBUG
    //std::cout << "telnet 192.168.68.117 4010\n";
    std::wstring ip = GetPublicIp();
    std::wcout << "telnet " << ip << " 4010" << std::endl;
#endif // _DEBUG

    for (std::thread& t : threads)
    {
        //std::cout << "Waiting for other threads to die\n";
        t.join();
    }

    return 0;
}








// DND_Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*

#include <iostream>
#include "DND_lib/GameController.h"
#include "DND_lib/Player.h"
#include "CinReader.h"
#include "CoutWriter.h"
#include "GameServer.h"

//#include "DND_lib/Dungeon_mt.h"

int main()
{
    // Allocate communication stream of type ostream and istream
    game_server::CoutWriter coutW { std::cout };
    game_server::CinReader cinR { std::cin };

    // Dungeon_mt object will be reponsible ...
    dnd_game::Dungeon_mt dungeon_mt {};
    dnd_game::Player player { "tmp name", dnd_game::Dungeon_mt::ENTRY_ROOM, coutW };  // tmp player for debugging
    dnd_game::GameController gameExe { coutW, cinR, dungeon_mt, player };
    gameExe.Start();

}


*/
