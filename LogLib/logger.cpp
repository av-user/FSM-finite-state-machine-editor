#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>

#include "logger.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QThread>

const char *Log::Logger::pVersion = "V00.00";
Log::Logger::Loglevel Log::Logger::loglevel = Log::Logger::ERR;
std::map<std::string, Log::Logger::Loglevel> Log::Logger::loglevels;
std::string Log::Logger::logfilename = "default_log.txt";
long Log::Logger::max_filebytes = 10000;
unsigned int Log::Logger::backup_count = 5;
char Log::Logger::buffer[256];
QString Log::Logger::timestr;
unsigned char Log::Logger::key8[] = {31,32,53,61,30,20,33,65};
const unsigned char Log::Logger::key8develop[KEYLEN] = {'d','e','v','e','l','o','p','_'};
unsigned int Log::Logger::COMPONENT_NAME_LENGTH = 7;
const char *Log::Logger::pEOL = "\n";
QSystemSemaphore Log::Logger::semaphore ("logger", 1, QSystemSemaphore::Create);

using namespace std;

void Log::Logger::v2hexstr (std::string *pStr, const std::vector<unsigned char> *pV){
	size_t offset = 0;
	char buffer256[256];
	size_t size = pV->size();
	std::vector<unsigned char>::const_iterator cit;
	std::ostringstream os;
	std::string chars;
	pStr->clear();
	os << hex << std::setfill('0');
	for (cit = pV->begin(); cit != pV->end(); cit++){
		os << std::setw(2) << int(*cit) << ' ';
		chars += ::isprint(*cit) ? *cit : '.';
	}
	os << dec << std::setfill(' ');
	std::string str = os.str();
	while (offset < size){
		::sprintf (buffer256, "%04x:  %-24s %-24s  %-8s %-8s\n", offset
				, str.substr(offset*3, 8*3).c_str()
				, (offset+8) < size ? str.substr((offset+8)*3, 8*3).c_str() : ""
				, chars.substr(offset, 8).c_str()
				, (offset+8) < size ? chars.substr(offset+8, 8).c_str() : ""
				);
		*pStr += buffer256;
		offset += 16;
	}
}
void Log::Logger::initialize(const char *pCfgfilename, const char *pProcessLogName){
	char buffer [128];
	Log::Logger::configure (pCfgfilename);
	::sprintf (buffer, "initialize: %s\n", pProcessLogName);
	write (buffer);
}
bool Log::Logger::configure (const char *pCfgfilename){
	bool retval = false;
	std::string line;
	std::string str;
	std::string str2;
	try {
		std::ifstream infile( pCfgfilename ) ;
		loglevels.clear();
		if ( infile ) {
			if (getline( infile, line ) && line.compare(Log::Logger::pVersion) == 0){
				if (getline( infile, line ) && line.size() == KEYLEN){
					::memcpy((void*)key8, line.c_str(), KEYLEN);
					if (getline( infile, line )){
						if (getline( infile, line )){
							logfilename = line;
							if (getline( infile, line )){
								max_filebytes = ::atoi(line.c_str());
								if (getline( infile, line )){
									backup_count = ::atoi(line.c_str());
									Log::Logger::write("", "", "");
                                    ::sprintf(buffer, "%lld CONFIGURED", QCoreApplication::applicationPid());
									Log::Logger::write(buffer, "", "");
									::sprintf(buffer, "%ld, %u", max_filebytes, backup_count);
									Log::Logger::write("max_filebytes & backup_count: ", buffer, "");

									// component log levels:
									while ( getline( infile, line ) ) {
										// no comments allowed before!
										if (!line.empty() && line.at(0) == '#'){
											// bypass comment
											continue;
										}
										Log::Logger::write("CFG", "", line.c_str());
										if (line.size() > COMPONENT_NAME_LENGTH + 3){
											str = line.substr(0, COMPONENT_NAME_LENGTH);
											if (loglevels.find(str) == loglevels.end()){
												str2 = line.substr(COMPONENT_NAME_LENGTH+1);
												if (str2.compare("ALL") == 0){
													loglevels [str] = ALL;
												} else if (str2.compare("DEBUG") == 0){
													loglevels [str] = DEBUG;
												} else if (str2.compare("ERROR") == 0){
													loglevels [str] = ERR;
												} else if (str2.compare("FATAL") == 0){
													loglevels [str] = FATAL;
												} else if (str2.compare("INFO") == 0){
													loglevels [str] = INFO;
												} else if (str2.compare("OFF") == 0){
													loglevels [str] = OFF;
												} else if (str2.compare("TRACE") == 0){
													loglevels [str] = TRACE;
												} else if (str2.compare("WARN") == 0){
													loglevels [str] = WARN;
												}
											}
										}
									}
									Log::Logger::write("", "", "");
									retval = true;
								}
							}
						}
					}
				}
			} else {
				cout << "invalid cfg version" << endl;
			}
		} else {
			cout << "could not open config file" << endl;
		}
		infile.close( ) ;
	} catch (...){
		;
	}
	if (!retval){
		cout << "configure() failed" << endl;
	}
	return retval;
}
void Log::Logger::debug(const char *pComponent, const char *pMessage){
	Log::Logger::write (Log::Logger::DEBUG, pComponent, pMessage);
}
void Log::Logger::error(const char *pComponent, const char *pMessage){
	Log::Logger::write (Log::Logger::ERR, pComponent, pMessage);
}
void Log::Logger::fatal(const char *pComponent, const char *pMessage){
	Log::Logger::write (Log::Logger::FATAL, pComponent, pMessage);
}
void Log::Logger::info(const char *pComponent, const char *pMessage){
	Log::Logger::write (Log::Logger::INFO, pComponent, pMessage);
}
void Log::Logger::trace(const char *pComponent, const char *pMessage){
	Log::Logger::write (Log::Logger::TRACE, pComponent, pMessage);
}
void Log::Logger::warn(const char *pComponent, const char *pMessage){
	Log::Logger::write (Log::Logger::WARN, pComponent, pMessage);
}
void Log::Logger::debugX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV){
	Log::Logger::writeX (Log::Logger::DEBUG, pComponent, pText, pV);
}
void Log::Logger::errorX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV){
	Log::Logger::writeX (Log::Logger::ERR, pComponent, pText, pV);
}
void Log::Logger::fatalX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV){
	Log::Logger::writeX (Log::Logger::FATAL, pComponent, pText, pV);
}
void Log::Logger::infoX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV){
	Log::Logger::writeX (Log::Logger::INFO, pComponent, pText, pV);
}
void Log::Logger::traceX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV){
	Log::Logger::writeX (Log::Logger::TRACE, pComponent, pText, pV);
}
void Log::Logger::warnX(const char *pComponent, const char *pText, const std::vector<unsigned char> *pV){
	Log::Logger::writeX (Log::Logger::WARN, pComponent, pText, pV);
}

void Log::Logger::debugXML(const char *pComponent, const char *pText, const char *pXmlStr){
	Log::Logger::writeXML (Log::Logger::DEBUG, pComponent, pText, pXmlStr);
}
void Log::Logger::errorXML(const char *pComponent, const char *pText, const char *pXmlStr){
	Log::Logger::writeXML (Log::Logger::ERR, pComponent, pText, pXmlStr);
}
void Log::Logger::fatalXML(const char *pComponent, const char *pText, const char *pXmlStr){
	Log::Logger::writeXML (Log::Logger::FATAL, pComponent, pText, pXmlStr);
}
void Log::Logger::infoXML(const char *pComponent, const char *pText, const char *pXmlStr){
	Log::Logger::writeXML (Log::Logger::INFO, pComponent, pText, pXmlStr);
}
void Log::Logger::traceXML(const char *pComponent, const char *pText, const char *pXmlStr){
	Log::Logger::writeXML (Log::Logger::TRACE, pComponent, pText, pXmlStr);
}
void Log::Logger::warnXML(const char *pComponent, const char *pText, const char *pXmlStr){
	Log::Logger::writeXML (Log::Logger::WARN, pComponent, pText, pXmlStr);
}


void Log::Logger::debugV(const char *pComponent, const char *pMessage, ...){
	va_list arglist;
	va_start(arglist, pMessage);
	Log::Logger::writeV (Log::Logger::DEBUG, pComponent, pMessage, &arglist);
	va_end(arglist);
}
void Log::Logger::errorV(const char *pComponent, const char *pMessage, ...){
	va_list arglist;
	va_start(arglist, pMessage);
	Log::Logger::writeV (Log::Logger::ERR, pComponent, pMessage, &arglist);
	va_end(arglist);
}
void Log::Logger::fatalV(const char *pComponent, const char *pMessage, ...){
	va_list arglist;
	va_start(arglist, pMessage);
	Log::Logger::writeV (Log::Logger::FATAL, pComponent, pMessage, &arglist);
	va_end(arglist);
}
void Log::Logger::infoV(const char *pComponent, const char *pMessage, ...){
	va_list arglist;
	va_start(arglist, pMessage);
	Log::Logger::writeV (Log::Logger::INFO, pComponent, pMessage, &arglist);
	va_end(arglist);
}
void Log::Logger::traceV(const char *pComponent, const char *pMessage, ...){
	va_list arglist;
	va_start(arglist, pMessage);
	Log::Logger::writeV (Log::Logger::TRACE, pComponent, pMessage, &arglist);
	va_end(arglist);
}
void Log::Logger::warnV(const char *pComponent, const char *pMessage, ...){
	va_list arglist;
	va_start(arglist, pMessage);
	Log::Logger::writeV (Log::Logger::WARN, pComponent, pMessage, &arglist);
	va_end(arglist);
}

void Log::Logger::check(){
	struct stat filestatus;
	stat( logfilename.c_str(), &filestatus );
	if (filestatus.st_size > max_filebytes){
		Log::Logger::rotate();
	}
}
void Log::Logger::rotate(){
	char buffer2[256];
	char buffer3[256];
	::sprintf (buffer, "%s.%d", logfilename.c_str(), backup_count-1);
	std::ifstream f(buffer);
	if (f.is_open()){
		Log::Logger::write ("exists");
		Log::Logger::write ("removing file:");
		Log::Logger::write (Log::Logger::buffer);
		f.close();
		remove(buffer);
	} else {
		::sprintf (buffer, "%s does not exist", buffer);
		Log::Logger::write (buffer);
	}
	for (int i = backup_count - 2; i >= 0; i--){
		// old filename
		::sprintf (buffer, "%s.%d", logfilename.c_str(), i);
		// new filename
		::sprintf (buffer2, "%s.%d", logfilename.c_str(), i+1);
		::sprintf (buffer3, "renaming %s to %s", buffer, buffer2);
		Log::Logger::write (buffer3);
		rename(buffer, buffer2);
	}
	::sprintf (buffer3, "renaming %s to %s", logfilename.c_str(), buffer);
	Log::Logger::write (buffer3);
	rename(logfilename.c_str(), buffer);
}
void Log::Logger::pick_time(){
	timestr = QDateTime::currentDateTime().toString ("dd.MM.yy HH:mm:ss,zzz ");
}
void Log::Logger::write(const char *pText){
	std::ofstream ofs;
	bool encr = ::memcmp (Log::Logger::key8, Log::Logger::key8develop, KEYLEN) != 0;
	ios_base::openmode mode = std::ofstream::out | std::ofstream::app;
	if (encr){
		mode |= ios::binary;
	}
	ofs.open (logfilename.c_str(), mode);
	Log::Logger::pick_time();
	char buffer [16];
    sprintf(buffer, "0x%08x ", (unsigned int)QThread::currentThreadId());
	ofs << Log::Logger::timestr.toStdString().c_str() << buffer << pText << '\n';
	ofs.flush();
	ofs.close();
}
void Log::Logger::write(const char *pLevel, const char *pComponent, const char *pMessage){
	Log::Logger::check();
	std::ofstream ofs;
	bool encr = ::memcmp (Log::Logger::key8, Log::Logger::key8develop, KEYLEN) != 0;
	ios_base::openmode mode = std::ofstream::out | std::ofstream::app;
	if (encr){
		mode |= ios::binary;
	}
	ofs.open (logfilename.c_str(), mode);
	if (ofs.is_open()){
		Log::Logger::pick_time();
		char buffer [16];
        sprintf(buffer, "0x%08x ", (unsigned int)QThread::currentThreadId());
		ofs << timestr.toStdString().c_str() << buffer << pLevel << ' ' << pComponent << ' ' << pMessage << '\n';
		ofs.flush();
		ofs.close();
	}
}
void Log::Logger::writeX(Loglevel level, const char *pComponent, const char *pText, const std::vector<unsigned char> *pV){
	if (loglevels.find(pComponent) != loglevels.end()
			&& loglevels[pComponent] != Log::Logger::OFF
			&& loglevels[pComponent] <= level){

		std::string str;
		char buf[8];
		Logger::v2hexstr(&str, pV);
		switch (level){
		case Log::Logger::DEBUG:
			::strcpy (buf, "DBG");
			break;
		case Log::Logger::ERR:
			::strcpy (buf, "ERR");
			break;
		case Log::Logger::FATAL:
			::strcpy (buf, "FTL");
			break;
		case Log::Logger::INFO:
			::strcpy (buf, "INF");
			break;
		case Log::Logger::TRACE:
			::strcpy (buf, "TRC");
			break;
		case Log::Logger::WARN:
			::strcpy (buf, "WRN");
			break;
		default:
			::strcpy (buf, "UDF");
		}
		std::string str0 = pText;
		str0 += '\n';
		str0 += str;
		try {
			semaphore.acquire();
			Log::Logger::write(buf, pComponent, str0.c_str());
			semaphore.release();
		} catch (...){
			semaphore.release();
		}
	}
}
void Log::Logger::writeXML(Loglevel level, const char *pComponent, const char *pText, const char *pXmlStr){
	if (loglevels.find(pComponent) != loglevels.end()
			&& loglevels[pComponent] != Log::Logger::OFF
			&& loglevels[pComponent] <= level){

		std::string str = pXmlStr;
		char buf[8];
		Logger::editXML(&str);
		switch (level){
		case Log::Logger::DEBUG:
			::strcpy (buf, "DBG");
			break;
		case Log::Logger::ERR:
			::strcpy (buf, "ERR");
			break;
		case Log::Logger::FATAL:
			::strcpy (buf, "FTL");
			break;
		case Log::Logger::INFO:
			::strcpy (buf, "INF");
			break;
		case Log::Logger::TRACE:
			::strcpy (buf, "TRC");
			break;
		case Log::Logger::WARN:
			::strcpy (buf, "WRN");
			break;
		default:
			::strcpy (buf, "UDF");
		}
		std::string str0 = pText;
		str0 += str;
		str0 += '\n';
		try {
			semaphore.acquire();
			Log::Logger::write(buf, pComponent, str0.c_str());
			semaphore.release();
		} catch (...){
			semaphore.release();
		}
	}
}
void Log::Logger::writeV(Loglevel level, const char *pComponent, const char *pMessage, const va_list *pArglist){
	if (loglevels.find(pComponent) != loglevels.end()
			&& loglevels[pComponent] != Log::Logger::OFF
			&& loglevels[pComponent] <= level){

		std::string str;
		QString qstr = QString::vasprintf(pMessage, *pArglist);
		str = qstr.toStdString().c_str();
		char buf[8];
		switch (level){
		case Log::Logger::DEBUG:
			::strcpy (buf, "DBG");
			break;
		case Log::Logger::ERR:
			::strcpy (buf, "ERR");
			break;
		case Log::Logger::FATAL:
			::strcpy (buf, "FTL");
			break;
		case Log::Logger::INFO:
			::strcpy (buf, "INF");
			break;
		case Log::Logger::TRACE:
			::strcpy (buf, "TRC");
			break;
		case Log::Logger::WARN:
			::strcpy (buf, "WRN");
			break;
		default:
			::strcpy (buf, "UDF");
		}
		try {
			semaphore.acquire();
			Log::Logger::write(buf, pComponent, str.c_str());
			semaphore.release();
		} catch (...){
			semaphore.release();
		}
	}
}
void Log::Logger::write(Loglevel level, const char *pComponent, const char *pMessage){
	if (loglevels.find(pComponent) != loglevels.end()
			&& loglevels[pComponent] != Log::Logger::OFF
			&& loglevels[pComponent] <= level){
		char buf[8];
		switch (level){
		case Log::Logger::DEBUG:
			::strcpy (buf, "DBG");
			break;
		case Log::Logger::ERR:
			::strcpy (buf, "ERR");
			break;
		case Log::Logger::FATAL:
			::strcpy (buf, "FTL");
			break;
		case Log::Logger::INFO:
			::strcpy (buf, "INF");
			break;
		case Log::Logger::TRACE:
			::strcpy (buf, "TRC");
			break;
		case Log::Logger::WARN:
			::strcpy (buf, "WRN");
			break;
		default:
			::strcpy (buf, "UDF");
		}
		try {
			semaphore.acquire();
			Log::Logger::write(buf, pComponent, pMessage);
			semaphore.release();
		} catch (...){
			semaphore.release();
		}
	}
}
void Log::Logger::editXML (std::string *pStr){
	unsigned long	level = 0;
	unsigned long	index = std::string::npos;
	unsigned long	indexA = std::string::npos;
	unsigned long	indexB = std::string::npos;
	unsigned long 	sizeEOL = ::strlen (pEOL);
	unsigned long   size;

	enum ParserState {
		Start = 1,		// we are waiting thet the string beginns with <?xml
		XmlTag,			// index points on <?xml at the beginning of the string (== 0)
		Opening,		// index points on some <
		Inside,			// index points on the next byte after <tag attr="val">
		EmptyClosed,	// index points on the next byte after an empty element: <emptytag attr="val"/>
		NotEmptyClosed, // index points on the next byte after </tag>
		Comment,		// index points on <!--
		CDATA,			// index points on <![CDATA[
		End,			// string has been parsed
		Quit,			// parsing breaked
	};

	ParserState	state = Start;
	ParserState	savedState = Quit;
	std::string tstStr;
	index = pStr->find ('\r');
	while (index != std::string::npos){
		pStr->erase (index, 1);
		index = pStr->find ('\r');
	}
	index = pStr->find ('\n');
	while (index != std::string::npos){
		pStr->erase (index, 1);
		index = pStr->find ('\n');
	}
	while (state != Quit && state != End){
		switch (state){
		case Start:
			if (pStr->find ("<?xml") == 0){
				state = XmlTag;
			} else {
				state = Quit;
			}
			break;
		case XmlTag:
			pStr->insert (0, pEOL);
			indexA = pStr->find ("?>", 3);
			indexB = pStr->find ('<', 3);
			if (indexA != std::string::npos && indexB != std::string::npos && indexA < indexB){
				index = indexB;
				tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
				state = Opening;
			} else {
				state = Quit;
			}
			break;
		case Opening:
			// insert end of line and spaces
			pStr->insert (index, pEOL);
			pStr->insert (index+sizeEOL, level, ' ');
			index += level + sizeEOL;
			tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
			// check if comment
			if (pStr->find ("<!--", index) == index){
				savedState = state;
				state = Comment;
			} else if (pStr->find ("<![CDATA[", index) == index){
				savedState = state;
				state = CDATA;
			} else {
				// check for EmptyElemTag:
				tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
				indexA = pStr->find ('<', index+1);
				tstStr.assign (pStr->begin() + indexA, pStr->begin()+indexA+5);
				indexB = pStr->find ("/>", index+1);
				if (indexB != std::string::npos && indexB < indexA){
					tstStr.assign (pStr->begin() + indexB, pStr->begin()+indexB+5);
					// EmptyElemTag
					index = indexB+2;
					tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
					state = EmptyClosed;
				} else {
					indexB = pStr->find ('>', index+1);
					if (indexB != std::string::npos && indexB < indexA){
						tstStr.assign (pStr->begin() + indexB, pStr->begin()+indexB+5);
						index = indexB+1;
						tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
						state = Inside;
						level++;
					} else {
						state = Quit;
					}
				}
			}
			break;
		case Inside:
			size = pStr->size ();
			if (index < size){
				tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
				if (pStr->find ("<!--", index) == index){
					savedState = state;
					state = Comment;
				} else if (pStr->find ("<![CDATA[", index) == index){
					savedState = state;
					state = CDATA;
				} else {
					indexA = pStr->find ('<', index);
					if (indexA == index){
						if (pStr->find ("</", index) == index){
							indexA = pStr->find ('<', index+1);
							indexB = pStr->find ('>', index+1);
							if (indexB != std::string::npos && (indexB < indexA || indexA == std::string::npos)){
								pStr->insert (index, pEOL);
								pStr->insert (index+sizeEOL, --level, ' ');
								index += level + sizeEOL;
								tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
								index = indexB+1+level + sizeEOL;
								tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
								state = NotEmptyClosed;
							}
						} else {
							state = Opening;
						}
					} else {
						index++;
					}
				}
			} else {
				state = End;
			}
			break;
		case EmptyClosed:
			tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
			state = Inside;
			break;
		case NotEmptyClosed:
			size = pStr->size ();
			if (index < size){
				tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
				state = Inside;
			} else {
				state = End;
			}
			break;
		case Comment:
			tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
			// insert end of line and spaces
			pStr->insert (index, pEOL);
			pStr->insert (index+sizeEOL, level, ' ');
			index += level + sizeEOL;
			tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
			indexA = pStr->find ("-->", index);
			if (indexA != std::string::npos){
				index = indexA + 3;
				tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
				state = savedState;
				savedState = Quit;
			} else {
				state = Quit;
			}
			break;
		case CDATA:
			tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
			// insert end of line and spaces
			pStr->insert (index, pEOL);
			pStr->insert (index+sizeEOL, level, ' ');
			index += level + sizeEOL;
			tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
			indexA = pStr->find ("]]>", index);
			if (indexA != std::string::npos){
				index = indexA + 3;
				tstStr.assign (pStr->begin() + index, pStr->begin()+index+5);
				state = savedState;
				savedState = Quit;
			} else {
				state = Quit;
			}
			break;
		default:
			state = Quit;
		}
	}
	if (state == End && level != 0) {
		*pStr += " error: invalid nesting";
	}
}


