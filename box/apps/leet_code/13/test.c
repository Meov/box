/*
 * @lc app=leetcode.cn id=13 lang=c
 *
 * [13] 罗马数字转整数
 */

// @lc code=start
#include<stdio.h>
#include<string.h>


int get_number(char c){
    int result = 0;
    switch (c)
    {
    case 'I':
        result = 1;
        break;
    case 'V':
        result = 5;
        break;
    case 'X':
        result = 10;
        break;
    case 'L':
        result = 50;
        break;
    case 'C':
        result = 100;
	break;
    case 'D':
        result = 500;
        break;
    case 'M':
        result = 1000;
        break;
    
    default:
        break;
    }
    return result;
}


int romanToInt(char * s){
    int s_length = strlen(s);
    int res_num = 0;
    int i = 0;
    if(strspn(s, "IVXLCDM")!=s_length) {
	    printf("Wrong format\n");
	    return -1;
    }
    if(s_length == 1) return (get_number(s[0]));	


    /*XXVII*/
    /* left should less than right */

    i = s_length-1;
    res_num = get_number(s[i]);   

    for(i = s_length-1; i >= 1; i--){

	    printf("s[%d]: %d\n",i,get_number(s[i]));

	    if(get_number(s[i]) > get_number(s[i-1])){
		    res_num -= get_number(s[i-1]);
	    }else{
		    res_num +=  get_number(s[i-1]);
	    }
    }

	return res_num;
}
// @lc code=end

int main(int argc, char *argv[]){
	int num;	
	char *s = "DCXXI";

	num = romanToInt(s);

	printf("roman: %s num: %d\n",s,num);

}
