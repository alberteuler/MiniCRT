#ifndef __MINI_CRT_H__
#define __MINI_CRT_H__

#define asm __asm__
#ifdef __cplusplus
extern "C"{
#endif

#ifndef NULL
#define NULL (0)

#endif
void free(void *ptr);
void *malloc(unsigned size);
static int brk(void *end_data_segment);
int mini_crt_init_heap();

typedef int FILE;

#define EOF (-1)
#ifdef WIN32

#define stdin ((FILE*) GetStdHandle(STD_INPUT_HANDLE))
#define stdout ((FILE*) GetStdHandle(STD_OUTPUT_HANDLE))
#define stderr ((FILE*) GetStdHandle(STD_ERROR_HANDLE))

#else
#define stdin ((FILE*)0) 
#define stdout ((FILE*)1)
#define stderr ((FILE*)2)
#endif

//printf
int fputc(int c, FILE *stream);
int fputs(const char *str, FILE *stream);
int printf(const char*format, ...);
int fprintf(FILE *stream, const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif 