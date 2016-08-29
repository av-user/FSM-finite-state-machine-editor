#ifndef REG_H
#define REG_H

#include "utillib_global.h"
#include <vector>
#include <map>
#include <sys/types.h>
#include <string>
#include <QMutex>
#include <QSystemSemaphore>

namespace UtilLib {

    class UTILLIBSHARED_EXPORT Reg {
    public:
        static void set_basedir (const char *pBasedir);
        static void close ();
        static bool set(const char *pSection, const char *pKey, unsigned int value);
        static bool set (const char *pSection, const char *pKey, const char *pStrValue);
        static bool getUint(unsigned int* pOutValue, const char* pSection, const char* pKey);
        static bool getStr (std::string *pOutStr, const char *pSection, const char *pKey);
        static bool remove (const char *pSection, const char *pKey, bool removeEmptySection = false);

        Reg();
    private:
        static QMutex mutex;
        static QSystemSemaphore semaphore;
        static bool isInitialized;
        static unsigned int MAX_SPLIT_COUNT;
        static unsigned int MAX_SECTION_BASE_LENGTH;
        static std::string filename_base;
        static std::string base_dir;
        static const char *pVersion;

        static bool configure (const char *pCfgfilename);
        static bool setKeyString (const char *pSection, const char *pKey, const char *pStrValue);
        static bool getKeyString (std::string *pOutStr, const char *pSection, const char *pKey);
        static bool getFilename  (std::string *pOutFilename, const char *pSection);
        static bool editSectionName (char *pOutSection, size_t outbuf_size, const char *pSection);

        class Section {
        public:
            void init (const char *pName);
            bool fromLines (const std::vector<std::string> *pLines);
            void toString (std::string *pStr) const;
            void addKey (const char *pKey, const char *pStrBuf);
            void setKey (const char *pKey, const char *pStrBuf);
            void removeKey (const char *pKey);
            bool empty ();
            std::string	m_Name;
        private:
            std::map<std::string, std::string> m_Keys;
        };
    };
};
#endif // REG_H
