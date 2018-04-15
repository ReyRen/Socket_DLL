#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket_common.h"
#include "socketclient.h"
#include "socketLog.h"

typedef struct _SCK_HANDLE
{
	char version[16];
	char serverip[16];
	int serverport;
	unsigned char * buf;
	int buflen;
	
	EncData encDataCallbak;
	void *Encref;
	int EncLen;
	DecData decDataCallbak;
	void *Decref;
	int DecLen;

}SCK_HANDLE;

SOCKET_FUNC_EXPORT(int)
clitSocket_SetEncFunc(void *handle, EncData encDataCallbak, 
			void *ref, int refLen)
{
	int ret = 0;
	SCK_HANDLE *sh = NULL;
	if(handle == NULL)
	{
		ret = -1;
		return ret;
	}
	sh = (SCK_HANDLE *)handle;
	sh->encDataCallbak = encDataCallbak;
	if(ref != NULL && refLen > 0)
	{
		sh->Encref = (void *)malloc(refLen);
		if(sh->Encref == NULL)
		{
			ret = -2;
			return ret;
		}
		memcpy(sh->Encref, ref, refLen);
		sh->EncLen = refLen;
	}
	return ret;
}

SOCKET_FUNC_EXPORT(int)
clitSocket_SetDecFunc(void *handle, DecData decDataCallbak, 
			void *ref, int refLen)
{
	int ret = 0;
	SCK_HANDLE *sh = NULL;
	if(handle == NULL)
	{
		ret = -1;
		return ret;
	}
	sh = (SCK_HANDLE *)handle;
	sh->decDataCallbak = decDataCallbak;
	if(ref != NULL && refLen > 0)
	{
		sh->Decref = (void *)malloc(refLen);
		if(sh->Decref == NULL)
		{
			ret = -2;
			return ret;
		}
		memcpy(sh->Decref, ref, refLen);
		sh->Decref = refLen;
	}
	return ret;
}

/*client initialization*/
SOCKET_FUNC_EXPORT(int)
cltSocketInit(void **handle)
{
	SCK_HANDLE *sh = NULL;
	int rv = 0;

	socket_log(__FILE__, __LINE__, LogLevel[1], rv, 
			"FUNC cltSocketInit() BEGIN");
	if(handle == NULL)
	{
		rv = -1;
		socket_log(__FILE__, __LINE__, 
			LogLevel[4], rv, 
			"FUNC cltSocketInit() check handle==NULL [%d]", rv);
		return rv;
	}
	
	/*alloc memory*/
	sh = (SCK_HANDLE *)malloc(sizeof(SCK_HANDLE));
	if(sh == NULL)
	{
		rv = -2;
		socket_log(__FILE__, __LINE__,
			LogLevel[4], rv,
			"FUNC cltSocketInit() malloc error [%d]", rv);
		return rv;
	}
	memset(sh, 0, sizeof(SCK_HANDLE));
	strcpy(sh->version, "0.0.1");
	strcpy(sh->serverip, "10.211.55.9");
	sh->serverport = 8888;

	*handle = sh;

	socket_log(__FILE__, __LINE__, LogLevel[1], rv,
		"FUNC cltSocketInit() END");
	return rv;
}

/*send data from client*/
SOCKET_FUNC_EXPORT(int)
cltSocketSend(void *handle, unsigned char *buf, int buflen)
{
	int rv = 0;
	SCK_HANDLE *sh = NULL;
	EncData encDataCallbak;

	if(handle == NULL)
	{
		rv = -4;
		socket_log(__FILE__, __LINE__,
			LogLevel[4], rv,
			"FUNC cltSocketSend() check handle==NULL [%d]", rv);
		return rv;
	}
	if(buf == NULL || buflen <= 0)
	{
		rv = -5;
		socket_log(__FILE__, __LINE__,
			LogLevel[4], rv,
			"FUNC cltSocketSend() buf == NULL || buflen <= 0 [%d]", rv);
		return rv;
	}
	
	sh = (SCK_HANDLE *)handle;
	sh->buf = (unsigned char *)malloc(sizeof(char)*buflen + 128);
	if( sh->buf == NULL)
	{
		rv = -6;
		socket_log(__FILE__, __LINE__,
			LogLevel[4], rv,
			"FUNC cltSocketSend() buflen:[%d]", buflen);
		socket_log(__FILE__, __LINE__,
			LogLevel[4], rv,
			"FUNC cltSocketSend() sh->buflen=0: [%d]", rv);
		return rv;
	}
	/*Encrypted Data, set the third pirty function*/
	if(sh->encDataCallbak != NULL)
	{
		encDataCallbak = sh->encDataCallbak;
		rv = encDataCallbak(buf, buflen, sh->buf, &sh->buflen
				sh->Encref, sh->EncLen);
		if(rv != 0)
		{
			socket_log(__FILE__, __LINE__,
				LogLevel[4], rv, 
				"FUNC encDataCallbak() buflen:%d", buflen);
			return rv;
		}
	}
	else
	{
		memcpy(sh->buf, buf, buflen);
		sh->buflen = buflen;
	}

	return rv;
}

/*get data for client*/
SOCKET_FUNC_EXPORT(int)
cltSocketRev(void *handle, unsigned char *buf, int *buflen)
{
	int rv = 0;
	SCK_HANDLE *sh = NULL;
	EncData decDataCallbak;

	if(handle == NULL)
	{
		rv = -4;
		socket_log(__FILE__, __LINE__,
			LogLevel[4], rv,
			"FUNC cltSocketRev() check handle==NULL [%d]", rv);
		return rv;
	}
	if(buflen == NULL)
	{
		rv = -5;
		socket_log(__FILE__, __LINE__,
			 "FUNC cltSocketRev() buflen=NULL [%d]", rv);
		return rv;
	}
	sh = (SCK_HANDLE *)handle;
	if(sh->decDataCallbak != NULL)
	{
		decDataCallbak = sh->decDataCallbak;
		rv = decDataCallbak(buf, buflen, sh->buf, &sh->buflen,
					sh->Decref, sh->DecLen);
		if(rv != 0)
		{
			socket_log(__FILE__, __LINE__,
				LogLevel[4], rv,
				"FUNC decDataCallbak() buflen:%d", buflen);
			return rv;
		}
		else
		{
			memcpy(sh->buf, buf, buflen);
			sh->buflen = buflen;
		}
	}
/*
	if (buf != NULL)
	{
		memcpy(buf, sh->buf, sh->buflen);
	}
	*buflen = sh->buflen;
*/
	return rv;
}
/*release the resource*/
SOCKET_FUNC_EXPORT(int)
cltSocketDestory(void *handle)
{
	SCK_HANDLE *sh = NULL;
	sh = handle; 
	if(sh != NULL)
	{
		if(sh->Encref != NULL)
		{
			free(sh->Encref);
		}
		if(sh->Decref != NULL)
		{
			free(sh->Decref);
		}
		if(sh->buf != NULL)
		{
			free(sh->buf);
		}
		free(sh);
	}
	return 0;
}
