/*written by Yuan 04/14/2018*/

/*socketlog.h*/

#ifndef _SOCKET_LOG_
#define _SOCKET_LOG_

/*
int line: is the line number of the file
int level: is the level of error
	0 --- no log
	1 --- debug
	2 --- info
	3 --- warning 
	4 --- err
int status: the error code
onst char *fmt: optional parameter
*/
extern int LogLevel[5];
void socket_log(const char *file, int line, int level, int status,
		const char * fmt, ...);

#endif

