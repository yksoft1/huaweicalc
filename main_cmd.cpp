#include "encrypt.h"
#include <unistd.h>

void helptxt(char* exename) {
     printf( "\n\n *** SIM unlock code calculator for HUAWEI ***\n\n\
    %s [-f123] IMEI\n\n\
    -r - reverse IMEI\n\
    -f - generate flash-code\n\
    -1 - generate code v1\n\
    -2 - generate code v2\n\
    -3 - generate code v201\n\n",exename);
}

int main(int argc, char* argv[]) {

char codebuf[40];
char imeibuf[16];
char c;
int opt;
int ff=0; // флаг ключей команды
int rf=0; // флаг reverse
int i;

// Если аргументов нет - запускаем графическую морду

if (argc == 1) {  
	helptxt(argv[0]);
	return 0;
}

// аргументы есть - работаем в консольном режиме

while ((opt = getopt(argc, argv, "rRfF123hH")) != -1) {
  switch (opt) {
   case 'h':
   case 'H':
	helptxt(argv[0]);
    return 0;
   case 'f':
   case 'F':
     ff=1;
     break;
   case '1':
     ff=2;
     break;
   case '2':
     ff=3;
     break;
   case '3':
     ff=4;
     break;
   case 'r':
   case 'R':
     rf=1;
     break;
  }
}   


if (optind >= argc) {
  printf("\n No IMEI specified\n\n");
  return 0;
}

strcpy(imeibuf,argv[optind]);

// Разворот imei, если надо
if (rf != 0) 
  for (i=0;i<7;i++) {
    c=imeibuf[i];
    imeibuf[i]=imeibuf[14-i];
    imeibuf[14-i]=c;
  }


if (strlen(imeibuf) != 15) {
  printf("\n Incorrect IMEI Length\n");
  return 0;
}

switch (ff) {
  case 0: {

  // режим без ключей - вывдим форматирванный результат
  encrypt_v1(imeibuf,codebuf,"e630upgrade");
  printf("\n Flash code        = %s",codebuf);

  encrypt_v1(imeibuf,codebuf,"hwe620datacard");
  printf("\n Encrypt_v1 code   = %s",codebuf);

  calc2(imeibuf,codebuf);
  printf("\n Encrypt_v2 code   = %s",codebuf);

  calc201(imeibuf,codebuf);
  printf("\n Encrypt_v201 code = %s\n\n",codebuf);
  return 0;
 }

// режим с ключами - вывди только отдельные цифры

 case 1: {
   encrypt_v1(imeibuf,codebuf,"e630upgrade");
   break;
 }

 case 2: {
  encrypt_v1(imeibuf,codebuf,"hwe620datacard");
  break;
}

 case 3: {
  calc2(imeibuf,codebuf);
  break;
}

 case 4: {
  calc201(imeibuf,codebuf);
  break;
}
}
printf("%s\n",codebuf);

}
