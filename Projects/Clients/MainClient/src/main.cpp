#include "MainClient.h"

CMainClient client;

int __cdecl ace_main_i(int,char * * const){
	return client.Run();
}

int main(){
	return client.Run();
}