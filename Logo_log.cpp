#include "StdAfx.h"

#include "Logo_log.h"

using namespace std;

namespace LogRec
{
	IMPLEMENT_SINGLETON(Logger);
	const char* strLogLevel[] = 
	{
		"All",
		"Debug",
		"Info",
		"Warn",
		"Error",
		"Fatal",
		"Off"
	};

	void LogBase::setLogOption(eLogOption OPT)
	{
		m_logOption = OPT;
	}

	eLogOption LogBase::getLogOption() const
	{
		return m_logOption;
	}

	void LogBase::setLogInfo(eLogLevel LOG_LEVEL, char* File, int line)
	{
		m_logLevel	 = LOG_LEVEL;
		m_File		 = File;
		m_line		 = line;
	}

	void LogBase::setMinLogLevel(eLogLevel MIN_LOG_LEVEL)
	{
		m_minLogLevel = MIN_LOG_LEVEL;
	}

	eLogLevel LogBase::getMinLogLevel() const
	{
		return m_minLogLevel;
	}

	eLogLevel Logger::getLogLevel() const
	{
		return m_logLevel;
	}

	char* Logger::getFileName() const
	{
		return m_File;
	}

	int Logger::getLine() const
	{
		return m_line;
	}

	void LogBase::writeLog(const char* fmt, ...)
	{
		//日志级别控制
		if (m_logLevel >= m_minLogLevel)
		{
			char str[30];
			fprintf(stderr,"\n======================Log Begin===================\n");
			time_t now = time(NULL);
			tm datetime = {0};
			localtime_s(&datetime,&now);
			//时间信息
			_snprintf_s(str,30,30,"%d-%02d-%02d %02d:%02d:%02d",datetime.tm_year+1900,
				datetime.tm_mon,datetime.tm_mday,datetime.tm_hour,datetime.tm_min,datetime.tm_sec);
			//位置信息
			fprintf(stderr,"%s %d %s [%s]\n",m_File,m_line,str,strLogLevel[m_logLevel]);
			//日志主要内容
			va_list argp;
			va_start(argp,fmt);
			vfprintf(stderr,fmt,argp);
			va_end(argp);
			fprintf(stderr,"\n======================Log End======================\n");
		}
	}
	void PRINT_LOG_MESSAGE(const char* fmt, ...)
	{
		//日志级别控制
		if (LogRec::Logger::getInstance()->getLogLevel() >= LogRec::Logger::getInstance()->getMinLogLevel())
		{
			char str[30];
			if (LogRec::Logger::getInstance()->getLogOption() != LOG_OPTION_INFO)
			{
				fprintf(stderr,"\n======================Log Begin===================\n");
			}
			time_t now = time(NULL);
			tm datetime = {0};
			localtime_s(&datetime,&now);

			switch( LogRec::Logger::getInstance()->getLogOption() )
			{
				case LOG_OPTION_ALL:
					//时间信息
					_snprintf_s(str,30,30,"%d-%02d-%02d %02d:%02d:%02d",datetime.tm_year+1900,
						datetime.tm_mon,datetime.tm_mday,datetime.tm_hour,datetime.tm_min,datetime.tm_sec);
					//位置信息
					fprintf(stderr,"%s %d %s [%s]\n",LogRec::Logger::getInstance()->getFileName(),
						LogRec::Logger::getInstance()->getLine()+1,str,strLogLevel[LogRec::Logger::getInstance()->getLogLevel()]);
					//日志主要内容
					break;
				case LOG_OPTION_FILE:
					//位置信息
					fprintf(stderr,"%s %d [%s]\n",LogRec::Logger::getInstance()->getFileName(),
						LogRec::Logger::getInstance()->getLine()+1,strLogLevel[LogRec::Logger::getInstance()->getLogLevel()]);
					break;

				case LOG_OPTION_TIME:
					//时间信息
					_snprintf_s(str,30,30,"%d-%02d-%02d %02d:%02d:%02d",datetime.tm_year+1900,
						datetime.tm_mon,datetime.tm_mday,datetime.tm_hour,datetime.tm_min,datetime.tm_sec);
	
					fprintf(stderr,"%s [%s]\n",str,strLogLevel[LogRec::Logger::getInstance()->getLogLevel()]);
					break;

				case LOG_OPTION_INFO:
					fprintf(stderr,"\n[%s] ",strLogLevel[LogRec::Logger::getInstance()->getLogLevel()]);
					break;

				default:
					break;
			}

			va_list argp;
			va_start(argp,fmt);
			vfprintf(stderr,fmt,argp);
			va_end(argp);
			if (LogRec::Logger::getInstance()->getLogOption() != LOG_OPTION_INFO)
			{
				fprintf(stderr,"\n======================Log End=====================\n");
			}
		}
	}
	
}