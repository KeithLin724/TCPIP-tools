#pragma once
#ifndef __ONLINE_TOOLS__
#define __ONLINE_TOOLS__

/*
Title: online tools
Written By Keith Lin
This is a code for make TCPIP tools
easy to use
support tcp and udp 
Date: 15/9/2022
*/


#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
// https://www.twblogs.net/a/5c3ed65fbd9eee35b21e0b01


#include <sstream>
#include <vector>
#include <utility>
#include <exception>
// https://stackoverflow.com/questions/16854308/do-i-need-a-return-after-throwing-exception-c-and-c


using std::pair;
using std::vector;
using std::stringstream;
using std::string;
using std::wstring;
using std::exception;
using std::hash;
using std::size_t;
// define 

#define __KYFUNC_BUFFER 2000


namespace KYFunc {
	
	class Recv { // support tcp and udp 
	public:
		static constexpr auto Normal = 0;
		static constexpr auto Reek = MSG_PEEK;
		static constexpr auto Oob = MSG_OOB;

		
	public:
		inline Recv()
			:_dataLen(INT32_MIN), _dataStr(string()) {}; // obj 

		// tcp 
		inline Recv(const SOCKET& socket, const int32_t& flags) {// function style
			try {
				this->setTCPrecvIn(socket, flags);
			}
			catch (const std::exception& e) {
				throw e;
			}
		}

		// udp 
		inline Recv(const SOCKET& socket, const int32_t& flags, sockaddr& fromAddr) {
			try {
				this->setUDPrecvIn(socket, flags, fromAddr);
			}
			catch (const std::exception& e) {
				throw e;
			}
		}

		inline ~Recv() {};
		// set style 

		// tcp
		const Recv& setTCPrecvIn(const SOCKET& socket, const int32_t& flags) {
			char _charStr[__KYFUNC_BUFFER] = { 0 };
			
			uint32_t _size = sizeof(_charStr) - 1;

			this->_dataLen = recv(socket, _charStr, _size, flags);

			if (this->_dataLen <= 0) {
				this->_dataStr = string();
				throw exception("setTCPrecvIn : data length less than 0");
				// return *this;
				
			}
			_charStr[this->_dataLen] = 0;
			this->_dataStr.assign(_charStr, _size);
			return *this;
		}

		// udp 
		const Recv& setUDPrecvIn(const SOCKET& socket, const int32_t& flags , sockaddr& fromAddr ) {
			char _charStr[__KYFUNC_BUFFER] = { 0 };

			int32_t _size = sizeof(_charStr) - 1;

			this->_dataLen = recvfrom(socket, _charStr, sizeof(_charStr) - 1, flags, &fromAddr, &_size);

			if (this->_dataLen <= 0) {
				this->_dataStr = string();
				throw exception("setUDPrecvIn : data length less than 0");
				// return *this;

			}

			_charStr[this->_dataLen] = 0;
			this->_dataStr.assign(_charStr, _size);
			return *this;
		}

		//get method 
		inline const int32_t& getDataLen() const {
			return this->_dataLen;
		}

		inline const string& getDataStr() const {
			return this->_dataStr;
		}

	
	private:
		int32_t _dataLen;
		string _dataStr;

	};

	class Send { // support tcp and udp 
	public:
		static constexpr auto Normal = 0;
		static constexpr auto Dontroute = MSG_DONTROUTE;
		static constexpr auto Oob = MSG_OOB;
	
	public:
		inline Send() {
		}
		
		inline ~Send() {

		}

		//support tcp
		inline Send(const SOCKET& socket, const string& msg, const int32_t& flags) {
			try {
				this->setTCPsendOut(socket, msg, flags);
			}
			catch (const std::exception& e) {
				throw e;
			}
		}

		//support upd 
		inline Send(const SOCKET& socket, const string& msg, const int32_t& flags, sockaddr& toAddr, const int32_t& len = sizeof(sockaddr)) {
			try {
				this->setUDPsendOut(socket, msg, flags, toAddr, len);
			}
			catch (const std::exception& e) {
				throw e;
			}
		}

		//set TCP Send 
		inline const void setTCPsendOut(const SOCKET& socket, const string& msg, const int32_t& flags) {
			int32_t _res = send(socket, msg.c_str(), msg.length(), flags);

			if (_res == SOCKET_ERROR) {
				throw exception("send : socket error");
			}
		}

		//set UDP Send 
		inline const void setUDPsendOut(const SOCKET& socket, const string& msg, const int32_t& flags, const sockaddr& toAddr, const int32_t& len = sizeof(sockaddr)) {
			int32_t _res = sendto(socket, msg.c_str(), msg.length(), flags, &toAddr, len);

			if (_res == SOCKET_ERROR) {
				throw exception("sendto : socket error");
			}
		}

		inline const void setUDPsendOut(const SOCKET& socket, const string& msg, const int32_t& flags, sockaddr_in& toAddr, const int32_t& len = sizeof(sockaddr)) {
			int32_t _res = sendto(socket, msg.c_str(), msg.length(), flags, (sockaddr*)&toAddr, len);

			if (_res == SOCKET_ERROR) {
				throw exception("sendto : socket error");
			}
		}

	};

	class Accept {
	public:
		inline Accept() 
			:_newSocket(SOCKET()), _addr(new sockaddr()), _addrLen(int32_t()) {

		}
		
		inline Accept(const SOCKET& socket) {
			try {
				this->setAccept(socket);
			}
			catch (const std::exception& e) {
				throw e;
			}
		}
		
		inline ~Accept() {

		}

		inline const Accept& setAccept(const SOCKET& socket) {
			this->_newSocket = accept(socket, this->_addr, &this->_addrLen);

			if (this->_newSocket == INVALID_SOCKET) {
				throw exception("Accept: INVALID_SOCKET");
			}
		}

		inline const SOCKET& getNewSocket() const {
			return this->_newSocket;
		}

		inline const sockaddr* getNewSockAddr() const {
			return this->_addr;
		}

		inline const int32_t& getAddrLength() const {
			return this->_addrLen;
		}

	private:
		SOCKET _newSocket;
		sockaddr* _addr;
		int32_t _addrLen;
	};

	class SockAddr {
	public:
		inline static const sockaddr* to_sockaddr(const sockaddr_in& Addrs) {
			return (sockaddr*)&Addrs;
		}

	public:
		inline SockAddr() {
			// this->_addrIn = sockaddr_in();
			this->_setting = false;
			this->_addrLen = sizeof(sockaddr);
		}

		inline ~SockAddr() {

		}

		inline SockAddr (ADDRESS_FAMILY family, uint16_t port, uint64_t IP_code) {
			try {
				this->setData(family, port, IP_code);
			}
			catch (const std::exception& e) {
				throw e;
			}

		}

		inline void setData(ADDRESS_FAMILY family, uint16_t port, uint64_t IP_code) {
			sockaddr_in* tmpRes = (sockaddr_in*)&this->_addrIn;

			// function 
			tmpRes->sin_family = family;
			tmpRes->sin_port = port;
			tmpRes->sin_addr.s_addr = IP_code;

			this->_addrLen = sizeof(sockaddr);
			this->_setting = true;
		}

		inline const sockaddr* getConstSockaddrPTR() const {
			if (this->_setting == false) {
				throw exception("SockAddr no setting");
			}

			return (sockaddr*)&this->_addrIn;
		}

		inline sockaddr* getSockaddrPTR() const {
			if (this->_setting == false) {
				throw exception("SockAddr no setting");
			}

			return (sockaddr*)&this->_addrIn;
		}

		inline sockaddr& passData() {
			return this->_addrIn;
		}

		inline const sockaddr& passConstData() {
			return this->_addrIn;
		}

		inline int32_t& getSockaddrLen() {
			return this->_addrLen;
		}

		// operator 
		bool operator==(const SockAddr& obj) {
			return memcmp(&this->_addrIn, &obj._addrIn, sizeof(sockaddr)) == 0;
		}

		bool operator!=(const SockAddr& obj) {
			return memcmp(&this->_addrIn, &obj._addrIn, sizeof(sockaddr)) != 0;
		}

		// hash func 
		size_t operator()(const SockAddr& k) const {

			return ((hash<ADDRESS_FAMILY>()(k._addrIn.sa_family) ^ (hash<string>()(k._addrIn.sa_data) << 1)) >> 1);
		}

	private:
		sockaddr _addrIn;
		int32_t _addrLen;
		bool _setting;
	};

	

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
		inline Socket(){
			try {
				this->__openWinSocket();
			}
			catch (const std::exception& e) {
				throw e;
			}
		}
		inline Socket(const int32_t& af, const int32_t& type, const int32_t& protocol) {
			try {
				this->__openWinSocket();
				this->setSocket(af, type, protocol);
			}
			catch (const std::exception& e) {
				throw e;
			}
		}

		inline ~Socket() {

		}

		inline Socket& setSocket(const int32_t& af, const int32_t& type, const int32_t& protocol) {
			this->_socket = socket(af, type, protocol);

			if (this->_socket == INVALID_SOCKET) {
				throw exception("Socket is INVALID_SOCKET");
			}

			return *this;
		}

		inline const SOCKET& getSocket() const {
			return this->_socket;
		}
		
		inline void BindBuilder(const sockaddr* addr) const {
			int32_t _res = bind(this->_socket, addr, sizeof(addr));

			if (_res < 0) {
				throw exception("Socket Error: bind error");
			}
		}

		inline void BindBuilder(const SockAddr& addr) const {
			int32_t _res;
			try {
				_res = bind(this->_socket, addr.getConstSockaddrPTR(), sizeof(addr));
			}
			catch (const std::exception& e) {
				throw e;
			}

			if (_res < 0) {
				throw exception("Socket Error: bind error");
			}
		}

		inline void ConnectorBuilder(const sockaddr* name, const int32_t& len = sizeof(sockaddr)) const {
			int32_t _res = connect(this->_socket, name, len);

			if (_res < 0) {
				throw exception("Socket error (about connect)");
			}
		}

		inline void ConnectorBuilder(const SockAddr& name, const int32_t& len = sizeof(sockaddr)) const {
			int32_t _res;
			try {
				_res = connect(this->_socket, name.getConstSockaddrPTR(), len);
			}
			catch (const std::exception& e) {
				throw e;
			}

			if (_res < 0) {
				throw exception("Socket error (about connect)");
			}
		}

		inline const Accept& getAccept() {
			try {
				this->_acceptor.setAccept(this->_socket);
			}
			catch (const std::exception& e) {
				throw e;
			}

			return this->_acceptor;
		}

		inline void ListenerBuilder(const int32_t& backlog) {
			int32_t _res = listen(this->_socket, backlog);

			if (_res < 0) {
				throw exception("Socket Error : listener error");
			}
		}

		// for tcp recv 
		inline const Recv& doRecv(const int32_t& flags) {
			try {
				this->_recvtor.setTCPrecvIn(this->_socket, flags);
			}
			catch (const std::exception& e) {
				throw e;
			}
			return this->_recvtor;
		}

		// for udp recv 
		inline const Recv& doRecvFrom(const int32_t& flags, sockaddr& fromAddr) {
			try {
				this->_recvtor.setUDPrecvIn(this->_socket, flags, fromAddr);
			}
			catch (const std::exception& e) {
				throw e;
			}
			return this->_recvtor;
		}

		inline const Recv& doRecvFrom(const int32_t& flags, SockAddr& fromAddr) {
			try {
				this->_recvtor.setUDPrecvIn(this->_socket, flags, fromAddr.passData());
			}
			catch (const std::exception& e) {
				throw e;
			}
			return this->_recvtor;
		}

		// for tcp send 
		inline const Send& doSend(const string& msg , const int32_t& flags) {
			try {
				this->_sendtor.setTCPsendOut(this->_socket, msg, flags);
			}
			catch (const std::exception& e) {
				throw e;
			}

			return this->_sendtor;
		}

		// for udp sendto
		inline const Send& doSendTo(const string& msg, const int32_t& flags, sockaddr& fromAddr, const int32_t& fromLen = sizeof(sockaddr)) {
			try {
				this->_sendtor.setUDPsendOut(this->_socket, msg, flags, fromAddr, fromLen);
			}
			catch (const std::exception& e) {
				throw e;
			}

			return this->_sendtor;
		}

		inline const Send& doSendTo(const string& msg, const int32_t& flags, SockAddr& fromAddr, const int32_t& fromLen = sizeof(sockaddr)) {
			try {
				this->_sendtor.setUDPsendOut(this->_socket, msg, flags, fromAddr.passData(), fromLen);
			}
			catch (const std::exception& e) {
				throw e;
			}

			return this->_sendtor;
		}

		inline void shutDown(const int32_t& how) const {
			int32_t _res = shutdown(this->_socket, how);
			
			if (_res < 0) {
				throw exception("Socket Error: shut down");
			}
		}

		inline void closeSocket() const {
			int32_t _res = closesocket(this->_socket);
			if (_res < 0) {
				throw exception("Socket Error : close socket error");
			}
		}
	
		inline void WSACleanUp() const {
			int32_t _res = WSACleanup();

			if (_res < 0) {
				throw exception("WSA clean up error");
			}
		}


	private:
		inline const void __openWinSocket() {
			int32_t _res = WSAStartup(0x202, &this->_wsdata);

			if (_res < 0) {
				throw exception("cant open the winSocket");
			}
		}

		// hash 
		size_t operator()(const Socket& k) const {

			return ((hash<SOCKET>()(k._socket) ^ (hash<SOCKET>()(k._socket) << 1)) >> 1);
		}


	private:
		SOCKET _socket;
		WSADATA _wsdata;
		Recv _recvtor;
		Send _sendtor;
		Accept _acceptor;
	};

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

	inline const vector<string>& split_string(const string& inputStr , const char& chr) {
		stringstream ss;
		string tmpStr;
		vector<string> res;
		ss << inputStr;

		while (getline(ss,tmpStr,chr)) {
			res.push_back(tmpStr);
		}

		return res;
	}
}

#endif // !__ONLINE_TOOLS__
