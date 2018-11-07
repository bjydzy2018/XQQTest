/*
 * VASingleton.cpp
 *
 *  Created on: 2013-3-25
 *      Author: pffang
 */

#include "VASingleton.h"
namespace VA {
#if VA_GNUC && !VA_ANDROID
int CSingleton::lockfile(int fd) {
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}
int CSingleton::GetLock(const char* pidfilepath) {
    int fd;
    char buf[16];
    fd = open(pidfilepath, O_RDWR | O_CREAT,
              (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
    if (fd < 0) {
        //syslog(LOG_ERR, "can't open %s: %s", pidfilepath, strerror(errno));
        std::cout << "can't open " << pidfilepath << ": " << strerror(errno)
                        << std::endl;
        exit(EXIT_FAILURE);
    }
    if (lockfile(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return 1;
        }
        //syslog(LOG_ERR,"can't lock %s: %s", LOCKFILE, strerror(errno));
        std::cout << "can't lock " << pidfilepath << ": " << strerror(errno)
                        << std::endl;
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long) getpid());
    write(fd, buf, strlen(buf) + 1);
    return 0;
}
#elif VA_VC
	int CSingleton::GetLock(const wchar_t* MutexName) {
		hMutex = CreateMutexW(NULL, FALSE, MutexName);
		if (WaitForSingleObject(hMutex, 0) == WAIT_TIMEOUT)
			return 1;
		else
			return 0;
	}
	CSingleton::~CSingleton() {
		CloseHandle(hMutex);
	}
#endif
} /* namespace VA */
