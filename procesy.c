#include <stdio.h>
#include <Windows.h>

int ile(char *tab)
{
	int i;
	for(i=0;tab[i]!=0;i++);
	return i;
}

int main(int argc, char *argv[])
{
	//printf("\nuruchomienie pid: %d, argv[0]: %s\n",GetCurrentProcessId(),argv[0]);
	if(argc!=2)
	{
		int i;
		for(i=0;i<argc;i++)
		{
			//printf("%i %s \n",i,argv[i]);
		}
		return 1;
	}
	//printf("argv[1]: %s\n\n",argv[1]);
	char *tab=argv[1]; 
	int il=ile(tab);
	if(il<1) return 2;
	else if(il==1)
	{
		//pojedyńczy
		return tab[0];
	}
	else if(il<=20)
	{
		//podiał
		//potrzebne zmienne do stworzenia procesu
		STARTUPINFO lpStartupInfo={sizeof(lpStartupInfo)},lpStartupInfo2={sizeof(lpStartupInfo2)};
		PROCESS_INFORMATION lpProcessInformation,lpProcessInformation2;
		int il2=il%2;//czy nieparzysta ilość liter 
		il/=2;//połowa ilości liter
		il2+=il;//połowa wilekości liter + 1 jeśli nieparzyste
		char pierszy[il+1], drugi[il2+1];//zmienne dla nowych procesów
		int j=0;//licznik orgialnego argumętu
		int i;
		//tworzenie pierwszego argumętu
		for(i=0;i<il;i++)
		{
			pierszy[i]=tab[j];
			j++;
		}
		pierszy[il]=0;//tablice zkończona \0
		//tworzenie drugiego argumętu
		for(i=0;i<il2;i++)
		{
			drugi[i]=tab[j];
			j++;
		}
		drugi[il2]=0;//tablice zkończona \0
		//składanie
		int argv0size=ile(argv[0]);
		char pierszyZestaw[argv0size+il+1];
		char drugiZestaw[argv0size+il2+1];
		for(i=0;i<argv0size;i++)
		{
			pierszyZestaw[i]=argv[0][i];
			drugiZestaw[i]=argv[0][i];
		}
		pierszyZestaw[argv0size]=32;
		drugiZestaw[argv0size]=32;
		argv0size++;
		for(i=0;i<il;i++) pierszyZestaw[i+argv0size]=pierszy[i];
		for(i=0;i<il2;i++) drugiZestaw[i+argv0size]=drugi[i];
		pierszyZestaw[il+argv0size]=0;
		drugiZestaw[il2+argv0size]=0;
		//printf("     %s %i\n     %s %i\n",pierszy,il,drugi,il2);
		//printf("     %s %s\n     %s %s\n",pierszyZestaw,pierszyZestaw+argv0size,drugiZestaw,drugiZestaw+argv0size);
		//tworzenie pierwszego procesu
		if(!CreateProcessA(argv[0],pierszyZestaw,NULL,NULL,0,0,NULL,NULL,&lpStartupInfo,&lpProcessInformation))
		{
			printf("          error creating proces1: %d\n",GetLastError());
			return 3;
		}
		//tworzenie drugiego procesu
		if(!CreateProcessA(argv[0],drugiZestaw,NULL,NULL,0,0,NULL,NULL,&lpStartupInfo2,&lpProcessInformation2))
		{
			printf("          error creating proces2: %d\n",GetLastError());
			return 4;
		}
		long unsigned int returnCode,returnCode2;//zmienne do przychowywania kodu powrotu
		//printf("\n");
		//obsługa procesu 1
		WaitForSingleObject(lpProcessInformation.hProcess,INFINITE);//oczekiwanie na zakończenie
		WaitForSingleObject(lpProcessInformation2.hProcess,INFINITE);//oczekiwanie na zakończenie
		int test=GetExitCodeProcess(lpProcessInformation.hProcess,&returnCode);//pobieranie kodu zakończenia
		if(!test)
		{
			returnCode = GetLastError(); 
			printf("          test error\n",test);
			printf("%d %d %s %d \n",GetCurrentProcessId(),lpProcessInformation.dwProcessId,pierszy,returnCode);
		}
		else printf("%d %d %s %c \n",GetCurrentProcessId(),lpProcessInformation.dwProcessId,pierszy,returnCode);
		CloseHandle( lpProcessInformation.hProcess );//zamykanie uchwytów
		CloseHandle( lpProcessInformation.hThread );
		//obsługa procesu 2
		test=GetExitCodeProcess(lpProcessInformation2.hProcess,&returnCode2);//pobieranie kodu zakończenia
		if(!test)
		{
			returnCode = GetLastError(); 
			printf("          test error\n",test);
			printf("%d %d %s %d \n",GetCurrentProcessId(),lpProcessInformation2.dwProcessId,drugi,returnCode2);
		}
		else printf("%d %d %s %c \n",GetCurrentProcessId(),lpProcessInformation2.dwProcessId,drugi,returnCode2);//wypis informacji
		CloseHandle( lpProcessInformation2.hProcess );//zamykanie uchwytów
		CloseHandle( lpProcessInformation2.hThread );
		return returnCode2;
	}
    return 0;
}
