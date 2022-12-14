#pragma once
#ifndef __KYFUNCLITE_H__
#define __KYFUNCLITE_H__

// for window 
#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#pragma comment(lib,"ws2_32.lib")
#include <winsock2.h>
// https://www.twblogs.net/a/5c3ed65fbd9eee35b21e0b01
#endif // _WIN32

// for linux 
#ifdef __unix
#include <sys/socket.h>
#include <unistd.h>   //close 
#include <netinet/in.h> 
#include <arpa/inet.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#define ADDRESS_FAMILY sa_family_t
#endif // __unix

#include <sstream>
#include <vector>
#include <utility>
#include <exception>
#include <stdexcept>
#include <memory>
// https://stackoverflow.com/questions/16854308/do-i-need-a-return-after-throwing-exception-c-and-c

using std::pair;
using std::vector;
using std::stringstream;
using std::string;
using std::wstring;
using std::invalid_argument;
// define 

#define __KYFUNC_BUFFER 5000

namespace KYFuncLite {
	class Socket {
	public:
		static constexpr auto Af_Inet = AF_INET;
		static constexpr auto Stream = SOCK_STREAM;
		static constexpr auto Dgram = SOCK_DGRAM;
		static constexpr auto Normal = 0;

		static constexpr auto NoMoreRecv = 0;
		static constexpr auto NoMoreSend = 1;
		static constexpr auto NoRecvSend = 2;

	public:
		inline Socket() {
#ifdef _WINSOCK2API_
			try {
				this->__openWinSocket();
			}
			catch (const std::exception& e) {
				throw e;
			}
#endif // _WINSOCK2API_
		}

		inline Socket(const SOCKET& socketIn) {
			this->_socket = socketIn;
		}

		inline Socket(const int32_t& af, const int32_t& type, const int32_t& protocol) {
			try {
#ifdef _WINSOCK2API_
				this->__openWinSocket();
#endif // _WINSOCK2API_
				this->setSocket(af, type, protocol);
			}
			catch (const std::exception& e) {
				throw e;
			}
		}

		inline ~Socket() {
			this->closeSocket();
#ifdef _WINSOCK2API_
			this->__WSACleanUp();
#endif // _WINSOCK2API_
		}

		inline Socket& setSocket(const SOCKET& socketIn) {
			this->_socket = socketIn;

			return *this;
		}

		inline Socket& setSocket(const int32_t& af, const int32_t& type, const int32_t& protocol) {
			this->_socket = socket(af, type, protocol);

			if (this->_socket == INVALID_SOCKET) {
				throw invalid_argument("Socket is INVALID_SOCKET");
			}

			return *this;
		}

		inline const SOCKET& getSocket() const {
			return this->_socket;
		}

		inline void BindBuilder(const sockaddr& addr) const {
			int32_t _res = bind(this->_socket, &addr, sizeof(sockaddr));

			if (_res < 0) {
				throw invalid_argument("Socket Error: bind error");
			}
		}

		inline void ConnectorBuilder(const sockaddr& name) const {
			int32_t _res = connect(this->_socket, &name, sizeof(sockaddr));

			if (_res < 0) {
				throw invalid_argument("Socket error (about connect)");
			}
		}

		inline void ListenerBuilder(const int32_t& backlog) const {
			int32_t _res = listen(this->_socket, backlog);

			if (_res < 0) {
				throw invalid_argument("Socket Error : listener error");
			}
		}
#ifdef _WIN32
		inline pair<SOCKET, sockaddr> AcceptBuilder() const {
			sockaddr _sockaddrRes;
			int32_t _len = sizeof(sockaddr);

			SOCKET _res = accept(this->_socket, &_sockaddrRes, &_len);

			if (_res == INVALID_SOCKET) {
				throw invalid_argument("INVALID Socket : accept builder error");
			}

			return pair<SOCKET, sockaddr>(_res, _sockaddrRes);

		}
#else
		inline pair<SOCKET, sockaddr> AcceptBuilder() const {
			sockaddr _sockaddrRes;

			SOCKET _res = accept(this->_socket, &_sockaddrRes, NULL);

			if (_res == INVALID_SOCKET) {
				throw invalid_argument("INVALID Socket : accept builder error");
			}

			return pair<SOCKET, sockaddr>(_res, _sockaddrRes);

		}
#endif

		

		// for tcp recv 
		inline string doRecv(const int32_t& flags = 0) const {
			vector<char> buf(__KYFUNC_BUFFER);

			int32_t nBytes = recv(this->_socket, buf.data(), buf.size(), flags);

			if (nBytes < 0) {
				throw invalid_argument("doRecv: data length less than 0");
			}

			buf.resize(nBytes);

			return string(buf.begin(), buf.end());
		}

		// for udp recv 
#ifdef _WIN32
		inline string doRecvFrom(sockaddr& fromAddr, const int32_t& flags = 0) const {
			vector<char> buf(__KYFUNC_BUFFER);
			int32_t fromLen = sizeof(sockaddr);

			int32_t nBytes = recvfrom(this->_socket, buf.data(), buf.size(), flags, &fromAddr, &fromLen);

			if (nBytes < 0) {
				throw invalid_argument("doRecvFrom: data length less than 0");
			}

			buf.resize(nBytes);

			return string(buf.begin(), buf.end());
		}
#else 
		inline string doRecvFrom(sockaddr& fromAddr, const int32_t& flags = 0) const {
			vector<char> buf(__KYFUNC_BUFFER);
			int32_t fromLen = sizeof(sockaddr);

			int32_t nBytes = recvfrom(this->_socket, buf.data(), buf.size(), flags, &fromAddr, NULL);

			if (nBytes < 0) {
				throw invalid_argument("doRecvFrom: data length less than 0");
			}

			buf.resize(nBytes);

			return string(buf.begin(), buf.end());
		}
#endif
	
		// for tcp send 
		inline void doSend(const string& msg, const int32_t& flags = 0) const {
			int32_t _res = send(this->_socket, msg.c_str(), msg.length(), flags);

			if (_res == SOCKET_ERROR) {
				throw invalid_argument("send : socket error");
			}
		}

		// for udp sendto
		inline void doSendTo(const string& msg, const sockaddr& fromAddr, const int32_t& flags = 0) const {
			int32_t _res = sendto(this->_socket, msg.c_str(), msg.length(), flags, &fromAddr, sizeof(sockaddr));
			if (_res == SOCKET_ERROR) {
				throw invalid_argument("sendto : socket error");
			}
		}

		inline void shutDown(const int32_t& how) const {
			int32_t _res = shutdown(this->_socket, how);
			if (_res < 0) {
				throw invalid_argument("Socket Error: shut down");
			}
		}

		inline void closeSocket() const {
			int32_t _res = closesocket(this->_socket);
			if (_res < 0) {
				throw invalid_argument("Socket Error : close socket error");
			}
		}

		//operator 
		inline Socket& operator= (const SOCKET& sockObj) {
			this->_socket == sockObj;
			return  *this;
		}

		inline bool operator== (const Socket& socketObj) const {
			return this->_socket == socketObj._socket;
		}

		inline bool operator!= (const Socket& socketObj) const {
			return this->_socket != socketObj._socket;
		}

	private:
		SOCKET _socket;

#ifdef _WINSOCK2API_
	private:
		WSADATA _wsdata;
		inline const void __openWinSocket() {
			int32_t _res = WSAStartup(0x202, &this->_wsdata);
			if (_res < 0) {
				throw exception("cant open the winSocket");
			}
		}

		inline void __WSACleanUp() const {
			int32_t _res = WSACleanup();

			if (_res < 0) {
				throw exception("WSA clean up error");
			}
		}
#endif // _WINSOCK2API_

	};

	class SocketMonitor {
	public:
		inline SocketMonitor(const Socket& sock) {
			FD_ZERO(&this->_fdSock);
			FD_SET(sock.getSocket(), &this->_fdSock);
			this->_fdRead = this->_fdSock;
		}

		~SocketMonitor() {

		}

		inline void addPoll(const Socket& sock) {
			FD_SET(sock.getSocket(), &this->_fdSock);
		}

		inline void popPoll(const Socket& sock) {
			FD_CLR(sock.getSocket(), &this->_fdSock);
		}

		inline void listen() {
			this->_fdRead = this->_fdSock;

			int32_t res = select(0, &this->_fdRead, 0, 0, 0); // only read
			if (res == -1) {
				throw std::invalid_argument("select error");
			}
		}

		inline fd_set get_fd_data() const {
			return this->_fdRead;
		}

	private:
		fd_set _fdSock, _fdRead;
	};

	// func 

	inline sockaddr& make_sockaddr(const ADDRESS_FAMILY& family, const uint16_t& port, const uint64_t& addr) {
		sockaddr _res = {};
		sockaddr_in* ptr = (sockaddr_in*)&_res;

		ptr->sin_family = family;
		ptr->sin_port = port;
		ptr->sin_addr.s_addr = addr;

		return _res;
	}
#ifdef _WIN32
	inline const wstring& Big5ToUniCode(char* big5, const uint32_t& len) {
		wchar_t* uniCode = new wchar_t[len] {0};
		MultiByteToWideChar(950, 0, big5, -1, uniCode, len);

		return wstring(uniCode, len);
	}

	inline const string& UniCodeToBig5(wchar_t* uniCode, const uint32_t& len) {
		char* big5 = new char[len] {0};
		WideCharToMultiByte(950, 0, uniCode, -1, big5, len, NULL, NULL);
		big5[len] = 0;

		return string(big5, len);
	}
#endif
	

	inline const vector<string>& split_string(const string& inputStr, const char& chr) {
		stringstream ss;
		string tmpStr;
		vector<string> res;
		ss << inputStr;

		while (getline(ss, tmpStr, chr)) {
			res.push_back(tmpStr);
		}

		return res;
	}

	namespace format {
		template<typename ... Args>
		static std::string str_format(const std::string& format, Args ... args) {
			auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;

			if (size_buf <= 0) {
				throw std::runtime_error("Error during formatting.");
			}

			auto size_buf64 = static_cast<size_t>(size_buf);
			std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf64]);

			if (!buf) {
				return std::string("");
			}

			std::snprintf(buf.get(), size_buf64, format.c_str(), args ...);
			return std::string(buf.get(), buf.get() + size_buf64 - 1);
		}

		template<typename ... Args>
		static std::wstring wstr_format(const std::wstring& format, Args ... args) {
			auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;

			if (size_buf <= 0) {
				throw std::runtime_error("Error during formatting.");
			}

			auto size_buf64 = static_cast<size_t>(size_buf);
			std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf64]);

			if (!buf) {
				return std::wstring("");
			}

			std::snprintf(buf.get(), size_buf64, format.c_str(), args ...);
			return std::wstring(buf.get(), buf.get() + size_buf64 - 1);
		}

	}

}

#endif // !__KYFUNCLITE_H__
