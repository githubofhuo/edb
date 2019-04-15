
#ifndef PD_HPP__
#define PD_HPP__

#include <string>

#define PD_LOG_STRINGMAX 4096

#define PD_LOG(level,fmt,...)       \
   do {                             \
      if (level <= _curPDLevel) {    \
         pdLog(level, __func__, __FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
      }                             \
   } while(0)                       \

#define PD_CHECK(cond,retCode,gotoLabel,level,fmt,...)         \
   do {                                                        \
      if (!(cond)) {                                           \
         rc = (retCode);                                       \
         PD_LOG((level),fmt, ##__VA_ARGS__);                   \
         goto gotoLabel;                                       \
      }                                                        \
   } while(0)                                                  \

#define PD_RC_CHECK(rc,level,fmt,...)                          \
   do {                                                        \
      PD_CHECK ((EDB_OK==(rc)), (rc), error, (level),          \
                 fmt, ##__VA_ARGS__) ;                         \
   } while(0)                                                  \


enum PDLEVEL {
   PDCRITICAL = 0,
   PDERROR,
   PDEVENT,
   PDWARNING,
   PDINFO,
   PDTRACE
};

#define PD_DFT_DIAGLEVEL PDTRACE

extern PDLEVEL _curPDLevel;
extern PDLEVEL _curPDFileLevel;
const char *getPDLevel (PDLEVEL level);

void pdLog(PDLEVEL level, const char *func, const char *file,
         unsigned int line, const char *format, ...);

void pdLog(PDLEVEL level, const char *func, const char *file,
         unsigned int line, std::string message);
#endif