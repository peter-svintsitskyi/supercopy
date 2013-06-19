#include "StdAfx.h"
#include "Job.h"

#include <tchar.h>
#include <Strsafe.h>

using namespace std;

Job::Job(int nId, LPCTSTR lpSourcePath, LPCTSTR lpTargetPath)
{
	m_nId = nId;
	m_strSourcePath = tstring(lpSourcePath);
	m_strTargetPath = tstring(lpTargetPath);

	//f.open("D:\\Centos\\www\\log.txt", ios::out);

	m_nAllFilesSize = 0;

	m_dPercentDone = 0.0;
	m_nTotalBytesTransferred = 0;
	m_nLastBytesTransferred = 0;

	m_nLastSpeedCheckTimeMilliseconds = 0;
	m_nTotalBytesTransferredOnLastSpeedCheck = 0;
	m_nTransferRateBytesPerSecond = 0;
	m_nTransferRateLimitBytesPerSecond = 500 * 1024;
}


Job::~Job(void)
{
	//f.close();
}

void Job::execute()
{
	setStatus(jobStatusEstimating);
	estimate();

	setStatus(jobStatusPaused);
	copy();

	cleanup();
}

void Job::cleanup()
{
	if(m_eStatus == jobStatusAborted || m_eStatus == jobStatusFailed) {
		// TODO: do the cleanup
	}

	m_bIsClean = true;
}

void Job::estimate()
{
	m_nAllFilesSize = 0;
	DWORD dwSourcePathAttributes = GetFileAttributes(m_strSourcePath.c_str());

	if(dwSourcePathAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		calculateDirectorySize(m_strSourcePath);
	} else {
		calculateFileSize(m_strSourcePath);
	}
}

void Job::copy()
{
	m_dPercentDone = 0.0;
	m_nTotalBytesTransferred = 0;
	m_nLastBytesTransferred = 0;

	m_nLastSpeedCheckTimeMilliseconds = 0;
	m_nTotalBytesTransferredOnLastSpeedCheck = 0;
	m_nTransferRateBytesPerSecond = 0;

	DWORD dwSourcePathAttributes = GetFileAttributes(m_strSourcePath.c_str());

	tstring strSourceFileName = m_strSourcePath;
	size_t sep = strSourceFileName.find_last_of(tstring(_T("\\/")));
		
	if (sep != std::string::npos)
		strSourceFileName = strSourceFileName.substr(sep + 1, strSourceFileName.size() - sep - 1);

	if(dwSourcePathAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		tstring strTargetDir = m_strTargetPath + _T("\\") + strSourceFileName;
		CreateDirectory(strTargetDir.c_str(), 0);
		copyDirectory(m_strSourcePath, strTargetDir);
	} else {
		copyFile(m_strSourcePath, m_strTargetPath + _T("\\") + strSourceFileName);
	}
}

void Job::calculateFileSize(const tstring &strPath)
{
	WIN32_FIND_DATA sFindData;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &sFindData);
	if(INVALID_HANDLE_VALUE == hFind) {
		// TODO add error reporting. Modify job status to failed.

		// f << "Calculate File Size: FindFirstFile failed, condition: " << strPath << endl;
		FindClose(hFind);
		return;
	}

	ULARGE_INTEGER sFileSize;
	sFileSize.LowPart = sFindData.nFileSizeLow;
	sFileSize.HighPart = sFindData.nFileSizeHigh;
	m_nAllFilesSize += sFileSize.QuadPart;

	FindClose(hFind);
}

void Job::calculateDirectorySize(const tstring &strPath) 
{
	tstring currentDir(strPath);
	currentDir.erase(currentDir.find_last_not_of(_T("\\"))+1);
	currentDir += _T("\\*");

	WIN32_FIND_DATA sFindData;
	HANDLE hFind = FindFirstFile(currentDir.c_str(), &sFindData);
	if(INVALID_HANDLE_VALUE == hFind) {
		// TODO add error reporting. Modify job status to failed.

		// f << "Calculate Directory Size: FindFirstFile failed, condition: " << currentDir << endl;
		FindClose(hFind);
		return;
	}

	do {
		 if(sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			 if(_tcscmp(sFindData.cFileName, _T(".")) == 0 || _tcscmp(sFindData.cFileName, _T("..")) == 0) {
				 continue;
			 }

			 tstring subDir = strPath + _T("\\") + sFindData.cFileName + _T("\\");
			 calculateDirectorySize(subDir);
		 } else {
			 m_nAllFilesSize += (sFindData.nFileSizeHigh * (MAXDWORD+1)) + sFindData.nFileSizeLow;
		 }
	} while(FindNextFile(hFind, &sFindData) != 0);

	FindClose(hFind);
}


void Job::copyDirectory(const tstring &strSourcePath, const tstring &strTargetPath)
{
	// Job is aborted, cancel copying
	if(m_eStatus == jobStatusAborted) {
		return;
	}

	// job is paused, waiting for resume
	while(m_eStatus == jobStatusPaused) {
		Sleep(1000);
	}

	tstring source = strSourcePath + _T("\\*");

	WIN32_FIND_DATA sFindData;
	HANDLE hFind = FindFirstFile(source.c_str(), &sFindData);
	if(INVALID_HANDLE_VALUE == hFind) {
		// TODO add error reporting. Modify job status to failed.

		//f << "CopyDirectory: FindFirstFile failed, condition: " << source << endl;
		FindClose(hFind);
		return;
	}

	do {
		 if(sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			 if(_tcscmp(sFindData.cFileName, _T(".")) == 0 || _tcscmp(sFindData.cFileName, _T("..")) == 0) {
				 continue;
			 }

			 tstring strSourceSubDirectory = strSourcePath + _T("\\") + sFindData.cFileName + _T("\\");
			 tstring strTargetSubDirectory = strTargetPath + _T("\\") + sFindData.cFileName + _T("\\");
			
			 // create directory in target
			 //f << "Creating Directory " << strSourceSubDirectory << endl;
			 CreateDirectory(strTargetSubDirectory.c_str(), 0);
			 //f << "Copy Directory from " << strSourceSubDirectory << " to " << strTargetSubDirectory << endl;
			 copyDirectory(strSourceSubDirectory, strTargetSubDirectory);
		 } else {

			 //tstring strSourceFilename = strSourcePath + _T("\\") + sFindData.cFileName;

			 //f << "Copy File from " << strSourcePath + _T("\\") + sFindData.cFileName << " to " << strTargetPath + _T("\\") + sFindData.cFileName << endl;
			 copyFile(strSourcePath + _T("\\") + sFindData.cFileName, strTargetPath + _T("\\") + sFindData.cFileName);
		 }
	} while(FindNextFile(hFind, &sFindData) != 0);

	FindClose(hFind);
}

void Job::copyFile(const tstring &strSourcePath, const tstring &strTargetPath)
{
	// Job is aborted, cancel copying
	if(m_eStatus == jobStatusAborted) {
		return;
	}

	m_nLastBytesTransferred = 0;
	CopyFileEx(strSourcePath.c_str(), strTargetPath.c_str(), &Job::s_copyProgressRoutine, this, NULL, 0);
}

DWORD CALLBACK Job::s_copyProgressRoutine(
	_In_      LARGE_INTEGER TotalFileSize,
	_In_      LARGE_INTEGER TotalBytesTransferred,
	_In_      LARGE_INTEGER StreamSize,
	_In_      LARGE_INTEGER StreamBytesTransferred,
	_In_      DWORD dwStreamNumber,
	_In_      DWORD dwCallbackReason,
	_In_      HANDLE hSourceFile,
	_In_      HANDLE hDestinationFile,
	_In_opt_  LPVOID lpData
) 
{
	Job* job = static_cast<Job*>(lpData);

	// Job is aborted, cancel copying
	if(job->getStatus() == jobStatusAborted) {
		return PROGRESS_CANCEL;
	}


	// Job is paused, waiting for resume
	while(job->getStatus() == jobStatusPaused) {
		Sleep(1000);
	}

	// calculating progress
	__int64 copiedSinceLastTime = 0;
	if(job->m_nLastBytesTransferred == 0) {
		copiedSinceLastTime = TotalBytesTransferred.QuadPart;
	} else {
		copiedSinceLastTime = TotalBytesTransferred.QuadPart - job->m_nLastBytesTransferred;
	}
	job->m_nTotalBytesTransferred += copiedSinceLastTime;
	job->m_nLastBytesTransferred = TotalBytesTransferred.QuadPart;

	if(job->m_nAllFilesSize > 0) {
		job->m_dPercentDone = (long double)job->m_nTotalBytesTransferred / (long double)job->m_nAllFilesSize * (long double)100.0;
	}

	if(job->m_nTotalBytesTransferred  == job->m_nAllFilesSize) {
		job->m_dPercentDone = 100.0;
	}

	if(copiedSinceLastTime == 0) {
		return PROGRESS_CONTINUE;
	}

	// calculating speed
	FILETIME sCurrentTimeFileTime;
	ULARGE_INTEGER sCurrentTime;
	__int64 nCurrentTimeMilliseconds;
	__int64 nTimeDiffMilliseconds;
	long double dTimeDiffSeconds;

	GetSystemTimeAsFileTime(&sCurrentTimeFileTime);
	sCurrentTime.LowPart = sCurrentTimeFileTime.dwLowDateTime;
	sCurrentTime.HighPart = sCurrentTimeFileTime.dwHighDateTime;
	nCurrentTimeMilliseconds = sCurrentTime.QuadPart / 10000; 
	nTimeDiffMilliseconds = nCurrentTimeMilliseconds - job->m_nLastSpeedCheckTimeMilliseconds;
	dTimeDiffSeconds = nTimeDiffMilliseconds / 1000.0;

	int nCheckIntervalMilliseconds = 5;
	if(nTimeDiffMilliseconds < nCheckIntervalMilliseconds) {
		return PROGRESS_CONTINUE;
	}

	__int64 nBytesTransferredSinceLastSpeedCheck = job->m_nTotalBytesTransferred - job->m_nTotalBytesTransferredOnLastSpeedCheck;

	// real speed
	long double dTransferRateBytesPerSecond = nBytesTransferredSinceLastSpeedCheck / dTimeDiffSeconds;

	if(dTransferRateBytesPerSecond > job->m_nTransferRateLimitBytesPerSecond) { // sleep to gain ethalon speed limit
		long double dSleepSeconds = (long double)nBytesTransferredSinceLastSpeedCheck / (long double)job->m_nTransferRateLimitBytesPerSecond - dTimeDiffSeconds;
		int nSleepMilliseconds = dSleepSeconds * 1000;  

		if(nSleepMilliseconds > 0 ) {
			// adjasted speed with sleep time included
			job->m_nTransferRateBytesPerSecond = nBytesTransferredSinceLastSpeedCheck / (dTimeDiffSeconds + dSleepSeconds);
			Sleep(nSleepMilliseconds);
		}
	}

	GetSystemTimeAsFileTime(&sCurrentTimeFileTime);
	sCurrentTime.LowPart = sCurrentTimeFileTime.dwLowDateTime;
	sCurrentTime.HighPart = sCurrentTimeFileTime.dwHighDateTime;
	nCurrentTimeMilliseconds = sCurrentTime.QuadPart / 10000; 
	nTimeDiffMilliseconds = nCurrentTimeMilliseconds - job->m_nLastSpeedCheckTimeMilliseconds;
	dTimeDiffSeconds = nTimeDiffMilliseconds / 1000.0;

	// adjasted speed with sleep time included
	job->m_nTransferRateBytesPerSecond = (job->m_nTransferRateBytesPerSecond + nBytesTransferredSinceLastSpeedCheck / dTimeDiffSeconds) / 2;
	
	job->m_nTotalBytesTransferredOnLastSpeedCheck = job->m_nTotalBytesTransferred;
	job->m_nLastSpeedCheckTimeMilliseconds = nCurrentTimeMilliseconds;

	return PROGRESS_CONTINUE;
}

