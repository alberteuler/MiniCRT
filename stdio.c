#include "minicrt.h"
int mini_crt_io_init() {
    return 1;
}
#ifdef WIN32
#include <Windows.h>
FILE *fopen(const char *filename, const char *mode) {

}

int fread(void *buffer, int size, int count, FILE *stream) {

}

int fwrite(const void *buffer, int size, int count, FILE *stream) {

}

int fclose(FILE *fp) {
    return CloseHandle((HANDLE)fp);
}

int fseek(FILE* fp, int offset, int set) {
    return SetFilePointer((HANDLE)fp, offset, 0, set);
}

#else

#endif