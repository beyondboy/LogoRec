#ifndef	LOG_H
#define	LOG_H	
namespace LogRec
{

	/***
	  *
	  *
	  *单例模式的两种复用方式
	  *
	  */

	//模板方式
	template<typename T>
	class Singleton 
	{
	public:
		static T* getInstance()
		{
			if (m_instance == NULL)
			{
				m_instance = new T;
			}
			return m_instance;
		}

		static void ungetInstance()
		{
			if (m_instance != NULL)
			{
				delete m_instance;
				m_instance = NULL;
			}
		}

	protected:
		Singleton(){}
		virtual ~Singleton(){}
	private:
		static T* m_instance;
	};


	template <typename T> 
	T* Singleton<T>::m_instance = NULL;

	//宏定义方式
	#define	DECLARE_SINGLETON(Singleton)	public:\
		static Singleton* getInstance(){ \
			if (m_instance == NULL){\
				m_instance = new Singleton;\
			}\
			return m_instance;\
		}\
		static void unInstance(){\
			if (m_instance != NULL){\
				delete m_instance;\
				m_instance = NULL;\
			}\
		}\
	private:\
		Singleton(){}\
		virtual ~Singleton(){\
			if (m_instance != NULL){\
				delete m_instance;\
				m_instance = NULL;\
			}\
		}\
	private:\
		static Singleton* m_instance;\

	#define IMPLEMENT_SINGLETON(Singleton)	Singleton* Singleton::m_instance = NULL;


	/**
	  *
	  *
	  *简单日志类的基本实现
	  *
	  */

	//define the log level
	enum eLogLevel
	{
		LEVEL_ALL	=	0,
		LEVEL_DEBUG,
		LEVEL_INFO,
		LEVEL_WARN,
		LEVEL_ERROR,
		LEVEL_FATAL,
		LEVEL_OFF
	};

	enum eLogOption
	{
		LOG_OPTION_ALL	=	0,
		LOG_OPTION_FILE,
		LOG_OPTION_TIME,
		LOG_OPTION_INFO
	};

	class LogBase
	{
	public:
		void setLogInfo(eLogLevel LOG_LEVEL, char* File, int line);
		void setMinLogLevel(eLogLevel MIN_LOG_LEVEL);
		eLogLevel getMinLogLevel() const;
		virtual void writeLog(const char* fmt, ...);
		
		void setLogOption(eLogOption OPT);
		eLogOption getLogOption() const;
		
		void Loginfo()
		{
			std::cout << "hello world!" << std::endl;
		}

	private:
		eLogLevel m_minLogLevel;

	protected:	
		char* m_File;
		int m_line;
		eLogLevel m_logLevel;
		eLogOption m_logOption;
	};

	class Logger : public LogBase
	{
		DECLARE_SINGLETON(Logger);
	public:
		eLogLevel getLogLevel() const;
		char* getFileName() const;
		int getLine() const;
	};


	#define LOG_SET_MIN_LEVEL(minLevel)	Logger::getInstance()->setMinLogLevel(minLevel);
	#define LOG_SET_OPTION(option)		Logger::getInstance()->setLogOption(option);

	#define LOG_LEVEL_ALL		Logger::getInstance()->setLogInfo(LEVEL_ALL,__FILE__,__LINE__);
	#define LOG_LEVEL_INFO		Logger::getInstance()->setLogInfo(LEVEL_INFO,__FILE__,__LINE__);
	#define LOG_LEVEL_WARN		Logger::getInstance()->setLogInfo(LEVEL_WARN,__FILE__,__LINE__);
	#define LOG_LEVEL_ERROR		Logger::getInstance()->setLogInfo(LEVEL_ERROR,__FILE__,__LINE__);
	#define LOG_LEVEL_FATAL		Logger::getInstance()->setLogInfo(LEVEL_FATAL,__FILE__,__LINE__);
	#define LOG_LEVEL_OFF		Logger::getInstance()->setLogInfo(LEVEL_OFF,__FILE__,__LINE__);
	//该句相当于定义和实例化一个静态变量，不能放在头文件中，否则一旦多个文件包含该头文件，就会报该静态变量已经在多个目标文件中定义的错误
	//IMPLEMENT_SINGLETON(Logger);

	void PRINT_LOG_MESSAGE(const char* fmt, ...);
}

#endif	LOG_H