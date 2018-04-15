#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "socketLog.h"

#define LOG_FILE "socketclient.log"
#define MAX_STRING_LEN 40960

/*Level*/
#define NO_LOG_LEVEL 0
#define DEBUG_LOG_FILE 1
#define INFO_LOG_LEVEL 2
#define WARNING_LOG_LEVEL 3
#define ERROR_LOG_LEVEL 4

int LogLevel[5] = {NO_LOG_LEVEL, DEBUG_LOG_FILE,
			INFO_LOG_LEVEL, 
			WARNING_LOG_LEVEL,
			ERROR_LOG_LEVEL};
char LogLevelName = {"NOLOG", "DEBUG", "INFO", "WARNING", "ERROR"};

//get the log time
static int Socket_log_CurTime(char * strTime)
{
	struct tm * tmTime = NULL;
	size_t timeLen = 0;
	time_t tTime = 0; 
	
	if(strTime == NULL)
	{
		perror("strTime parameter is NULL");
		return -1;
	}
	tTime = time(NULL);
	if(tTime < 0)
	{
		perror("time() err");
		return -1;
	}
	tmTime = localtime(&tTime);
	if(tmTime == NULL)
	{
		perror("localtime() err");
		return -1;
	}
	timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", 
				tmTime);
	return timeLen;
	
}

static int Socket_log_File(int * pf)
{
	char fileName[1024];
	int sprintf_res;
	memset(fileName, 0, sizeof(fileName));
#ifdef WIN32
	sprintf_res = sprintf(fileName,  "c:\\socket_log\\%s", 
				LOG_FILE);
	if(sprintf_res < 0)
	{
		perror("sprintf() err");
		return -1;
	}
#else
	sprintf_res = sprintf(fileName, "%s/log/%s",
				getenv("HOME"), LOG_FILE);
	if(sprintf_res < 0)
	{
		perror("sprintf() err");
		return -1;
	}
#endif
	*pf = open(fileName, O_WRONLY|O_CREAT|O_APPEND, 0666);
	if(*pf < 0)
	{
		perror("open() failed");
		return -1;
	}
	return 0;
}

static void Socket_Error_Core_Content(const char *file, 
					int line,
					int level,
					int status,
					const char *fmt,
					va_list args)
{
	char str[MAX_STRING_LEN];
	int strLen = 0;
	char tmpStr[64];
	int tmpStrLen = 0;
	int write_res = 0;
	int pf = 0;

	memset(str, 0, MAX_STRING_LEN);
	memset(tmpStr, 0, 64);
	
	/*Add into the log time*/
	tmpStrLen = Socket_log_CurTime(tmpStr);
	if(tmpStrLen < 0)
	{
		perror("Socket_log_CurTime() err");
		return;
	}
	tmpStrLen = sprintf(str, "[%s] ", tmpStr);
	if(tmpStrLen < 0)
	{
		perror("sprintf() in Socket_Error_Core_Content err");
		return;
	}
	strLen = tmpStrLen;

	/*Add into the log level*/
	tmpStrLen = sprintf(str + strLen, "[%s] ", 
				LogLevelName[level]);
	if(tmpStrLen < 0)
	{
		perror("sprintf() in 
			Socket_Error_Core_Content err2");
		return;
	}
	strLen += tmpStrLen;
	
	/*Add into the log status*/
	if(status != 0)
	{
		tmpStrLen = sprintf(str+strLen, "[ERRNO is %d] ",
					status);
		if(tmpStrLen < 0)
		{
			perror("sprintf() in 
				Socket_Error_Core_content err3");
			return;
		}
	}
	else
	{
		tmpStrLen = sprintf(str+strLen, "[SUCCESS] ");
		if(tmpStrLen < 0)
		{
			perror("sprintf() in
				Socket_Error_Core_content err4");
			return;
		}
	}
	strLen += tmpStrLen;
	
	/*Add into the log message*/
	tmpStrLen = vsprintf(str+strLen, fmt, args);
	if(tmpStrLen < 0)
	{
		perror("sprintf() in
			Socket_Error_Core_content err5");
		return;
	}		
	strLen += tmpStrLen;

	/*Add into the log file*/
	tmpStrLen = sprintf(str+strLen, " [%s]", file);
	if(tmpStrLen < 0)
	{
		perror("sprintf() in
			Socket_Error_Core_content err6");
		return;
	}
	strLen += tmpStrLen;
	
	/*Add into the line numbers of log*/
	tmpStrLen = sprintf(str+strLen, " [%d]\n", line);
	if(tmpStrLen < 0)
	{
		perror("sprintf() in
			Socket_Error_Core_content err7");
		return;
	}
	strLen += tmpStrLen;

	/*open the log file*/	
	if(Socket_log_File(&pf) < 0)
	{
		perror("Socket_log_File() err");
		return;
	}
	
	/*write into the log file*/
	write_res = write(pf, str, strLen);
	if(write_res < 0)
	{
		perror("write() err");
		return;
	}
	/*close file*/
	close(pf);
	return;
}

void socket_log(const char *file, int line, int level, int status,
		const char * fmt, ...);
{
	va_list args;
	
	/*Whether or not to write into the log file*/
	if(level == NO_LOG_LEVEL)
	{
		return;
	}
	
	va_start(args, fmt);
	Socket_Error_Core_Content(file, line, level, 
				status, fmt, args);
	va_end(args);

	return;
}
