#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socketclient.h"

int myEncData(unsigned char *inData, int inDataLen,
		unsigned char *outData, int *outDataLen
		void *Ref, int RefLen)
{
	memcpy(outData, "111111111111111111111111111111111", 10);
	*outDataLen = 10;
	return 0;
}

int myDecData(unsigned char *inData,int inDataLen,
		unsigned char *outData,int *outDataLen,
		void *Ref, int RefLen)
{
	memcpy(outData, "2222222222222222222222222222222", 10);
	*outDataLen = 10;
	return 0;
}

int main(void)
{
	int rv = 0;
	void *handle = NULL;
	unsigned char buf[2048];
	int buflen = 0;
	unsigned char buf2[2048]={0};
	int buflen = 0;

	strcpy(buf, "AAAAAAAAAAAAAAAAAAAAA");
	buflen = 10;
	
	rv = cltSocketInit(&handle);
	if(rv != 0)
	{
		printf("func cltSocketInit():%d", rv);
		return rv;
	}
	rv = clitSocket_SetEncFunc(handle, myEncData, NULL, 0);
	if(rv != 0)
	{
		printf("func cltSocketInit():%d", rv);
		goto END;
	}
	rv = cltSocketSend(handle, buf,  buflen);
	if(rv != 0)
	{
		printf("func cltSocketSend():%d", rv);
		goto END;
	}
	rv = clitSocket_SetDecFunc(handle, myDecData, NULL, 0);
	if(rv != 0)
	{
		printf("func cltSocketInit():%d", rv);
		goto END;
	}
	rv = cltSocketRev(handle, buf2 , &buflen2);
	if(rv != 0)
	{
		printf("func cltSocketRev():%d", rv);
		goto END;
	}
	printf("\n%s", buf2);
END:
	rv = cltSocketDestory(handle);
	if(rv != 0)
	{
		printf("func cltSocketDestory():%d", rv);
		return rv;	
	}
	return 0;
}
