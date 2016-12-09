#pragma once

// easylogging includes
#ifndef _DEBUG
	#define _DEBUG
	#define ELPP_THREAD_SAFE
	#define ELPP_QT_LOGGING
	#define ELPP_NO_DEFAULT_LOG_FILE
	#include <easylogging++.h>
	#undef _DEBUG
#else
	#define ELPP_THREAD_SAFE
	#define ELPP_QT_LOGGING
	#define ELPP_NO_DEFAULT_LOG_FILE
	#include <easylogging++.h>
#endif
