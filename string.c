char *itoa(int n, char  *str, int radix) {
    char alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *p = str;
    char *head = str;
    if(!p || radix < 2 || radix > 36) return p; //p是空指针，或者进制超出可选范围
    if(radix != 10 && n < 0) return p;          //非十进制的复数
    if(n==0) {
        *p++ = '0';
        *p = 0;
        return p;
    }
    if(radix == 10 && n < 0){
        *p++ = '-';
        n = -n;
    }
}

int strcmp(const char *src, const char *dst) {

}
char *strcpy(char *dest, const char *src) {

}
unsigned strlen(const char *str) {
    
}