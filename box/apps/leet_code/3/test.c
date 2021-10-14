/*
 * @lc app=leetcode.cn id=3 lang=c
 *
 * [3] 无重复字符的最长子串
 */

// @lc code=start
#include <stdio.h>
#include <string.h>
#if 0
int lengthOfLongestSubstring(char * s){
    char *string_s = s;
    int num = 0;;
    int left = 0;
    int right = 0;
    int count = 0;
    int count_found = 0;
    int i;
    printf("enterd string :%s\n",s);

    for(right = left; s[right] != '\0';right++){
	count = 0;
        for(i = left; i < right; i++){
	   printf("left:%d right:%d\n",left,right);	
	   if(s[i] == s[right]){
                count_found = right - left;
		left = i;
           }else{
		count++; 
		}
        }

	if(num < (count > count_found ? count : count_found))
		num = count > count_found ? count : count_found;
    }

    return num;

}
#endif
int lengthOfLongestSubstring(char * s){
	char char_sets[128] = {0};
	int left = 0;
	int right = 0;
	int max_length = 0;
	int string_length = strlen(s);
	
	while(left < string_length && right < string_length){	
		if(char_sets[s[right]] == 0){
			char_sets[s[right]] = 1;
			right++;
			max_length = max_length > right-left ? max_length : right-left;	
		}else{
			char_sets[s[left]] = 0;
			left++;			
		}
	}
	return max_length;
}
int main(int argc ,int *argv[]){
	char *s = "dvdf"; 
	printf("result: %d\n",lengthOfLongestSubstring(s));
}
// @lc code=end

