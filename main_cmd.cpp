#include "encrypt.h"
#include <ctype.h>
#ifndef _MSC_VER
#include <unistd.h>
#else
#include "vcgetopt.h"
#endif
#ifdef WIN32
#include <windows.h>
#include "w32res.h"
#endif

char imeibuf[17];
char codebuf[40];

void helptxt(char* exename) {
     printf( "\n\n *** SIM unlock code calculator for HUAWEI ***\n\n\
    %s [-f123] IMEI\n\n\
    -r - reverse IMEI\n\
    -f - generate flash-code\n\
    -1 - generate code v1\n\
    -2 - generate code v2\n\
    -3 - generate code v201\n\n",exename);
}

#ifdef WIN32
BOOL CALLBACK HuaweiDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
			SetWindowPos(hwnd, 
						 HWND_TOP, 
						 300, 
						 200, 
						 0, 0,          // Ignores size arguments. 
						 SWP_NOSIZE); 
			return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hwnd, IDCANCEL);
					break;
				case IDB_CALC:
					imeibuf[16] = '\0';
					SetDlgItemTextA(hwnd, IDT_FLASHCODE, "");
					SetDlgItemTextA(hwnd, IDT_V1CODE, "");
					SetDlgItemTextA(hwnd, IDT_V2CODE, "");
					SetDlgItemTextA(hwnd, IDT_V201CODE, "");
					
					GetDlgItemTextA(hwnd, IDT_IMEI, imeibuf, 16);
					if (strlen(imeibuf) != 15) {
						MessageBox(hwnd, "Incorrect IMEI Length", "Error", MB_ICONEXCLAMATION);
						return FALSE;
					}
					for(int i=0;i<15;i++) {
						if (!isdigit (imeibuf[i])) {
							MessageBox(hwnd, "Invalid IMEI", "Error", MB_ICONEXCLAMATION);
							return FALSE;
						}
					}

					encrypt_v1(imeibuf,codebuf,"e630upgrade");
					SetDlgItemTextA(hwnd, IDT_FLASHCODE, codebuf);

					encrypt_v1(imeibuf,codebuf,"hwe620datacard");
					SetDlgItemTextA(hwnd, IDT_V1CODE, codebuf);

					calc2(imeibuf,codebuf);
					SetDlgItemTextA(hwnd, IDT_V2CODE, codebuf);

					calc201(imeibuf,codebuf);
					SetDlgItemTextA(hwnd, IDT_V201CODE, codebuf);		
					break;
				case IDB_REVERSE:
					{
						char c;
						GetDlgItemTextA(hwnd, IDT_IMEI, imeibuf, 16);
						if (strlen(imeibuf) != 15) {
							MessageBox(hwnd, "Incorrect IMEI Length", "Error", MB_ICONEXCLAMATION);
							return FALSE;
						}

						for (int i=0;i<7;i++) {
							c=imeibuf[i];
							imeibuf[i]=imeibuf[14-i];
							imeibuf[14-i]=c;
						}
						
						SetDlgItemTextA(hwnd, IDT_IMEI, imeibuf);
					}
					break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}
#endif

int main(int argc, char* argv[]) {

char c;
int opt;
int ff=0; // флаг ключей команды
int rf=0; // флаг reverse
int i;

// Если аргументов нет - запускаем графическую морду

if (argc == 1) {  
#ifndef WIN32
	helptxt(argv[0]);
#else
	FreeConsole();
	DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HUAWEICALC), GetDesktopWindow(), HuaweiDlgProc);
#endif
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
