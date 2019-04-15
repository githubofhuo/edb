
#ifndef OSSPRIMITIVEFILEOP_HPP__
#define OSSPRIMITIVEFILEOP_HPP__

#include "core.hpp"

#define OSS_F_GETLK        F_GETLK64
#define OSS_F_SETLK        F_SETLK64
#define OSS_F_SETLKW       F_SETLKW64

#define oss_struct_statfs  struct statfs64
#define oss_statfs         statfs64
#define oss_fstatfs        fstatfs64
#define oss_struct_statvfs struct statvfs64
#define oss_statvfs        statvfs64
#define oss_fstatvfs       fstatvfs64
#define oss_struct_stat    struct stat64
#define oss_struct_flock   struct flock64
#define oss_stat           stat64
#define oss_lstat          lstat64
#define oss_fstat          fstat64
#define oss_open           open64
#define oss_lseek          lseek64
#define oss_ftruncate      ftruncate64
#define oss_off_t          off64_t
#define oss_close          close
#define oss_access         access
#define oss_chmod          chmod
#define oss_read           read
#define oss_write          write


#define OSS_FILE_OP_FWRITE_BUF_SIZE 2048
#define OSS_FILE_OP_READ_ONLY     (((unsigned int)1) << 1)
#define OSS_FILE_OP_WRITE_ONLY    (((unsigned int)1) << 2)
#define OSS_FILE_OP_OPEN_EXISTING (((unsigned int)1) << 3)
#define OSS_FILE_OP_OPEN_ALWAYS   (((unsigned int)1) << 4)
#define OSS_FILE_OP_OPEN_TRUNC    (((unsigned int)1) << 5)

#define OSS_INVALID_HANDLE_FD_VALUE (-1)

typedef oss_off_t offsetType ;

class ossFileOp {
public:
   typedef int       handleType;
private:
   handleType        _fileHandle;
   ossFileOp(const ossFileOp &){}
   const ossFileOp &operator=(const ossFileOp&);
   bool _bIsStdout;

protected:
   void setFileHandle(handleType handle);

public:
   ossFileOp();
   int Open(const char *pFilePath, unsigned int options = OSS_FILE_OP_OPEN_ALWAYS);
   void openStdout();
   void Close();
   bool isValid(void);
   int Read(void *pBuf, size_t len, int *const pReadSize);
   int Write(void *pBuf, size_t len);
   int fWrite(const char *fmt, ...);
   offsetType getCurrentOffset (void) const;
   int seekToOffset(offsetType offset);
   int seekToEnd(void);
   int getSize(offsetType *const pFileSize);
   handleType getHandle(void) const {
      return _fileHandle;
   }

};

#endif