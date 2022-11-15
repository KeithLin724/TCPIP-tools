#pragma once

#ifndef __TCPIP_FUNCLITE__
#define __TCPIP_FUNCLITE__

#include "KYFuncLite.h"

namespace KYFuncLite {
	// Tcp function 
	inline void Start_TCP_Server(Socket& sock, const std::uint16_t& Port) {

		try {
			sock.setSocket(Socket::Af_Inet, Socket::Stream, Socket::Normal);

			auto& tcpServer = make_sockaddr(Socket::Af_Inet, htons(Port), htonl(INADDR_ANY));
			
			sock.BindBuilder(tcpServer);
			sock.ListenerBuilder(SOMAXCONN);
		}
		catch (const std::exception& e) {
			throw e;
		}

	}

	inline void Start_TCP_Client(Socket& sock, const std::uint16_t& R_Port, const string& IP) {

		// sockaddr_in tcpClients;

		try {
			sock.setSocket(Socket::Af_Inet, Socket::Stream, Socket::Normal);
			
			auto& tcpClients = make_sockaddr(Socket::Af_Inet, htons(R_Port), inet_addr(IP.c_str()));
			
			sock.ConnectorBuilder(tcpClients);
		}
		catch (const std::exception& e) {
			throw e;
		}

	}

	inline void Start_UDP_Server(Socket& sock, const std::uint16_t& Port) {

		try {
			sock.setSocket(Socket::Af_Inet, Socket::Dgram, Socket::Normal);

			auto& udpServer = make_sockaddr(Socket::Af_Inet, htons(Port), htonl(INADDR_ANY));

			sock.BindBuilder(udpServer);
		}
		catch (const std::exception& e) {
			throw e;
		}


	}

	inline void Start_UDP_Client(Socket& sock, sockaddr& udpClient, const std::uint16_t& R_Port, const string& IP) {
		try {
			sock.setSocket(Socket::Af_Inet, Socket::Dgram, Socket::Normal);
		}
		catch (const std::exception& e) {
			throw e;
		}

		udpClient = make_sockaddr(AF_INET, htons(R_Port), inet_addr(IP.c_str()));
	}

	
	/// ///////////////////////////////
	inline Socket& TCP_Server(const std::uint16_t& Port) {
		Socket sock;

		try {
			sock.setSocket(Socket::Af_Inet, Socket::Stream, Socket::Normal);

			auto& tcpServer = make_sockaddr(Socket::Af_Inet, htons(Port), htonl(INADDR_ANY));

			sock.BindBuilder(tcpServer);
			sock.ListenerBuilder(SOMAXCONN);
		}
		catch (const std::exception& e) {
			throw e;
		}

		return sock;
	}

	inline SOCKET& Fast_TCP_Server_Builder(const std::uint16_t& Port) {
		SOCKET tcpSock;
		WSADATA  wsadata;
		std::int32_t err;
		sockaddr_in  tcpserver;

		// 1. ¶}±Ò TCP Server
		if ((err = WSAStartup(0x202, &wsadata)) != 0) {
			throw std::invalid_argument("WSA ERROR");
		}
			
		if ((tcpSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			throw std::invalid_argument("Socket error");
		}

		tcpserver.sin_family = AF_INET;
		tcpserver.sin_port = htons(Port);
		tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(tcpSock, (sockaddr*)&tcpserver, sizeof(tcpserver)) < 0) {
			throw std::invalid_argument("bind error");
		}
		if ((err = listen(tcpSock, SOMAXCONN)) < 0) {
			throw std::invalid_argument("listen function error");
		};

		return tcpSock;
	}

	inline Socket& TCP_Client(const std::uint16_t& R_Port, const string& IP) {

		// sockaddr_in tcpClients;
		Socket sock;

		try {
			sock.setSocket(Socket::Af_Inet, Socket::Stream, Socket::Normal);

			auto& tcpClients = make_sockaddr(Socket::Af_Inet, htons(R_Port), inet_addr(IP.c_str()));

			sock.ConnectorBuilder(tcpClients);
		}
		catch (const std::exception& e) {
			throw e;
		}

		return sock;
	}

	inline SOCKET& Fast_TCP_Client_Builder(const std::uint16_t& R_Port, const std::string& IP) {
		SOCKET tcpSock;
		WSADATA  wsadata;
		std::int32_t err;
		sockaddr_in  tcpclient;

		if ((err = WSAStartup(0x202, &wsadata)) != 0) {
			throw std::invalid_argument("WSA ERROR");
		}

		if ((tcpSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			throw std::invalid_argument("Socket error");
		}

		tcpclient.sin_family = AF_INET;
		tcpclient.sin_port = htons(R_Port);
		tcpclient.sin_addr.s_addr = inet_addr(IP.c_str());

		if (err = connect(tcpSock, (sockaddr*)&tcpclient, sizeof(tcpclient)) < 0) {
			throw std::invalid_argument("connect error");
		};

		return tcpSock;
	}

	inline Socket& UDP_Server(const WORD& Port) {
		Socket sock;
		try {
			sock.setSocket(Socket::Af_Inet, Socket::Dgram, Socket::Normal);

			auto& udpServer = make_sockaddr(Socket::Af_Inet, htons(Port), htonl(INADDR_ANY));

			sock.BindBuilder(udpServer);
		}
		catch (const std::exception& e) {
			throw e;
		}

		return sock;
	}

	inline std::pair<Socket , sockaddr>& UDP_Client(const WORD& R_Port, const string& IP) {
		std::pair<Socket, sockaddr> res;

		try {
			res.first.setSocket(Socket::Af_Inet, Socket::Dgram, Socket::Normal);
		}
		catch (const std::exception& e) {
			throw e;
		}

		res.second = make_sockaddr(AF_INET, htons(R_Port), inet_addr(IP.c_str()));

		return res;
	}

	
}

#endif // !__TCPIP_FUNCLITE__

