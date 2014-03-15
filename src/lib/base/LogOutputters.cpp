/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2002 Chris Schoeneman, Nick Bolton, Sorin Sbarnea
 * 
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file COPYING that should have accompanied this file.
 * 
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LogOutputters.h"
#include "CArch.h"

#include <fstream>
#include <iostream>

//
// CStopLogOutputter
//

CStopLogOutputter::CStopLogOutputter()
{
	// do nothing
}

CStopLogOutputter::~CStopLogOutputter()
{
	// do nothing
}

void
CStopLogOutputter::open(const char*)
{
	// do nothing
}

void
CStopLogOutputter::close()
{
	// do nothing
}

void
CStopLogOutputter::show(bool)
{
	// do nothing
}

bool
CStopLogOutputter::write(ELevel, const char*)
{
	return false;
}

const char*
CStopLogOutputter::getNewline() const
{
	return "";
}


//
// CConsoleLogOutputter
//

CConsoleLogOutputter::CConsoleLogOutputter()
{
	// do nothing
}

CConsoleLogOutputter::~CConsoleLogOutputter()
{
	// do nothing
}

void
CConsoleLogOutputter::open(const char* title)
{
	ARCH->openConsole(title);
}

void
CConsoleLogOutputter::close()
{
	ARCH->closeConsole();
}

void
CConsoleLogOutputter::show(bool showIfEmpty)
{
	ARCH->showConsole(showIfEmpty);
}

bool
CConsoleLogOutputter::write(ELevel level, const char* msg)
{
	ARCH->writeConsole(level, msg);
	return true;
}

const char*
CConsoleLogOutputter::getNewline() const
{
	return ARCH->getNewlineForConsole();
}


//
// CSystemLogOutputter
//

CSystemLogOutputter::CSystemLogOutputter()
{
	// do nothing
}

CSystemLogOutputter::~CSystemLogOutputter()
{
	// do nothing
}

void
CSystemLogOutputter::open(const char* title)
{
	ARCH->openLog(title);
}

void
CSystemLogOutputter::close()
{
	ARCH->closeLog();
}

void
CSystemLogOutputter::show(bool showIfEmpty)
{
	ARCH->showLog(showIfEmpty);
}

bool
CSystemLogOutputter::write(ELevel level, const char* msg)
{
	ARCH->writeLog(level, msg);
	return true;
}

const char*
CSystemLogOutputter::getNewline() const
{
	return "";
}


//
// CSystemLogger
//

CSystemLogger::CSystemLogger(const char* title, bool blockConsole) :
	m_stop(NULL)
{
	// redirect log messages
	if (blockConsole) {
		m_stop = new CStopLogOutputter;
		CLOG->insert(m_stop);
	}
	m_syslog = new CSystemLogOutputter;
	m_syslog->open(title);
	CLOG->insert(m_syslog);
}

CSystemLogger::~CSystemLogger()
{
	CLOG->remove(m_syslog);
	delete m_syslog;
	if (m_stop != NULL) {
		CLOG->remove(m_stop);
		delete m_stop;
	}
}


//
// CBufferedLogOutputter
//

CBufferedLogOutputter::CBufferedLogOutputter(UInt32 maxBufferSize) :
	m_maxBufferSize(maxBufferSize)
{
	// do nothing
}

CBufferedLogOutputter::~CBufferedLogOutputter()
{
	// do nothing
}

CBufferedLogOutputter::const_iterator
CBufferedLogOutputter::begin() const
{
	return m_buffer.begin();
}

CBufferedLogOutputter::const_iterator
CBufferedLogOutputter::end() const
{
	return m_buffer.end();
}

void
CBufferedLogOutputter::open(const char*)
{
	// do nothing
}

void
CBufferedLogOutputter::close()
{
	// remove all elements from the buffer
	m_buffer.clear();
}

void
CBufferedLogOutputter::show(bool)
{
	// do nothing
}

bool
CBufferedLogOutputter::write(ELevel, const char* message)
{
	while (m_buffer.size() >= m_maxBufferSize) {
		m_buffer.pop_front();
	}
	m_buffer.push_back(CString(message));
	return true;
}

const char*
CBufferedLogOutputter::getNewline() const
{
	return "";
}


//
// CFileLogOutputter
//

CFileLogOutputter::CFileLogOutputter(const char * logFile)
{
	assert(logFile != NULL);

	m_handle.open(logFile, std::fstream::app);
	// open file handle
}

CFileLogOutputter::~CFileLogOutputter()
{
	// close file handle
	if (m_handle.is_open())
		m_handle.close();
}

const char*
CFileLogOutputter::getNewline() const
{
	return "\n";
}

bool
CFileLogOutputter::write(ELevel level, const char *message)
{
	if (m_handle.is_open() && m_handle.fail() != true) {
		m_handle << message;
		
		// write buffer to file
		m_handle.flush();
	}

	return true;
}

void
CFileLogOutputter::open(const char *title) {}

void
CFileLogOutputter::close() {}

void
CFileLogOutputter::show(bool showIfEmpty) {}

CStdLogOutputter::CStdLogOutputter()
{
}

CStdLogOutputter::~CStdLogOutputter()
{
}

bool
CStdLogOutputter::write(ELevel level, const char* message)
{
	if ((level >= kFATAL) && (level <= kWARNING))
		std::cerr << message << std::endl;
	else
		std::cout << message << std::endl;

	return true;
}

void
CStdLogOutputter::close()
{
}

void
CStdLogOutputter::open(const char* title)
{
}

void
CStdLogOutputter::show(bool showIfEmpty)
{
}

const char*
CStdLogOutputter::getNewline() const
{
	return "";
}
