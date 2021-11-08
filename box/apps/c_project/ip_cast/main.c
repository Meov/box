#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <malloc.h>  
#include <math.h>

int main(int argc,char *argv[])  
{  
  
    const char *ip ="192.168.34.232";  
    char *ipstr=NULL;  
    char str_ip_index[4]={'\0'};  
    unsigned int ip_int,ip_add=0,ip_int_index[4],ip_temp_numbr=24;  
    int j =0,a=3;     
    for(unsigned int i =0;i<=strlen(ip);i++)//要用到'\0'  
    {  
        if (ip[i]=='\0'||ip[i]=='.')  
        {             
            ip_int =atoi(str_ip_index);   
            if (ip_int<0||ip_int>255)  
            {  
                printf("IP地址有误\n");  
                system("pause");  
                return 0;                 
            }  
  
            ip_add+=(ip_int*((unsigned int)pow(256.0,a)));            
            a--;  
            memset(str_ip_index,0,sizeof(str_ip_index));  
//          for (int x=0;x<4;x++)  
//          {  
//              str_ip_index[x]='\0';  
//          }  
            j=0;  
            continue;  
        }  
  
        str_ip_index[j]=ip[i];  
        j++;  
    }     
    printf("%u\n",ip_add);  
      
    //转换成x.x.x.x  
    for(j=0;j<4;j++)  
    {  
        ip_int_index[j]=(ip_add>>ip_temp_numbr)&0xFF;  
        ip_temp_numbr-=8;  
    }  
  
    if ((ipstr=(char *)malloc(17*sizeof(char)))==NULL)  
    {  
        return 0;  
    }  
      
    sprintf(ipstr,"%d.%d.%d.%d",ip_int_index[0],ip_int_index[1],ip_int_index[2],ip_int_index[3]);  
    printf("%s\n",ipstr);  
    free(ipstr);  
    ipstr=NULL;  
    system("pause");  
    return 0;  
}  
