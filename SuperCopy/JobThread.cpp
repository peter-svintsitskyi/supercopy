#include "StdAfx.h"
#include "JobThread.h"

JobThread::JobThread(Job *job)
{
	this->job = job;
}

void JobThread::operator() ()
{
	job->execute();
}

