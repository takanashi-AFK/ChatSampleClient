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
	WSAInit();	// WinSock������
	SOCKET udpSocket = UDPSocketInit();	// �\�P�b�g�쐬
		/*
		   �T�[�o�̃\�P�b�g�A�h���X�����Z�b�g
		*/

		// �ڑ���T�[�o��IP�A�h���X����͂�����
	std::string serverIpAddress;
	std::cout << "Input Server IPv4 address :";
	std::cin >> serverIpAddress;

	SOCKADDR_IN fromAddr;	// ���M��\�P�b�g�A�h���X�����i�[����̈�
	int tolen = sizeof(fromAddr);
	memset(&fromAddr, 0, sizeof(fromAddr));
	fromAddr.sin_family = AF_INET;                // IPv4�A�h���X
	fromAddr.sin_port = htons(portNum);            // �T�[�o�̃|�[�g�ԍ�
	inet_pton(AF_INET, serverIpAddress.c_str(), &fromAddr.sin_addr.s_addr);      // �T�[�o��IP�A�h���X

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


			ret = recvfrom(udpSocket, message, sizeof(message) - 1, 0, (SOCKADDR*)&fromAddr, &tolen);	// ��M
			cout << message << endl;
			/*
			�o��
			*/
		}
		closesocket(udpSocket);		// �\�P�b�g�j��
		WSACleanup();		// WinSock�I������
		return 0;
}

int WSAInit()
{
	WSADATA	wsaData;
	// WinSock2.2 ����������		
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
	// UDP���X���\�P�b�g�̍쐬
	SOCKET	sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cout << WSAGetLastError();
		return -1;
	}
	cout << "socket is completed" << endl;;
	return sock;
}
