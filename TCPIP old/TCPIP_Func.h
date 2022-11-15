#pragma once

#ifndef __TCPIP_FUNC__
#define __TCPIP_FUNC__

#include "online_tools.h"

namespace KYFunc {
	// Tcp function 
	inline void Start_TCP_Server(Socket& sock ,const WORD& Port ) { //,const DWORD& Event , HWND Hwnd 

		try {
			sock.setSocket(Socket::Af_Inet, Socket::Stream, Socket::Normal);
		}
		catch (const std::exception& e) {
			throw e;
		}

		/*if (WSAAsyncSelect(sock.getSocket(), Hwnd, Event, FD_ACCEPT | FD_WRITE | FD_READ | FD_CLOSE) < 0) {
			throw exception("Asynchronized Mode error");
		}*/

		try {
			auto tcpServer = SockAddr(Socket::Af_Inet, htons(Port), htonl(INADDR_ANY));
			sock.BindBuilder(tcpServer);
			sock.ListenerBuilder(SOMAXCONN);
		}
		catch (const std::exception& e) {
			throw e;
		}

	}

	inline void Start_TCP_Client(Socket& sock,const WORD& R_Port, const string& IP) { // const DWORD& Event, HWND Hwnd

		// sockaddr_in tcpClients;

		try {
			sock.setSocket(Socket::Af_Inet, Socket::Stream, Socket::Normal);
		}
		catch (const std::exception& e) {
			throw e;
		}

		/*if (WSAAsyncSelect(sock.getSocket(), Hwnd, Event, FD_ACCEPT | FD_WRITE | FD_READ | FD_CLOSE) < 0) {
			throw exception("Asynchronized Mode error");
		}*/

		try {
			auto tcpClients = SockAddr(Socket::Af_Inet, htons(R_Port), inet_addr(IP.c_str()));
			sock.ConnectorBuilder(tcpClients);
		}
		catch (const std::exception& e) {
			throw e;
		}

	}

	inline void Start_UDP_Server(Socket& sock, const WORD& Port) { // const int32_t& Event, HWND Hwnd
		
		try {
			sock.setSocket(Socket::Af_Inet, Socket::Dgram, Socket::Normal);
		}
		catch (const std::exception& e) {
			throw e;
		}

		/*if (WSAAsyncSelect(sock.getSocket(), Hwnd, Event, FD_ACCEPT | FD_WRITE | FD_READ | FD_CLOSE) < 0) {
			throw exception("Asynchronized Mode error");
		}*/

		try {
			// Socket Naming
			auto udpServer = SockAddr(Socket::Af_Inet, htons(Port), htonl(INADDR_ANY));
			sock.BindBuilder(udpServer);
		}
		catch (const std::exception& e) {
			throw e;
		}

	}

	inline void Start_UDP_Client(Socket& sock,SockAddr& udpClient ,const WORD& R_Port , const string& IP) { //const int32_t& Event , HWND Hwnd
		try {
			sock.setSocket(Socket::Af_Inet, Socket::Dgram, Socket::Normal);
		}
		catch (const std::exception& e) {
			throw e;
		}

		/*if (WSAAsyncSelect(sock.getSocket(), Hwnd, Event, FD_ACCEPT | FD_WRITE | FD_READ | FD_CLOSE) < 0) {
			throw exception("Asynchronized Mode error");
		}*/
		udpClient.setData(AF_INET, htons(R_Port), inet_addr(IP.c_str()));
	}

}

#endif // !__TCPIP_FUNC__

