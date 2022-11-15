#pragma once

// this is a version to support the linux env 

// For TCPIP
#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1  // 系統要求
#pragma comment(lib,"ws2_32.lib")
#include <winsock2.h>
#endif // _WIN32

#ifdef __unix
#include <sys/socket.h>
#endif // __unix

#include <iostream>

inline int Start_UDP_Server(SOCKET* SSock, int Port) {
	//SOCKET  SSock;    // 通道連線代碼
	// 1.變數宣告
	sockaddr_in Addr; // IP+Port+Protocol

	// 2.設定Winsock/sock
	int err = 0;
	
#ifdef _WINSOCK2API_
	WSADATA Wsa;      // Winsock 參數
	if (err = WSAStartup(0x202, &Wsa) != 0) { // 啟動Winsock
		return err;
	}
#endif // _WINSOCK2API_

	*SSock = socket(AF_INET, SOCK_DGRAM, 0); // 開啟UDP通道

	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(Port);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 3.啟動UDP Server
	bind(*SSock, (sockaddr*)&Addr, sizeof(sockaddr));
	return 0;
}

inline int Start_UDP_Client(SOCKET* CSock, sockaddr* CAddr, const char* IP, int Port) {
	// 1.變數宣告
	//SOCKET  SSock;    // 通道連線代碼
	sockaddr_in* Addr; // IP+Port+Protocol

	Addr = (sockaddr_in*)CAddr;
	// 2.設定Winsock/sock
	
	int err = 0;
#ifdef _WINSOCK2API_
	WSADATA Wsa;      // Winsock 參數
	if (err = WSAStartup(0x202, &Wsa) < 0) { // 啟動Winsock
		return err;
	}
#endif // _WINSOCK2API_

	*CSock = socket(AF_INET, SOCK_DGRAM, 0); // 開啟UDP通道

	Addr->sin_family = AF_INET;
	Addr->sin_port = htons(Port);
	Addr->sin_addr.s_addr = inet_addr(IP);

	return 0;
}

// =======================================================================
// ==================  啟動 TCP Server(使用thread接受連線與接收資料)   ===
// =======================================================================
#include <process.h>  // Project => Setting => C/C++ => Category=Code Generation => Use Runtime Library=Debug Multithreaded
SOCKET   g_S_Socket, g_S_Socket1;
DWORD    Thread_ID11;
// 執行緒子程式
VOID Thread_Func11(PVOID pvoid) {
	char S1[5000];
	int  i, Len = sizeof(sockaddr);
	sockaddr_in Addr;

	while (1) {
		g_S_Socket1 = accept(g_S_Socket, (sockaddr*)&Addr, &Len);
		do {
			i = recv(g_S_Socket1, S1, sizeof(S1) - 1, 0);
			if (i > 0) {
				S1[i] = 0;
				printf("Recv(%s) = %s\r\n", inet_ntoa(Addr.sin_addr), S1);
			}
		} while (i > 0);
	}
}

int  Start_TCP_Server(SOCKET* psockfd, WORD Port) {
	
	int      err;
	struct   sockaddr_in  tcpserver;

	// 1. 開啟 TCP Server
#ifdef _WINSOCK2API_
	WSADATA  wsadata;
	if ((err = WSAStartup(0x202, &wsadata)) != 0) return -1;
#endif // _WINSOCK2API_

	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;

	tcpserver.sin_family = AF_INET;
	tcpserver.sin_port = htons(Port);
	tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(*psockfd, (struct sockaddr*)&tcpserver, sizeof(tcpserver)) < 0) return -1;
	if ((err = listen(*psockfd, SOMAXCONN)) < 0) return -1;
	g_S_Socket = *psockfd;

	// 2. 開啟子執行緒
	// Thread_ID11 = _beginthread(Thread_Func11, 0, NULL);

	return 0;
}

// =======================================================================
// ==================  啟動 TCP Client   =================================
// =======================================================================
SOCKET   g_C_Socket; // Global Variable
DWORD    Thread_ID22;
// 執行緒子程式
VOID Thread_Func22(PVOID pvoid) {
	char S1[2000];
	int  i, j, Len = sizeof(sockaddr);
	sockaddr_in Addr;
	BOOL Flag = 0;
	int  k, k1, k2, k3, Beg, End, Fa2 = 0;
	char S2[2000];
	while (1) {
		i = recv(g_C_Socket, S1, sizeof(S1) - 1, 0);

		if (i > 0) {
			S1[i] = 0;
			// 去除 顏色控制碼(Remove Color Control Code) 
			for (j = 0; j < i; j++) {
				if (S1[j] == 27) { // ESC
					Flag = true;
					Beg = j + 1;
				}

				if (!Flag) {
					printf("%c", S1[j]);
				}

				if ((Flag) && (S1[j] == 'm')) {
					Flag = false;
					End = j;
					memcpy(S2, &S1[Beg], static_cast<size_t>(End) - Beg + 1);
					S2[End - Beg + 1] = 0;
					k = sscanf_s(S2, "[%d;%d;%dm", &k1, &k2, &k3);
					//if ((k==1)&&(k1==41))
				}
			}
			//printf("%s",S1);
			//for (i=0;i<strlen(S1);i++)
			//{   if      (S1[i]==0x1b) Flag=1;
			//    else if (S1[i]=='m')  Flag=0;
			//    else if (Flag==0) printf("%c",S1[i]);
		} //}   
	}
}

inline int Start_TCP_Client(SOCKET* psockfd, WORD R_Port, const char* IP) {
	
	int      err;
	sockaddr_in  tcpclient;

#ifdef _WINSOCK2API_
	WSADATA  wsadata;
	if ((err = WSAStartup(0x202, &wsadata)) != 0) return -1;
#endif // !_WINSOCK2API_

	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;

	tcpclient.sin_family = AF_INET;
	tcpclient.sin_port = htons(R_Port);
	tcpclient.sin_addr.s_addr = inet_addr(IP);

	if (err = connect(*psockfd, (sockaddr*)&tcpclient, sizeof(tcpclient)) < 0) return -1;

	g_C_Socket = *psockfd;

	// 2. 開啟子執行緒
	Thread_ID22 = _beginthread(Thread_Func22, 0, NULL);


	return 0;
}