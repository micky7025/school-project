#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main (){
    int data [4];
    int i, j, k=1;
    for(i=0; i<=4; i++){
        printf("請輸入第%d個數字", i+1);
        scanf("%d",&data[i]);
		j = isdigit(data[i]);
		
		
		
			while(j=0){
			fflush(stdin);	
			printf("請重新輸入第%d個數字", i+1);
			scanf("%d",&data[i]);
			j = isdigit(data[i]);
			if(j!=0)
			break;
			}
		}
		
		
		
    
    
    
    
    
    for(i=0; i<=4; i++){
    printf("%d", data[i]);
    }    
        
        
    return 0;
}

