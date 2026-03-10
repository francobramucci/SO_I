#include <stdio.h>
#include <unistd.h>

int main(){
	
	while(1){
		printf("> ");
		char* s;
		fgets(s);
		char* c = strtok(s, ' ');
		while(c != NULL){
			

		}
		
	}


}
