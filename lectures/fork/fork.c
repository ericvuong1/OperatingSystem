#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	if(fork()){
			printf("I'm parent\n");
	} else {
	printf("I'm child");
	}
}
