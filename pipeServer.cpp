/**
 * @file pipeServer.cpp
 * @author Ben Price
 * @brief Program (server) to send anonymous pipe handle to client
 * @version 1.0
 * @date 2024-03-08
 * 
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define BUFFER_SIZE 25

int main(VOID) {
    /** 
     * Declaration of needed parameters*/
    HANDLE readHandle, writeHandle;
    STARTUPINFO strtInfo;
    PROCESS_INFORMATION procInfo;
    char message[BUFFER_SIZE] = "Sup?";
    DWORD written;
    LPCSTR executablePath = "pipeClient.exe";

    /**
     *  Set up security attributes so child can inherit */
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};

    /** 
     * Memory allocation */
    ZeroMemory(&procInfo, sizeof(procInfo));

    /** 
     * Create the pipe. Notifies of errors */
    if (!CreatePipe(&readHandle, &writeHandle, &sa, 0)) {
        fprintf(stderr, "Create Pipe Failure");
        return 1;
    }

    /** 
     * Set up START_INFO structure for child */
    GetStartupInfo(&strtInfo);
    strtInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    /** 
     * Change client's standard input to pipe read end */
    strtInfo.hStdInput = readHandle;
    strtInfo.dwFlags = STARTF_USESTDHANDLES;

    /** 
     * Prevent child from inheriting write end of pipe */
    SetHandleInformation(writeHandle, HANDLE_FLAG_INHERIT, 0);

    /** 
     * Create the child process,
     * allowing inheritance of handles */
    CreateProcess(executablePath, NULL, NULL, NULL,
        TRUE, 0, NULL, NULL, &strtInfo, &procInfo);

    /** 
     * Close the parent's unused read end of the pipe */
    CloseHandle(readHandle);

    /**
     * Write the message to the pipe,
     * handling errors */
    if (!WriteFile(writeHandle, message, BUFFER_SIZE, &written, NULL))
        fprintf(stderr, "Error writing to pipe.");

    /** 
     * Close the parent's write end of the pipe */
    CloseHandle(writeHandle);

    /** 
     * Await termination of child process */
    WaitForSingleObject(procInfo.hProcess, INFINITE);
    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);

    return 0;
}



