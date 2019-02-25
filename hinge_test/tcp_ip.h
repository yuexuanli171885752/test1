#ifndef _TCPIP_H_
#define _TCPIP_H_

extern int connect_to_plc(char *ip);
extern int disconnect_from_plc(void);
extern int read_from_plc_db(int index, int start, int lens, BYTE *buf);
extern int write_to_plc_db (int index, int start, int lens, void *buf);



#endif