#ifndef _AI_LINK_H_
#define _AI_LINK_H_

#ifndef AI_EXPORT
#define AI_EXPORT __declspec(dllexport) 
#endif

namespace ai_link
{
	//////////////////////////////////////////////////////////////////////////
	//RS232
#define SERIAL_ERROR_COMPORTNUM		-1
#define SERIAL_ERROR_BAUDRATE		-2
#define SERIAL_ERROR_PARITY			-3
#define SERIAL_ERROR_STOPBIT		-4
#define SERIAL_ERROR_READEVENT		-5
#define SERIAL_ERROR_WRITEEVENT		-6
#define SERIAL_ERROR_COMPORT		-7
#define SERIAL_ERROR_COMCLOSE		-10
	
#define COM_BUFFER_SIZE				4192
	
	class AI_EXPORT RS232
	{
	public:
		RS232();
		virtual ~RS232();
		int com_open(int port, int baud_rate, int parity, int stop_bit);
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
	
	//////////////////////////////////////////////////////////////////////////
	//TCPIP
#define ERROR_CREATE_SOCKET					-1
#define ERROR_SERVER_EXISTED				-2
#define ERROR_BIND_FAIL						-3
#define ERROR_IP_ADDRESS_UNRECOGNIZED		-4
#define ERROR_CONNECT_FAIL					-5
#define ERROR_SEND_PACK_FAIL				-6
#define ERROR_ACCEPT_CLIENT_FAIL			-7	
	
#define RECEIVE_COMMUNICATION_DISCONNECT	-8
#define RECEIVE_COMMUNICATION_RESET			-9
#define RECEIVE_COMMUNICATION_ABORTED		-10
#define RECEIVE_COMMUNICATION_INTERRUPT		-11
	
	class AI_EXPORT TCPIP
	{
	public:
		TCPIP();
		virtual ~TCPIP();
		SOCKET get_server_socket(void){return server;}
		SOCKET get_client_socket(void){return client;}
		
		int build_server(int port);
		int client_connecting(const char *server_ip_address, int server_port);
		int send_pack(SOCKET handshake_socket, const char *pack, int pack_size);
		
		int server_listen(int backlog);
		SOCKET server_accept_client(char *client_ip_address);
		
		void shutdown_server(void);
		void shutdown_client(void);
		
		void close_server_socket(void);
		void close_client_socket(void);
		
		void shutdown_socket(SOCKET s);
		void close_socket(SOCKET s);
		
		int receive_pack(SOCKET handshake_socket, char *pack, int pack_size);
		
	private:
		SOCKET server, client;
		WSADATA	socket_init_data;
	};
}




#endif