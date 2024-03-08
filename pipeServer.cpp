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
#include <iostream>
#define BUFFER_SIZE 40
using namespace std;

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
     * User is prompted for message to send
    */
    cout << "Enter a word to send to child: ";
    cin >> message;

    /**
     * Set up security attributes for CreatePipe parameter
     */
    SECURITY_ATTRIBUTES sec_atts = {sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};

    /** 
     * Memory allocation */
    ZeroMemory(&procInfo, sizeof(procInfo));

    /** 
     * Create the pipe. Notifies of errors */
    if (!CreatePipe(&readHandle, &writeHandle, &sec_atts, 0)) {
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



