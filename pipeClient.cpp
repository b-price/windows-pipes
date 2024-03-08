/**
 * @file pipeClient.cpp
 * @author Ben Price
 * @brief A client program to receive a string message from a pipe
 * @version 1.0
 * @date 2024-03-08
 * 
 * 
 * 
 */

#include <stdio.h>
#include <windows.h>
#define BUFFER_SIZE 25

int main(VOID) {
    /** 
     * Declaration of needed parameters*/
    HANDLE readHandle;
    CHAR charBuffer[BUFFER_SIZE];
    DWORD read;

    /**
     * Get the read handle of the pipe */
    readHandle = GetStdHandle(STD_INPUT_HANDLE);
    
    /**
     * @brief Read contents of pipe from parent process
     * 
     */
    if (ReadFile(readHandle, charBuffer, BUFFER_SIZE, &read, NULL))
        printf("Read from parent:  %s", charBuffer);
    else
        fprintf(stderr, "Error reading from pipe");

    return 0;
}