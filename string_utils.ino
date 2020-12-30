#include <string.h>
#include <ctype.h>

char* append(char* str, char chr) {
    size_t len = strlen(str);
    char *str2 = malloc(len + 1 + 1 ); /* one for extra char, one for trailing zero */
    strcpy(str2, str);
    str2[len] = chr;
    str2[len + 1] = '\0';
    return str2;
}

bool isNumeric(char* str){
  for (int i=0; i<strlen(str); i++){
    if (isdigit(str[i]) == 0){
      return false;
    }
  }
  return true;
}
