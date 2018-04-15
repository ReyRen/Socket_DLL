/*
Written by Yuan 04/14/2018
*/

/*
Following are client socket send and receive the 
datagram. 
*/

#ifndef _SOCKET_CLIENT_
#define _SOCKET_CLIENT_

#ifdef __cplusplus
extern "C" {
#endif

/*---------API abstract function for Encrypt and Decrypt*/

typedef int (*EncData) (unsigned char * inData, int inDataLen,
			unsigned char * outData, int *outDataLen,
			void * Ref, int RefLen);
typedef int (*DecData) (unsigned char * inData, int inDatalen,
			unsigned char * outData, int *outDatalen,
			void * Ref, int RefLen);
/*put the upper DDL function into the function to let the thrid party
to implemented*/
int clitSocket_SetEncFunc(void *handle, EncData encDataCallbak,
				void *ref, int refLen);
int clitSocket_SetDecFunc(void *handle, DecData decDataCallbak,
				void * ref, int reflen);

/*---------API abstract function for Encrypt and Decrypt END*/


/*---------API version.1*/

/*Initialization the client*/
int cltSocketInit(void **handle);
/*send datagram from client*/
int cltSocketSend(void *handle, unsigned char *buf, int buflen);
/*receive datagram by client*/
int cltSocketRev(void *handle, unsigned char *buf, int *buflen);
/*release the resources*/
int cltSocketDestory(void *handle);

/*---------API version.1 END*/

/*---------API version.2*/

int cltSocketInit2(void **handle);
int cltSocketSend2(void *handle, unsigned char *buf,  int buflen);
int cltSocketRev2(void *handle, unsigned char **buf, int *buflen);
int cltSocketRev2_Free(unsigned char **buf);
int cltSocketDestory2(void **handle);

/*---------API version.2 END*/

#ifdef  __cplusplus
}
#endif

#endif/*_SOCKET_CLIENT_*/
