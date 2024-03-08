#include <stdio.h>
#include <windows.h>
#define BUFFER_SIZE 25

int main(VOID) {
    HANDLE readHandle;
    CHAR charBuffer[BUFFER_SIZE];
    DWORD read;

    /* get the read handle of the pipe */
    readHandle = GetStdHandle(STD_INPUT_HANDLE);
    
    /* the child reads from the pipe */
    if (ReadFile(readHandle, charBuffer, BUFFER_SIZE, &read, NULL))
        printf("Read from parent:  %s", charBuffer);
    else
        fprintf(stderr, "Error reading from pipe");

    return 0;
}