/******************************************************************************
 * scrun: Run a ShellCode
 ******************************************************************************
 * Version: 1.0
 ******************************************************************************
 *
 * Copyright (c) 2015 garigouster
 * Contact: garigouster dot git at google dot com
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************
 */

#if __WIN32__ || __WIN64__

#include <stdio.h>
#include <windows.h>

int main( int argc, char *argv[]) {
    LPCSTR fileName;
    HANDLE hFile;
    SIZE_T size;
    LPTSTR msg;
    void (*sc)(void);
    if (argc != 2 || !strlen(fileName = argv[1])) {
        fprintf(stderr,"Usage: %s shellcode\n",argv[0]);
    }
    else if ((hFile = CreateFile(fileName,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL)) == HFILE_ERROR
        || (size = GetFileSize(hFile,NULL)) == INVALID_FILE_SIZE
        || (sc = (void (*)(void)) VirtualAlloc(NULL,size,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE)) == NULL
        || !ReadFile(hFile,(LPVOID) sc,size,&size,NULL)
        || !CloseHandle(hFile)) {
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL,GetLastError(),MAKELANGID(LANG_USER_DEFAULT,SUBLANG_DEFAULT),(LPTSTR) &msg,0,NULL);
        fprintf(stderr,"%s: %s\n",fileName,msg);
    }
    else {
        sc();
        ExitProcess(ERROR_SUCCESS);
    }
    ExitProcess(-1);
}

#elif __UNIX__ || __APPLE__

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main( int argc, char *argv[]) {
    const char *fileName;
    int fdFile;
    struct stat stFile;
    void (*sc)(void);
    if (argc != 2 || !strlen(fileName = argv[1])) {
        fprintf(stderr,"Usage: %s shellcode\n",argv[0]);
    }
    else if ((fdFile = open(fileName,O_RDONLY)) == -1
        || stat(fileName,&stFile) == -1
        || (sc = (void (*)(void)) mmap(0,stFile.st_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE,fdFile,0)) == (void *) -1
        || close(fdFile) == -1) {
        fprintf(stderr,"%s: %s\n",fileName,strerror(errno));
    }
    else {
        sc();
        _exit(EXIT_SUCCESS);
    }
    exit(-1);
}

#endif
