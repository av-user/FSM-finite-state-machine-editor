#ifndef LOGGER_H
#define LOGGER_H

#include "loglib_global.h"
#include <map>
#include <vector>
#include <sys/types.h>
#include <cstdarg>
#include <QMutex>
#include <QSystemSemaphore>

namespace Log {

class LOGLIBSHARED_EXPORT Logger {

		public:

			static void initialize (const char *pCfgfilename, const char *pProcessLogName);

			static void debugV(const char *pComponent, const char *pMessage, ...);
			static void errorV(const char *pComponent, const char *pMessage, ...);
			static void fatalV(const char *pComponent, const char *pMessage, ...);
			static void infoV(const char *pComponent, const char *pMessage, ...);
			static void traceV(const char *pComponent, const char *pMessage, ...);
			static void warnV(const char *pComponent, const char *pMessage, ...);

			static void debug(const char *pComponent, const char *pMessage);
			static void error(const char *pComponent, const char *pMessage);
			static void fatal(const char *pComponent, const char *pMessage);
			static void info(const char *pComponent, const char *pMessage);
			static void trace(const char *pComponent, const char *pMessage);
			static void warn(const char *pComponent, const char *pMessage);

			static void debugX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV);
			static void errorX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV);
			static void fatalX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV);
			static void infoX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV);
			static void traceX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV);
			static void warnX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV);

			static void debugXML(const char *pComponent, const char *pText, const char *pXmlStr);
			static void errorXML(const char *pComponent, const char *pText, const char *pXmlStr);
			static void fatalXML(const char *pComponent, const char *pText, const char *pXmlStr);
			static void infoXML(const char *pComponent, const char *pText, const char *pXmlStr);
			static void traceXML(const char *pComponent, const char *pText, const char *pXmlStr);
			static void warnXML(const char *pComponent, const char *pText, const char *pXmlStr);

			static void v2hexstr (std::string *pStr, const std::vector<unsigned char> *pV);
			static void editXML (std::string *pStr);
			const static char *pEOL;
		private:
			enum Loglevel{
				ALL, 	//The ALL has the lowest possible rank and is intended to turn on all logging.
				DEBUG, 	//The DEBUG Level designates fine-grained informational events that are most useful to debug an application.
				ERR, 	//The ERROR level designates error events that might still allow the application to continue running.
				FATAL, 	//The FATAL level designates very severe error events that will presumably lead the application to abort.
				INFO,	//The INFO level designates informational messages that highlight the progress of the application at coarse-grained level.
				OFF, 	//The OFF has the highest possible rank and is intended to turn off logging.
				TRACE, 	//The TRACE Level designates finer-grained informational events than the DEBUG
				WARN,
			};
			static QMutex mutex;
			static Loglevel loglevel;
			static std::map<std::string, Log::Logger::Loglevel> loglevels;
			static std::string logfilename;
			static long max_filebytes;
			static unsigned int backup_count;
			static char buffer[256];
			static QString timestr;
			static unsigned int COMPONENT_NAME_LENGTH;
			static QSystemSemaphore semaphore;//("market", 5, QSystemSemaphore::Create);
			static const unsigned int KEYLEN = 8;
			static unsigned char key8[KEYLEN];
			static const unsigned char key8develop[KEYLEN];
			static const char *pVersion;

			static void write(Loglevel level, const char *pComponent, const char *pMessage);
			static void writeV(Loglevel level, const char *pComponent, const char *pMessage, const va_list *pArglist);
			static void writeX(Loglevel level, const char *pComponent, const char *pText, const std::vector<unsigned char> *pV);
			static void writeXML(Loglevel level, const char *pComponent, const char *pText, const char *pXmlStr);

			static bool configure (const char *pCfgfilename);
			static void check();
			static void rotate();
			static void pick_time();
			static void write(const char *pText);
			static void write(const char *pLevel, const char *pComponent, const char *pMessage);
	};
}

#endif // LOGGER_H
