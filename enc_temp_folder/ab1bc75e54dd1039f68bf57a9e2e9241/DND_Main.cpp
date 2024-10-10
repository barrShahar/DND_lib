
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

    std::cout << "Hostname: " << hostname << std::endl;

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
	std::wcout << "telnet " << GetPublicIp() << std::endl;
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
	dnd_game::Player player { "tmp name", dnd_game::Dungeon_mt::entryRoom, coutW };  // tmp player for debugging
	dnd_game::GameController gameExe { coutW, cinR, dungeon_mt, player };
	gameExe.Start();

}


*/