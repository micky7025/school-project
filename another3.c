#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main () {
	char data [4];
	int i, j, k=1;
	for(i=0; i<=4; i++) {
		j=0;
		do {
			if(!j) printf("請輸入第%d個數字", i+1);
			else printf("請重新輸入第%d個數字", i+1);
			scanf("%c", &data[i]);
			fflush(stdin);
			j=1;
		} while(!isdigit(data[i]));
	}
	for(i=0; i<=4; i++) {
		printf("%d", data[i]);
	}
	return 0;
}
