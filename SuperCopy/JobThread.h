#pragma once

#include "Job.h"

using namespace System;
using namespace System::Threading;

// Job wrapper. Used as a thread callback in .net managed code.
ref class JobThread
{
	Job *job;

public:
	JobThread(Job *job);

	// performs job execution
	void operator() ();
};

