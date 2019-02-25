#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include "rs232.h"


RS232::RS232()
{
	hcom = NULL;
}

RS232::~RS232()
{
	if (hcom != NULL){
		CloseHandle(hcom);
	}
}

int RS232::com_open(int port, int baud_rate, int parity, int stop_bit)
{
	char com_name[10] = {0};

	if (!check_com_port(port, com_name))   return SERIAL_ERROR_COMPORTNUM;
	if (!check_baud_rate(baud_rate))	   return SERIAL_ERROR_BAUDRATE;
	if (!check_parity(parity))			   return SERIAL_ERROR_PARITY;
	if (!check_stop_bit(stop_bit))		   return SERIAL_ERROR_STOPBIT;

	olread.Offset = 0;
	olread.OffsetHigh = 0;
	if (!(olread.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))){
		return SERIAL_ERROR_READEVENT;
	}

	olwrite.Offset = 0;
	olwrite.OffsetHigh = 0;
	if (!(olwrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))){
		return SERIAL_ERROR_WRITEEVENT;
	}

	if (hcom != NULL){
		CloseHandle(hcom);
		hcom = NULL;
	}

	hcom = CreateFile(com_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hcom == INVALID_HANDLE_VALUE) return SERIAL_ERROR_COMPORT;

	if (SetCommMask(hcom, EV_RXCHAR)){
		if (SetupComm(hcom, COM_BUFFER_SIZE, COM_BUFFER_SIZE)){
			if (PurgeComm(hcom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)){
				COMMTIMEOUTS cts;
				cts.ReadIntervalTimeout = 0XFFFFFFFF;
				cts.ReadTotalTimeoutMultiplier	= 0;
				cts.ReadTotalTimeoutConstant	= 0;
				cts.WriteTotalTimeoutMultiplier = 0;
				cts.WriteTotalTimeoutConstant	= 0;
				if (SetCommTimeouts(hcom, &cts)){
					DCB dcb;
					if (GetCommState(hcom, &dcb)){
						dcb.DCBlength = sizeof(DCB);
						dcb.BaudRate  = baud_rate;
						dcb.ByteSize  = 8;
						dcb.Parity    = parity;
						dcb.StopBits  = stop_bit;

						if (SetCommState(hcom, &dcb)){
							return TRUE;
						}	
					}	
				} //SetCommTimeouts
			} 
		} //SetupComm
	}
	return FALSE;
}

int RS232::com_close()
{
	if (hcom == NULL)	return TRUE;

	if (SetCommMask(hcom, 0)){
		if (EscapeCommFunction(hcom, CLRDTR)){
			if (PurgeComm(hcom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR)){
				if (olwrite.hEvent != NULL){
					CloseHandle(olread.hEvent);
					CloseHandle(olwrite.hEvent);
				}
				if (CloseHandle(hcom)){
					hcom = NULL;
					olread.hEvent = NULL;
					olwrite.hEvent = NULL;
					return TRUE;
				}
			}
		}
	}

	return SERIAL_ERROR_COMCLOSE;
}

DWORD RS232::receive_data(unsigned char *buffer, DWORD limited_data_size, COMSTAT &cs)
{
	if (hcom == NULL){
		return 0;
	}
	DWORD error_flag;
	DWORD received_data_size;

	if (!ReadFile(hcom, buffer, limited_data_size, &received_data_size, &olread)){
		DWORD err_code = GetLastError();
		if (ERROR_IO_PENDING == err_code){
			while (!GetOverlappedResult(hcom, &olread, &received_data_size, TRUE)){
				if (ERROR_IO_INCOMPLETE != err_code){
					//XTRACE("receive_data:ERROR IO INCOMPLETE.\n");
					received_data_size = 0;
					break;
				}
			}
		}
		else{
			//XTRACE("receive_data:other error[%d].\n", err_code);
			received_data_size = 0;
		}
	}
	::ClearCommError(hcom, &error_flag, &cs);
	PurgeComm(hcom, PURGE_RXCLEAR|PURGE_TXCLEAR);
	return received_data_size;
}

DWORD RS232::receive_data_loop(unsigned char *buffer, DWORD limit_data_size, int &mode)
{
	DWORD mask;
	DWORD error_flag = 0;
	int received_data_size = 0;

	BYTE *receive_buffer = new BYTE[limit_data_size];

	COMSTAT cs;

	WaitCommEvent(hcom, &mask, &olread);
	WaitForSingleObject(olread.hEvent, 1000);
	ResetEvent(olread.hEvent);
	if ((mask&EV_RXCHAR) == EV_RXCHAR){
		::ClearCommError(hcom, &error_flag, &cs);
		while (cs.cbInQue){
			Sleep(50);
			received_data_size = receive_data(receive_buffer, limit_data_size, cs);
			if (received_data_size>0){
				if (mode == 0){
					memcpy(buffer, receive_buffer, limit_data_size);
				}
				else{
					char buf[10] = {0};
					for (int i=0; i<received_data_size; i++){
						sprintf(buf, "0x%x ", receive_buffer[i]);
						strncat((char *)buffer, buf, strlen((char *)buf));
					}
				}
				return received_data_size;
			}
		}
	}

	delete [] receive_buffer;
	return 0;
}

DWORD RS232::send_data(const unsigned char *buffer, DWORD sending_data_size)
{
	if (hcom == NULL)	return -1;

	DWORD write_data_size = 0;
	DWORD error_flags = 0;
	COMSTAT cs;
	if (!WriteFile(hcom, buffer, sending_data_size, &write_data_size, &olwrite)){
		if (ERROR_IO_PENDING == GetLastError()){
			while (!GetOverlappedResult(hcom, &olwrite, &write_data_size, TRUE)){
				if (ERROR_IO_INCOMPLETE != GetLastError()){
					write_data_size = -2;
					ClearCommError(hcom, &error_flags, &cs);
					break;
				}
			}
		}
		else{
			write_data_size = -3;
			ClearCommError(hcom, &error_flags, &cs);
		}
	}
	return write_data_size;
}

DWORD RS232::send_data_feedback(const unsigned char *input, DWORD input_size, unsigned char *output, DWORD output_size)
{
	int result = 0;
	if (hcom == NULL)	return -1;

	DWORD write_data_size = 0;
	DWORD error_flags = 0;
	COMSTAT cs;

	if (!WriteFile(hcom, input, input_size, &write_data_size, &olwrite)){
		if (ERROR_IO_PENDING == GetLastError()){
			while (!GetOverlappedResult(hcom, &olwrite, &write_data_size, TRUE)){
				if (ERROR_IO_INCOMPLETE != GetLastError()){
					result = -2;
					ClearCommError(hcom, &error_flags, &cs);
					break;
				}
			}
		}
		else{
			result = -3;
			ClearCommError(hcom, &error_flags, &cs);
		}
	}

	int timeout = 5;
	int length = 0;
	int mode = 0;
	while ((timeout--)>0){
		length = receive_data_loop(output, output_size, mode);
		if (length>0){
			result = TRUE;
			break;
		}
		Sleep(1);
	}

	if (timeout<=0) result = FALSE;

	return result;
}


HANDLE RS232::get_com_port()
{
	return hcom;
}

LPOVERLAPPED RS232::get_olread()
{
	return &olread;
}

LPOVERLAPPED RS232::get_olwrite()
{
	return &olwrite;
}

BOOL RS232::check_com_port(int port, char *com_name)
{
	if (com_name == NULL)	return FALSE;

	memset(com_name, 0, sizeof(com_name));

	if (port <= 0 || port >256){
		return FALSE;
	}
	else{
		char buffer[10] = {0};

		wsprintf(buffer, "COM%d", port);
		strcpy(com_name, buffer);

		return TRUE;
	}
}

BOOL RS232::check_baud_rate(int baud_rate)
{
	switch (baud_rate)
	{
	case 9600:
	case 14400:
	case 19200:
	case 38400:
	case 57600:
	case 115200:
	case 128000:
	case 256000:
	case 921600:
		return TRUE;

	default:
		return FALSE;
	}
}

BOOL RS232::check_parity(int parity)
{
	switch(parity)
	{
	case NOPARITY:
	case ODDPARITY:
	case EVENPARITY:
	case MARKPARITY:
	case SPACEPARITY:
		return TRUE;

	default:
		return FALSE;
	}
}

BOOL RS232::check_stop_bit(int stop_bit)
{
	switch(stop_bit)
	{
	case ONESTOPBIT:
	case ONE5STOPBITS:
	case TWOSTOPBITS:
		return TRUE;

	default:
		return FALSE;
	}
}





