#pragma once

#ifndef tstring
typedef std::basic_string<TCHAR> tstring;
#endif

// Enumeration that represents possible for jobs
enum JobStatus {
	jobStatusEstimating,
	jobStatusPaused,
	jobStatusRunning,
	jobStatusFailed,
	jobStatusAborted,
	jobStatusCleanup,
};

// Executes recursive copy operation for files and directories. Sample usage:
//    Job *j = new Job(1, L"D:\\source-path", L"F:\\target-path);
//	  j->execute();
//
// You can keep track on job execution using different public getters:
//    double percentsDone = j->getProgress();
class Job
{
	//std::wfstream f;

protected:
	// Job identifier
	int m_nId;

	// Source path. Directory or file that should be copied to destination directory.
	tstring m_strSourcePath;

	// Target path. The destination of source files and directories.
	tstring m_strTargetPath;
	
	// Total size of files in source path in bytes. Computed recursively.
	__int64 m_nAllFilesSize;

	// Total bytes copied from source to target during job execution.
	__int64 m_nTotalBytesTransferred;

	// Number of bytes copied since last call of s_copyProgressRoutine callback
	__int64 m_nLastBytesTransferred;

	// Percents completed, total bytes transferred divided by all files size
	double m_dPercentDone;

	// Number of bytes copied since last speed check
	__int64 m_nTotalBytesTransferredOnLastSpeedCheck;

	// Last speed check time in milliseconds since 
	__int64 m_nLastSpeedCheckTimeMilliseconds;

	// Actual copying speed bytes / second
	__int64 m_nTransferRateBytesPerSecond;

	// Required copying speed limit bytes / second
	__int64 m_nTransferRateLimitBytesPerSecond;

	// Flag to show whether job requires cleanup when error occured etc.
	bool m_bIsClean;

	// Current status of job execution
	JobStatus m_eStatus;

	// Callback routine passed to CopyFileEx function
	// Calculates job progress and current speed.
	// Limits transfer speed when it exeeds the m_nTransferRateLimitBytesPerSecond.
	static DWORD CALLBACK s_copyProgressRoutine(
		_In_      LARGE_INTEGER TotalFileSize,
		_In_      LARGE_INTEGER TotalBytesTransferred,
		_In_      LARGE_INTEGER StreamSize,
		_In_      LARGE_INTEGER StreamBytesTransferred,
		_In_      DWORD dwStreamNumber,
		_In_      DWORD dwCallbackReason,
		_In_      HANDLE hSourceFile,
		_In_      HANDLE hDestinationFile,
		_In_opt_  LPVOID lpData
	); 

	// Performs recursive copy of strSourcePath directory to strTargetPath directory
	void copyDirectory(const tstring &strSourcePath, const tstring &strTargetPath);

	// Performs file copy from strSourcePath to strTargetPath
	void copyFile(const tstring &strSourcePath, const tstring &strTargetPath);

	// Calculates m_nAllFilesSize - total size of data that need to be copied.
	void estimate();

	// Performs cleanup when error occures or job is aborted.
	void cleanup();

	// Recursively calculates size of all files in strPath directory. Increments m_nAllFilesSize.
	void calculateDirectorySize(const tstring &strPath);

	// Calculates strPath file size. Increments m_nAllFilesSize.
	void calculateFileSize(const tstring &strPath);
	
	// Performs recursive copy operation.
	void copy();

public:
	// Job constructor. 
	//    nId - job identifier
	//    lpSourcePath - source directory or file
	//	  lpTargetPath - destination directory or file
	Job(int nId, LPCTSTR lpSourcePath, LPCTSTR lpTargetPath);
	~Job(void);

	// Estimates job size and executes copy routine.
	void execute();
	
	// returns job identifier
	inline int getId() { return m_nId; }

	// returns source path
	inline LPCTSTR getSourcePath() { return m_strSourcePath.c_str(); }

	// returns destination path
	inline LPCTSTR getTargetPath() { return m_strTargetPath.c_str(); }

	// returns job size
	inline __int64 getJobSizeBytes() { return m_nAllFilesSize; }

	// retuns percentage of job completion
	inline double getProgress() { return m_dPercentDone; }

	// returns actual copy speed
	inline __int64 getTransferRateBytesPerSecond() { return m_nTransferRateBytesPerSecond; }

	// Allows to update job status. Can be used to pause/resume job execution.
	inline void setStatus(JobStatus status) { m_eStatus = status; }

	// returns current status of a job
	inline JobStatus getStatus() { return m_eStatus; }

	// sets required copy speed limit
	inline void setTransferRateLimitBytesPerSecond(__int64 nLimit) { m_nTransferRateLimitBytesPerSecond = nLimit; }

	// returns required copy speed limit
	inline __int64 getTransferRateLimitBytesPerSecond() { return m_nTransferRateLimitBytesPerSecond; }

	// returns true if job is finished without errors.
	inline bool isClean() { return m_bIsClean; }
};

