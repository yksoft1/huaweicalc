//
//  Калькулятор nlock-кодов для модемов Huawei 
//
//  Автор - forth32  
//  2014 год
//
//  Ветка для вычисления флеш-кодов
//
#ifndef USE_OWN_MD5
#include <openssl/md5.h>
#else
#include "md5.h"
#endif
#include "encrypt.h"

void encrypt_v1(char* imei, char* resbuf,char* hstr) {
  
unsigned char xbytes[17];
char ybytes[100];
char hash[100];
unsigned int rh[30];
unsigned char res[4];

int i;

#ifdef USE_OWN_MD5
struct MD5Context context;
#endif

memset(xbytes,0,17);

#ifndef USE_OWN_MD5
MD5((unsigned char*)hstr,strlen(hstr),xbytes);
#else
MD5Init (&context);
MD5Update (&context, (unsigned char*)hstr, strlen (hstr));
MD5Final (xbytes, &context);
#endif

//printf("\n xbytes (1) =\n");
//for(i=0;i<16;i++) printf(" %02x",xbytes[i]&0xff);
for(i=0;i<16;i++) sprintf(ybytes+(i*2),"%02x",xbytes[i]&0xff);

strcpy(hash,imei);
strncat(hash,ybytes+8,16);
hash[31]=0;

#ifndef USE_OWN_MD5
MD5((unsigned char*)hash,31,xbytes);
#else
MD5Init (&context);
MD5Update (&context, (unsigned char*)hash, 31);
MD5Final (xbytes, &context);
#endif

for (i=0;i<16;i++) rh[i]=xbytes[i]&0xff;
for(i=0;i<4;i++) res[3-i]=rh[i]^rh[i+4]^rh[i+8]^rh[i+12];
i=*((unsigned int*)&res);
i|=0x2000000;
i&=0x3FFFFFF;
sprintf(resbuf,"%i",i);
}
