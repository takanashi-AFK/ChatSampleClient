#define  WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h> 
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include<string>

using namespace std;

const int portNum{ 8888 };
//127.0.0.2


int WSAInit();
SOCKET UDPSocketInit();
const int MESSAGELENGTH = 1024;

int main()
{
	WSAInit();	// WinSock初期化
	SOCKET udpSocket = UDPSocketInit();	// ソケット作成
		/*
		   サーバのソケットアドレス情報をセット
		*/

		// 接続先サーバのIPアドレスを入力させる
	std::string serverIpAddress;
	std::cout << "Input Server IPv4 address :";
	std::cin >> serverIpAddress;

	SOCKADDR_IN fromAddr;	// 送信先ソケットアドレス情報を格納する領域
	int tolen = sizeof(fromAddr);
	memset(&fromAddr, 0, sizeof(fromAddr));
	fromAddr.sin_family = AF_INET;                // IPv4アドレス
	fromAddr.sin_port = htons(portNum);            // サーバのポート番号
	inet_pton(AF_INET, serverIpAddress.c_str(), &fromAddr.sin_addr.s_addr);      // サーバのIPアドレス

		while (true)
		{
			char message[MESSAGELENGTH];	
			std::cout << "Input message:";
			std::cin >> message;

			int ret = sendto(udpSocket, message, MESSAGELENGTH, 0, (SOCKADDR*)&fromAddr, tolen);
			if (ret == SOCKET_ERROR)
			{
				cout << WSAGetLastError();
			}


			ret = recvfrom(udpSocket, message, sizeof(message) - 1, 0, (SOCKADDR*)&fromAddr, &tolen);	// 受信
			cout << message << endl;
			/*
			出力
			*/
		}
		closesocket(udpSocket);		// ソケット破棄
		WSACleanup();		// WinSock終了処理
		return 0;
}

int WSAInit()
{
	WSADATA	wsaData;
	// WinSock2.2 初期化処理		
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "WSAStartup is completed" << endl;
	return 1;
}

SOCKET UDPSocketInit()
{
	// UDPリスンソケットの作成
	SOCKET	sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "socket is completed" << endl;;
	return sock;
}
