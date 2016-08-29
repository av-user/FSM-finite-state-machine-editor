#include "reg.h"

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

//#include <sys/sem.h>
//#include <unistd.h>
#include <errno.h>

#define MAX_STRING_LENGTH 1024
#define MAX_SECTION_LENGTH 128

QSystemSemaphore UtilLib::Reg::semaphore ("regutil", 1, QSystemSemaphore::Create);

bool UtilLib::Reg::isInitialized = false;
unsigned int UtilLib::Reg::MAX_SPLIT_COUNT = 256;
unsigned int UtilLib::Reg::MAX_SECTION_BASE_LENGTH = 32;

const char *UtilLib::Reg::pVersion = "V00.00";
std::string UtilLib::Reg::base_dir = "/data/";
std::string UtilLib::Reg::filename_base = "reg_";

void UtilLib::Reg::set_basedir (const char *pBasedir){
    base_dir = pBasedir;
}

bool UtilLib::Reg::configure (const char *pCfgfilename){
    bool retval = false;
    std::string line;
    std::string str;
    std::string str2;
    try {
        std::ifstream infile( pCfgfilename ) ;
        if ( infile ) {
            if (getline( infile, line ) && line.compare(UtilLib::Reg::pVersion) == 0){
                if (getline( infile, line )){
                    base_dir = line;
//                    Log::Logger::debugV("UTILITY", "Reg::configure() base_dir: %s", base_dir.c_str());
                    if (getline( infile, line)){
                        filename_base = line;
//                        Log::Logger::debugV("UTILITY", "Reg::configure() filename_base: %s", filename_base.c_str());
                        retval = true;
                    }
                }
            } else {
                std::cout << "invalid cfg version" << std::endl;
            }
        } else {
            std::cout << "could not open config file" << std::endl;
        }
        infile.close( ) ;
    } catch (...){
        ;
    }
    if (!retval){
        std::cout << "configure() failed" << std::endl;
    }
    return retval;
}
void UtilLib::Reg::close() {
}
bool UtilLib::Reg::remove (const char *pSection, const char *pKey, bool removeEmptySection){
    bool retval = true;
    Section section;
    char section_name [MAX_SECTION_LENGTH+1];
    std::map<std::string, Section> sections;
    std::map<std::string, Section>::const_iterator cit;
    std::string line;
    std::vector<std::string> lines;
    std::string filename;
    try {
        semaphore.acquire();
        if (getFilename (&filename, pSection) && editSectionName(section_name, sizeof (section_name), pSection)){
            std::ifstream infile( filename.c_str() ) ;
            if ( infile ) {
                while ( getline(infile, line ) ) {
                    if (line.size() > sizeof ("#section ") - 1 && line.find("#section ") == 0) {
                        if (!lines.empty() && section.fromLines(&lines)){
                            sections [section.m_Name] = section;
                            lines.clear();
                        }
                        lines.push_back(line);
                    } else if (!lines.empty()){
                        lines.push_back(line);
                    } else {
                        //todo?
                    }
                }
                if (!lines.empty() && section.fromLines(&lines)){
                    sections [section.m_Name] = section;
                }
            } else {
                //todo?
            }
            infile.close( );
            if (sections.find(section_name) == sections.end()){
                section.init (section_name);
                sections [section_name] = section;
            }
            sections [section_name].removeKey (pKey);
            if (removeEmptySection && sections [section_name].empty()){
                sections.erase (section_name);
            }
            std::ofstream outfile (filename.c_str());
            if (outfile){
                for (cit = sections.begin(); cit != sections.end(); cit++){
                    std::string str;
                    cit->second.toString(&str);
                    outfile.write(str.c_str(), str.size());
                }
            } else {

            }
            outfile.close();
        }
    } catch (...){
        ; // todo
    }
    semaphore.release();
    return retval;
}
bool UtilLib::Reg::set(const char *pSection, const char *pKey, unsigned int value){
    bool retval = false;
    char buffer[16];
    ::sprintf (buffer, "%u", value);
    retval = set (pSection, pKey, buffer);
    return retval;
}
bool UtilLib::Reg::set (const char *pSection, const char *pKey, const char *pStrValue){
//    Log::Logger::debugV("UTILITY", "Reg::set() section: %s, key: %s, str: %s", pSection, pKey, pStrValue);
    bool retval = false;
    char buffer[MAX_STRING_LENGTH+1]; // one byte for trailing /0
    const void *pVoid = ::memchr (pStrValue, '\0', MAX_STRING_LENGTH);
    if (pVoid == NULL){
        *(buffer+MAX_STRING_LENGTH) = '\0';
        ::memcpy (buffer, pStrValue, MAX_STRING_LENGTH);
    } else {
        ::strcpy (buffer, pStrValue);
    }
    retval = setKeyString (pSection, pKey, buffer);
    return retval;
}
bool UtilLib::Reg::getFilename  (std::string *pOutFilename, const char *pSection){
    bool retval = false;
    *pOutFilename = base_dir;
    *pOutFilename += filename_base;
    const void *pVoid = ::memchr (pSection, '/', MAX_SECTION_BASE_LENGTH);
    if (pVoid != NULL){
        if (pVoid != pSection){
            pOutFilename->append (pSection, (char*)pVoid - pSection);
        }
        retval = true;
    } else {
        retval = ::memchr (pSection, '\0', MAX_SECTION_BASE_LENGTH+1) != NULL;
    }
//    Log::Logger::debugV("UTILITY", "Reg::getFilename() out filename: %s", pOutFilename->c_str());
    return retval;
}
bool UtilLib::Reg::editSectionName (char *pOutSection, size_t outbuf_size, const char *pSection){
    bool retval = false;
    const void *pVoid = ::memchr (pSection, '\0', MAX_SECTION_LENGTH+1);
    if (pVoid != NULL){
        pVoid = ::memchr (pSection, '/', MAX_SECTION_BASE_LENGTH);
        if (pVoid == NULL){
            ::strcpy (pOutSection, pSection);
        } else {
            ::strcpy (pOutSection, (const char*)pVoid+1);
        }
        retval = true;
    }
    return retval;
}
bool UtilLib::Reg::getKeyString (std::string *pOutStr, const char *pSection, const char *pKey){
    bool retval = false;
    char buffer [256];
    char section_name [MAX_SECTION_LENGTH+1];
    unsigned int keylen = 0;
    bool section_opened = false;
    std::string line;
    std::string filename;
    semaphore.acquire();
    if (getFilename (&filename, pSection) && editSectionName(section_name, sizeof (section_name), pSection)){
        keylen = ::strlen (pKey);
        std::ifstream infile( filename.c_str() ) ;
        if ( infile ) {
            ::sprintf (buffer, "#section %s", section_name);
            while ( getline( infile, line ) ) {
                if (section_opened){
                    if (line.find(buffer) == 0){
                        break;
                    } else if (line.find(pKey) == 0 && line.size() > keylen && line.at(keylen) == '='){
                        *pOutStr = line.substr(keylen+1).c_str();
                        retval = true;
                        break;
                    }
                } else {
                    if (line.find(buffer) == 0){
                        section_opened = true;
                    }
                }
            }
        }
        infile.close( ) ;
    }
    semaphore.release();
    return retval;
}
bool UtilLib::Reg::setKeyString (const char *pSection, const char *pKey, const char *pStrValue){
//    Log::Logger::debugV("UTILITY", "Reg::setKeyString() section: %s, key: %s, str: %s", pSection, pKey, pStrValue);
    bool retval = true;
    Section section;
    char section_name [MAX_SECTION_LENGTH+1];
    std::map<std::string, Section> sections;
    std::map<std::string, Section>::const_iterator cit;
    std::string line;
    std::vector<std::string> lines;
    std::string filename;
    try {
        semaphore.acquire();
//            Log::Logger::debug("UTILITY", "Reg::setKeyString() locked");
        if (getFilename (&filename, pSection) && editSectionName(section_name, sizeof (section_name), pSection)){
//                Log::Logger::debugV("UTILITY", "Reg::setKeyString() filename: %s", filename.c_str());
            std::ifstream infile( filename.c_str() ) ;
            if ( infile ) {
                while ( getline(infile, line ) ) {
                    if (line.size() > sizeof ("#section ") - 1 && line.find("#section ") == 0) {
                        if (!lines.empty() && section.fromLines(&lines)){
                            sections [section.m_Name] = section;
                            lines.clear();
                        }
                        lines.push_back(line);
                    } else if (!lines.empty()){
                        lines.push_back(line);
                    } else {
                        //todo?
                    }
                }
                if (!lines.empty() && section.fromLines(&lines)){
                    sections [section.m_Name] = section;
                }
            } else {
                //todo?
            }
            infile.close( );
            if (sections.find(section_name) == sections.end()){
                section.init (section_name);
                sections [section_name] = section;
            }
            sections [section_name].setKey (pKey, pStrValue);
            std::ofstream outfile (filename.c_str());
            if (outfile){
                for (cit = sections.begin(); cit != sections.end(); cit++){
                    std::string str;
                    cit->second.toString(&str);
                    outfile.write(str.c_str(), str.size());
                }
            } else {

            }
            outfile.close();
        }
    } catch (...){
        retval = true;
    }
    semaphore.release();
    return retval;
}
bool UtilLib::Reg::getUint(unsigned int* pOutValue, const char* pSection, const char* pKey){
    bool retval = false;
    std::string value ;
    if (getKeyString (&value, pSection, pKey)){
        *pOutValue = (unsigned int)::atoi (value.c_str());
        retval = true;
    }
    return retval;
}
bool UtilLib::Reg::getStr (std::string *pOutStr, const char *pSection, const char *pKey){
    bool retval = false;
    std::string str ;
    if (getKeyString (&str, pSection, pKey)){
        *pOutStr = str;
        retval = true;
    }
    return retval;
}
// section
void UtilLib::Reg::Section::init (const char *pName){
    m_Keys.clear();
    m_Name = pName;
}
void UtilLib::Reg::Section::toString (std::string *pStr) const{
    char buffer [MAX_STRING_LENGTH+64];
    std::map<std::string, std::string>::const_iterator cit;
    ::sprintf (buffer, "#section %s\n", m_Name.c_str());
    *pStr = buffer;
    for (cit = m_Keys.begin(); cit != m_Keys.end(); cit++){
        ::sprintf (buffer, "%s=%s\n", cit->first.c_str(), cit->second.c_str());
        *pStr += buffer;
    }
    *pStr += '\n';
}
bool UtilLib::Reg::Section::fromLines (const std::vector<std::string> *pLines){
    bool retval = false;
    std::vector<std::string>::const_iterator cit;
    this->init("noname");
    if (!pLines->empty()){
        std::string str = *pLines->begin();
        if (str.size() > sizeof ("#section ") - 1 && str.find("#section ") == 0) {
            retval = true;
            m_Name = str.substr(sizeof ("#section ") - 1);
            for (cit = pLines->begin()+1; cit != pLines->end(); cit++){
                size_t off = cit->find('=');
                if (off != std::string::npos){
                    m_Keys[cit->substr(0, off)] = cit->substr(off+1);
                } else {
                }
            }
        } else {
// todo ?
        }
    } else {
        //todo?
    }
    return retval;
}
void UtilLib::Reg::Section::setKey (const char *pKey, const char *pStrBuf){
    m_Keys[pKey] = pStrBuf;
}
void UtilLib::Reg::Section::removeKey (const char *pKey){
    m_Keys.erase(pKey);
}
bool UtilLib::Reg::Section::empty (){
    return m_Keys.empty();
}
UtilLib::Reg::Reg()
{
}
