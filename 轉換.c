#include<stdio.h>

int input,output,i,data[20],num,n,judge;

void doit(int input){
    
    num = output = input;
    while(num!=0){
        num/=16;
        n++;
    }
    n*=4;
    printf("\n二進制位元數為:%d\n",n);
    for(i=0;i<n;i++){
        data[n-i-1]=input%2;
        input/=2;
    }
    
    printf("十進制為%d,二進制為:",output);
    for(i=0;i<n;i++)
        printf("%d",data[i]);
    printf("\n______________________________\n");
}


void Jud(int judge){
    
    if(judge==1){
        for(i=0;i<n;i++){
        if(data[i]==0)
            data[i]=1;
        else
            data[i]=0;
        }
        
        data[n-1]+=1;    
        for(i=0;i<n;i++){
            if(data[i]==2){
                data[i]=0;
                data[i-1]+=1;
            }
        }                   
    }
    printf("\n輸入值的二的補數為:");   
    for(i=0;i<n;i++)
        printf("%d",data[i]);
   	printf("\n______________________________\n");
    
}

int main(){
    
	printf("\n______________________________\n");    
    printf("輸入一十六進制值");
    scanf("%x", &input);
    do{
    	printf("輸入正負位元");
    	scanf("%x", &judge);
    	if(judge>1||judge<0)
    		printf("請輸入正確的正負位元\n");
	}while(judge>1||judge<0);
    doit(input);
    Jud(judge);
    
    return 0;
}
