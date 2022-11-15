#pragma once

#ifndef __TCPIP_THREAD_H__
#define __TCPIP_THREAD_H__

#include "KYFuncLite.h"

#include <iostream>

using std::cout;
using std::uintptr_t;

namespace KYFuncLite {
	struct ServerThreadData {
		Socket g_S_Socket;
		Socket g_S_Socket1;
		uintptr_t Thread_ID11;
	} SData;


	// thread 

	void Thread_Func11(PVOID pvoid) {
		string recvData; 
		sockaddr_in* AddPtr;

		while (1) {
			try {
				auto _res = SData.g_S_Socket.AcceptBuilder();
				SData.g_S_Socket1.setSocket(_res.first);
				AddPtr = (sockaddr_in*)&_res.second;

				do {
					recvData = SData.g_S_Socket1.doRecv();

					if (!recvData.empty()) {
						cout << "Recv (" << inet_ntoa(AddPtr->sin_addr) << ") = " << recvData << '\n';
					}

				} while (recvData.size() > 0);
			}
			catch (const std::exception& e) {
				// pass 
			}
		}
	}

	struct ClientThreadData {
		Socket g_C_Socket;
		uintptr_t Thread_ID22;
	} CData;

	void Thread_Func22(PVOID pvoid) {
		string _res , _resClone;
		bool Flag = false;
		int  k, k1, k2, k3, Beg, Fa2 = 0;

		while (1) {
			_res = CData.g_C_Socket.doRecv();

			if (!_res.empty()) {
				for (size_t i = 0; i < _res.size(); i++) {
					if (_res[i] == 27) {
						Flag = true;
						Beg = i + 1;
					}

					if (!Flag) {
						cout << _res[i];
					}

					if ((Flag) && (_res[i] == 'm')) {
						Flag = false;
						
						_resClone = string(_res.begin() + Beg, _res.begin() + (i - Beg));
						
						// S2[End - Beg + 1] = 0;
						k = sscanf_s(_resClone.c_str(), "[%d;%d;%dm", &k1, &k2, &k3);
						//if ((k==1)&&(k1==41))
					}
				}
			}
		}
	}

}

#endif // !__TCPIP_THREAD_H__