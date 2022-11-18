#pragma comment(lib,"ws2_32.lib")
#include "winsock2.h"
//#define _WINSOCK_DEPRECATED_NO_WARNINGS 1  //³o­Ó©w¸q¦bpch.hªº²Ä¤@¦æ

/*
char API_ErrMsg[256];
void WSA_Get_ErrTxt(char *S1) {  }

int  Start_TCP_Server(SOCKET *psockfd,WORD Port,DWORD EVENT,HWND Hwnd)
{
WSADATA  wsadata;
int      err;
struct   sockaddr_in  tcpserver;

  // ±Ò°ÊWinsock
  if ((err=WSAStartup(0x202,&wsadata))!=0) 
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
	
  // TCP socket open
  if ((*psockfd=socket(AF_INET,SOCK_STREAM,0))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

  // Set Asynchronized Mode
  if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_ACCEPT | FD_WRITE | FD_READ | FD_CLOSE)) < 0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

  // Socket Naming
  tcpserver.sin_family=AF_INET;
  tcpserver.sin_port=htons(Port);
  tcpserver.sin_addr.s_addr=htonl(INADDR_ANY);
  if (bind(*psockfd,(struct sockaddr *)&tcpserver,sizeof(tcpserver))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

  // Wait for Client Login
  if ((err=listen(*psockfd,SOMAXCONN))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

   return 0;
}

int Start_TCP_Client(SOCKET *psockfd,WORD R_Port,char *IP,DWORD EVENT,HWND Hwnd)
{
WSADATA  wsadata;
int      err,i,on=1;
struct   sockaddr_in  tcpclient,Addr;
LPHOSTENT HostIP;
char      IP11[100];
int IP1, IP2, IP3, IP4;

  // ±Ò°ÊWinsock
  if ((err=WSAStartup(0x202,&wsadata))!=0) 
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
	
  // TCP socket open
  if ((*psockfd=socket(AF_INET,SOCK_STREAM,0))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

  // Set Asynchronized Mode
  if ((err=WSAAsyncSelect(*psockfd,Hwnd,EVENT,FD_CONNECT|FD_WRITE|FD_READ|FD_CLOSE))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1;  }

  // Socket Naming
  tcpclient.sin_family=AF_INET;
  tcpclient.sin_port=htons(R_Port);
  sscanf_s(IP, "%d.%d.%d.%d", &IP1, &IP2, &IP3, &IP4);
  tcpclient.sin_addr.s_addr = IP1 + (IP2 << 8) + (IP3 << 16) + (IP4 << 24);// inet_addr(IP);

  // Connect to Server
  if (err=connect(*psockfd,(struct sockaddr *)&tcpclient,sizeof(tcpclient))<0) 
  {  WSA_Get_ErrTxt(API_ErrMsg);  return -1; }

  return 0;
}



int Start_UDP_Server(SOCKET *psockfd,WORD Port,int EVENT, HWND Hwnd)
{
WSADATA  wsadata;
int      err;
struct   sockaddr_in  udpserver;

  // ±Ò°ÊWinsock
  if ((err=WSAStartup(0x202,&wsadata))!=0) 
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
  
  // TCP socket open
  if ((*psockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -2; }

  // Set Asynchronized Mode
  if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_READ )) < 0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -3;  }

  // Socket Naming
  udpserver.sin_family=AF_INET;
  udpserver.sin_port=htons(Port);
  udpserver.sin_addr.s_addr=htonl(INADDR_ANY);
  if (bind(*psockfd,(struct sockaddr *)&udpserver,sizeof(udpserver))<0)
  {  err=WSAGetLastError();
	 WSA_Get_ErrTxt(API_ErrMsg); return -4; }

  return 0;
}
int Start_UDP_Client(SOCKET *psockfd,sockaddr_in *udpclient,WORD R_Port,char *IP,int EVENT, HWND Hwnd)
{
WSADATA     wsadata;
int         err,i;
LPHOSTENT   HostIP;
char      IP11[100];
int IP1, IP2, IP3, IP4;

  // ±Ò°ÊWinsock
  if ((err=WSAStartup(0x202,&wsadata))!=0) 
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }
	
  // TCP socket open
  if ((*psockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1; }

    // Set Asynchronized Mode
  
  if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT,FD_READ)) < 0)
  {  WSA_Get_ErrTxt(API_ErrMsg); return -1;  }


  // Socket Naming
  udpclient->sin_family=AF_INET;
  udpclient->sin_port=htons(R_Port);
  //udpclient->sin_addr.s_addr=inet_addr(IP11);
  sscanf_s(IP, "%d.%d.%d.%d", &IP1, &IP2, &IP3, &IP4);
  udpclient->sin_addr.s_addr = IP1 + (IP2 << 8) + (IP3 << 16) + (IP4 << 24);// inet_addr(IP);

  return 0;
}

// ##############################################
//big5Âàunicode¬O¥ÎMultiByteToWideChar
void Big5ToUniCode(char *Big5, wchar_t *UniCode, int Len)
{
	MultiByteToWideChar(950, 0, Big5, -1, UniCode, Len);
}
//uniqueÂàbig5¬O¥ÎWideCharToMultiByte
void UniCodeToBig5(wchar_t *UniCode, char *Big5, int Len)
{
	WideCharToMultiByte(950, 0, UniCode, -1, Big5, Len, NULL, NULL);
	Big5[Len] = 0;
}
// wchar string process function
int wstrlen(wchar_t *Src, int Size)
{
	int i;
	for (i = 0; i < Size; i++) if (Src[i] == 0) break;
	if (i < Size) return i;
	else          return -1;
}
int wstrcpy(wchar_t *Dst, int Size, wchar_t *Src)
{
	int i, Len;
	Len = wstrlen(Src, Size); if (Len > Size - 1) Len = Size - 1;
	for (i = 0; i <= Len; i++) Dst[i] = Src[i];
	return Len;
}
int wstrcat(wchar_t *Dst, int Size, wchar_t *Src)
{
	int i, Len1, Len2;
	Len1 = wstrlen(Dst, Size);
	Len2 = wstrlen(Src, Size);
	if ((Len1 + Len2) > (Size - 1)) return -1;
	for (i = 0; i <= Len2; i++) Dst[Len1 + i] = Src[i];
	return 0;
}
// ####################################################

// ##########################################
// #### User Define Function & Variable #####
#define CMD_NUM    30          // «ü¥O°Ñ¼Æ¼Æ¶q
#define CMD_LEN  1000          // «ü¥O°Ñ¼Æªø«×
char    Cmd[CMD_NUM][CMD_LEN]; // Àx¦s«ü¥O°Ñ¼Æ

int Cmd_Proc(char *S1, char *Command, char Flag, int Command_Num, int Command_Len)
{	int  i, j, k1, k2;
	unsigned char *US1;
	US1 = (unsigned char *)S1;
	// ==== Resoluted Command Line ====
	i = 0; k1 = 0; k2 = 0; j = strlen(S1);
	while ((i < j) && (k1 < Command_Num - 1) && (k2 < Command_Len - 1))
	{	if (US1[i] == (unsigned char)Flag) { Command[k1*Command_Len + k2] = 0; k1++; k2 = 0; i++; }
		else
		{	if (US1[i] >= 0x80) { Command[k1*Command_Len + k2] = US1[i]; i++; k2++; }
			Command[k1*Command_Len + k2] = US1[i]; i++; k2++;
		}
	}
	Command[k1*Command_Len + k2] = 0; for (i = k1 + 1; i < Command_Num; i++) Command[i*Command_Len + 0] = 0;
	if ((k1 == 0) || (k2 == (Command_Len - 1))) return -1; else return k1;
}

*/

// ############################################################
// ###########  非同步模式(Asynchronized Mode)  ###############
// ############################################################

char API_ErrMsg[256];
void WSA_Get_ErrTxt(char *S1) {  }

int  Start_TCP_Server(SOCKET *psockfd, WORD Port, DWORD EVENT, HWND Hwnd)
{
	WSADATA  wsadata;
	int      err;
	struct   sockaddr_in  tcpserver;

	// ±Ò°ÊWinsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_ACCEPT | FD_WRITE | FD_READ | FD_CLOSE)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Socket Naming
	tcpserver.sin_family = AF_INET;
	tcpserver.sin_port = htons(Port);
	tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*psockfd, (struct sockaddr *)&tcpserver, sizeof(tcpserver)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Wait for Client Login
	if ((err = listen(*psockfd, SOMAXCONN)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	return 0;
}

int Start_TCP_Client(SOCKET *psockfd, WORD R_Port, char *IP, DWORD EVENT, HWND Hwnd)
{
	WSADATA  wsadata;
	int      err, i, on = 1;
	struct   sockaddr_in  tcpclient, Addr;
	LPHOSTENT HostIP;
	char      IP11[100];
	int IP1, IP2, IP3, IP4;

	// ±Ò°ÊWinsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_CONNECT | FD_WRITE | FD_READ | FD_CLOSE)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Socket Naming
	tcpclient.sin_family = AF_INET;
	tcpclient.sin_port = htons(R_Port);
	sscanf_s(IP, "%d.%d.%d.%d", &IP1, &IP2, &IP3, &IP4);
	tcpclient.sin_addr.s_addr = IP1 + (IP2 << 8) + (IP3 << 16) + (IP4 << 24);// inet_addr(IP);

	// Connect to Server
	if (err = connect(*psockfd, (struct sockaddr *)&tcpclient, sizeof(tcpclient)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg);  return -1;
	}

	return 0;
}



int Start_UDP_Server(SOCKET *psockfd, WORD Port, int EVENT, HWND Hwnd)
{
	WSADATA  wsadata;
	int      err;
	struct   sockaddr_in  udpserver;

	// ±Ò°ÊWinsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -2;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_READ)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -3;
	}

	// Socket Naming
	udpserver.sin_family = AF_INET;
	udpserver.sin_port = htons(Port);
	udpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*psockfd, (struct sockaddr *)&udpserver, sizeof(udpserver)) < 0)
	{
		err = WSAGetLastError();
		WSA_Get_ErrTxt(API_ErrMsg); return -4;
	}

	return 0;
}
int Start_UDP_Client(SOCKET *psockfd, sockaddr_in *udpclient, WORD R_Port, char *IP, int EVENT, HWND Hwnd)
{
	WSADATA     wsadata;
	int         err, i;
	LPHOSTENT   HostIP;
	char      IP11[100];
	int IP1, IP2, IP3, IP4;

	// ±Ò°ÊWinsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Set Asynchronized Mode

	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_READ)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}


	// Socket Naming
	udpclient->sin_family = AF_INET;
	udpclient->sin_port = htons(R_Port);
	//udpclient->sin_addr.s_addr=inet_addr(IP11);
	sscanf_s(IP, "%d.%d.%d.%d", &IP1, &IP2, &IP3, &IP4);
	udpclient->sin_addr.s_addr = IP1 + (IP2 << 8) + (IP3 << 16) + (IP4 << 24);// inet_addr(IP);

	return 0;
}

// ##############################################
//big5Âàunicode¬O¥ÎMultiByteToWideChar
void Big5ToUniCode(char *Big5, wchar_t *UniCode, int Len)
{
	MultiByteToWideChar(950, 0, Big5, -1, UniCode, Len);
}
//uniqueÂàbig5¬O¥ÎWideCharToMultiByte
void UniCodeToBig5(wchar_t *UniCode, char *Big5, int Len)
{
	WideCharToMultiByte(950, 0, UniCode, -1, Big5, Len, NULL, NULL);
	Big5[Len] = 0;
}
// wchar string process function
int wstrlen(wchar_t *Src, int Size)
{
	int i;
	for (i = 0; i < Size; i++) if (Src[i] == 0) break;
	if (i < Size) return i;
	else          return -1;
}
int wstrcpy(wchar_t *Dst, int Size, wchar_t *Src)
{
	int i, Len;
	Len = wstrlen(Src, Size); if (Len > Size - 1) Len = Size - 1;
	for (i = 0; i <= Len; i++) Dst[i] = Src[i];
	return Len;
}
int wstrcat(wchar_t *Dst, int Size, wchar_t *Src)
{
	int i, Len1, Len2;
	Len1 = wstrlen(Dst, Size);
	Len2 = wstrlen(Src, Size);
	if ((Len1 + Len2) > (Size - 1)) return -1;
	for (i = 0; i <= Len2; i++) Dst[Len1 + i] = Src[i];
	return 0;
}
// ####################################################

// ##########################################
// #### User Define Function & Variable #####
#define CMD_NUM    30          // «ü¥O°Ñ¼Æ¼Æ¶q
#define CMD_LEN  1000          // «ü¥O°Ñ¼Æªø«×
char    Cmd[CMD_NUM][CMD_LEN]; // Àx¦s«ü¥O°Ñ¼Æ

int Cmd_Proc(char *S1, char *Command, char Flag, int Command_Num, int Command_Len)
{
	int  i, j, k1, k2;
	unsigned char *US1;
	US1 = (unsigned char *)S1;
	// ==== Resoluted Command Line ====
	i = 0; k1 = 0; k2 = 0; j = strlen(S1);
	while ((i < j) && (k1 < Command_Num - 1) && (k2 < Command_Len - 1))
	{
		if (US1[i] == (unsigned char)Flag) { Command[k1*Command_Len + k2] = 0; k1++; k2 = 0; i++; }
		else
		{
			if (US1[i] >= 0x80) { Command[k1*Command_Len + k2] = US1[i]; i++; k2++; }
			Command[k1*Command_Len + k2] = US1[i]; i++; k2++;
		}
	}
	Command[k1*Command_Len + k2] = 0; for (i = k1 + 1; i < Command_Num; i++) Command[i*Command_Len + 0] = 0;
	if ((k1 == 0) || (k2 == (Command_Len - 1))) return -1; else return k1;
}