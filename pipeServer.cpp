#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 25
int main(VOID) {
    HANDLE readHandle, writeHandle;
    STARTUPINFO strtInfo;
    PROCESS_INFORMATION procInfo;
    char message[BUFFER_SIZE] = "Sup?";
    DWORD written;
    LPCSTR executablePath = "pipeClient.exe";

    /* set up security attributes allowing pipes to be inherited */
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};

    /* allocate memory */
    ZeroMemory(&procInfo, sizeof(procInfo));

    /* create the pipe */
    if (!CreatePipe(&readHandle, &writeHandle, &sa, 0)) {
        fprintf(stderr, "Create Pipe Failure");
        return 1;
    }

    /* establish the START INFO structure for the child process */
    GetStartupInfo(&strtInfo);
    strtInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    /* redirect standard input to the read end of the pipe */
    strtInfo.hStdInput = readHandle;
    strtInfo.dwFlags = STARTF_USESTDHANDLES;

    /* don’t allow the child to inherit the write end of pipe */
    SetHandleInformation(writeHandle, HANDLE_FLAG_INHERIT, 0);

    /* create the child process */
    CreateProcess(executablePath, NULL, NULL, NULL,
        TRUE, 
        /* inherit handles */
        0, NULL, NULL, &strtInfo, &procInfo);

    /* close the unused end of the pipe */
    CloseHandle(readHandle);

    /* the parent writes to the pipe */
    if (!WriteFile(writeHandle, message, BUFFER_SIZE, &written, NULL))
        fprintf(stderr, "Error writing to pipe.");

    /* close the write end of the pipe */
    CloseHandle(writeHandle);

    /* wait for the child to exit */
    WaitForSingleObject(procInfo.hProcess, INFINITE);
    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);

    return 0;
}



