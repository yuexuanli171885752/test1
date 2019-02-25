#define BCCWIN
#define DOEXPORT
#include "nodavesimple.h"
#include "openSocket.h"
#include "tcp_ip.h"

//#include "interface.h"  
/*quote to interface.h will have conflict issue with datastruct.h*/
#pragma comment(lib, "libnodave.lib")

#define RACK		0
#define SLOT		1
#define PLC_PORT	102

_daveOSserialType fds;
daveInterface *di = NULL;
daveConnection *dc = NULL;

HANDLE plc_mutex = NULL;

extern void log(const char *format, ...);

int connect_to_plc(char *ip)
{
	int result = FALSE;
	fds.rfd = openSocket(PLC_PORT, ip);
	fds.wfd = fds.rfd;
	if (fds.rfd > 0){
		log("open socket[%s:%d] OK.", ip, PLC_PORT);
		result = TRUE;
	}
	else{
		//log("open socket[%s:%d] Fail.", ip, PLC_PORT);
		//***********************
		result = FALSE;
	}

	if (result == TRUE){
		di = daveNewInterface(fds, "myPLC", daveProtoMPI, daveProtoISOTCP, daveSpeed187k);
		int rt = daveInitAdapter(di);
		if (rt == 0){
			dc = daveNewConnection(di, 2, RACK, SLOT);
			rt = daveConnectPLC(dc);
			if (rt == 0){
				log("connected to PLC.");
				result = TRUE;
			}
			else{
				log("connect to PLC failed.");
				result = FALSE;
			}
		}
		else{
			log("init adapter fail.");
			result = FALSE;
		}
	}

	return result;
}

int disconnect_from_plc()
{
	int result = FALSE;
	if (dc != NULL){
		result = daveDisconnectPLC(dc);
		daveFree(dc);
		dc = NULL;
	}
	if (di != NULL){
		result = daveDisconnectAdapter(di);
		daveFree(dc);
		di = NULL;
	}
	if (fds.rfd != NULL){
		result = closeSocket(fds.rfd);
		fds.rfd = NULL;
	}

	return result;
}

int read_from_plc_db(int index, int start, int lens, BYTE *buf)
{
	WaitForSingleObject(plc_mutex, INFINITE);
	int result = 0;
	try
	{
		result = daveReadBytes(dc, daveDB, index, start, lens, buf);
	}
	catch (...)
	{
		log("read catch err.");
	}

	if (result != 0x00){
// 		char *mystr = NULL;
// 		mystr = daveStrerror(result);
//		log("read err:%s, index=%d, start=%d", mystr, index, start);
		log("read err.");
	}

	ReleaseMutex(plc_mutex);
//	log("release read mutex.");
	Sleep(100);
	return result;
}

int write_to_plc_db(int index, int start, int lens, void *buf)
{
	WaitForSingleObject(plc_mutex, INFINITE);
	int result = 0;
	BYTE* read = 0;
	try
	{
		result = daveWriteBytes(dc, daveDB, index, start, lens, buf);
	}
	catch (...)
	{
		log("write catch err.");
	}
// 	while(1){
// 		result = read_from_plc_db(index, start, lens, read);
// 		log("write err.");
// 		if(read == buf) break;
// 		result = daveWriteBytes(dc, daveDB, index, start, lens, buf);
// 	}
	if (result != 0x00){
// 		char *mystr = NULL;
// 		mystr = daveStrerror(result);
// 		log("write err:%s, index=%d, start=%d", mystr, index, start);
		log("write err.");
	}

	ReleaseMutex(plc_mutex);
//	log("release write mutex.");
	Sleep(100);
	return result;
}



