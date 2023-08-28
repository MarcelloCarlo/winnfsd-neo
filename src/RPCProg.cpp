#include "RPCProg.h"
#include <stdarg.h>
#include <stdio.h>

#include <direct.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>

#define GetCurrentDir _getcwd


CRPCProg::CRPCProg()
{
    m_bLogOn = true;
}

CRPCProg::~CRPCProg()
{
}

void CRPCProg::SetLogOn(bool bLogOn)
{
    m_bLogOn = bLogOn;
}

int CRPCProg::PrintLog(const char *format, ...)
{
    va_list vargs;
    int nResult;

    nResult = 0;

    if (m_bLogOn) {
        va_start(vargs, format);
        nResult = vprintf(format, vargs);

		// Get the current working directory
		char cCurrentPath[FILENAME_MAX];
		char logFolder[7] = "\\logs";
		time_t curr_time;
		struct tm curr_tm;
		char logFileName[100] = "NFSLogs";
		char currentDate[50];
		FILE* logFile;
		errno_t err;

		GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));

		// Create the folder logs within the directory
		strcat_s(cCurrentPath, logFolder);

		CreateDirectory(cCurrentPath, NULL);

		// Check if logfile exists, if yes then open and add, if no then create new file

		time(&curr_time);
		localtime_s(&curr_tm, &curr_time);

		strftime(currentDate, 50, "_%m%d%Y.txt", &curr_tm);
		strcat_s(logFileName, currentDate);
		strcat_s(cCurrentPath, "\\");
		strcat_s(cCurrentPath, logFileName);

		// Log the time and the text from the logText
		fopen_s(&logFile, cCurrentPath, "a+");
		vfprintf(logFile, format, vargs);

		va_end(vargs);

		fclose(logFile);

		// Close the process (memory safety)
		_fcloseall();

    }

    return nResult;
}
