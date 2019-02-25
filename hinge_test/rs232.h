#ifndef _RS232_H_
#define _RS232_H_
#define SERIAL_ERROR_COMPORTNUM		-1
#define SERIAL_ERROR_BAUDRATE		-2
#define SERIAL_ERROR_PARITY			-3
#define SERIAL_ERROR_STOPBIT		-4
#define SERIAL_ERROR_READEVENT		-5
#define SERIAL_ERROR_WRITEEVENT		-6
#define SERIAL_ERROR_COMPORT		-7
#define SERIAL_ERROR_COMCLOSE		-10

#define COM_BUFFER_SIZE				4192

class RS232
{
public:
	RS232();
	~RS232();
	int com_open(int port, int baud_rate, int parity = NOPARITY, int stop_bit = ONESTOPBIT);
	int com_close(void);

	DWORD receive_data(unsigned char *buffer, DWORD limited_data_size, COMSTAT &cs);
	DWORD receive_data_loop(unsigned char *buffer, DWORD limit_data_size, int &mode);
	DWORD send_data(const unsigned char *buffer, DWORD sending_data_size);
	DWORD send_data_feedback(const unsigned char *input, DWORD input_size, unsigned char *output, DWORD output_size);

	HANDLE get_com_port(void);
	LPOVERLAPPED get_olread(void);
	LPOVERLAPPED get_olwrite(void);

	WORD generate_crc(BYTE *cmd,WORD data_length);

private:
	HANDLE hcom;
	OVERLAPPED olread, olwrite;

	BOOL check_com_port(int port, char *com_name);
	BOOL check_baud_rate(int baud_rate);
	BOOL check_parity(int parity);
	BOOL check_stop_bit(int stop_bit);
};



#endif