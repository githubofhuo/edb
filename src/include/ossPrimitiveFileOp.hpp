// oss 基础文件操作类
#ifndef OSSPRIMITIVEFILEOP_HPP__
#define OSSPRIMITIVEFILEOP_HPP__

#include "core.hpp"

// 标准linux文件操作
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


#define OSS_PRIMITIVE_FILE_OP_FWRITE_BUF_SIZE 2048
#define OSS_PRIMITIVE_FILE_OP_READ_ONLY     (((unsigned int)1) << 1)
#define OSS_PRIMITIVE_FILE_OP_WRITE_ONLY    (((unsigned int)1) << 2)
#define OSS_PRIMITIVE_FILE_OP_OPEN_EXISTING (((unsigned int)1) << 3)
#define OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS   (((unsigned int)1) << 4)
#define OSS_PRIMITIVE_FILE_OP_OPEN_TRUNC    (((unsigned int)1) << 5)

#define OSS_INVALID_HANDLE_FD_VALUE (-1)

// 文件偏移类型
typedef oss_off_t offsetType; 

class ossPrimitiveFileOp
{
public:
    typedef int handleType;
private:
    handleType _fileHandle;
    ossPrimitiveFileOp(const ossPrimitiveFileOp & ) {}
    const ossPrimitiveFileOp &operator = (const ossPrimitiveFileOp & );
    bool _bIsStdout;

protected:
    // 根据已知的文件句柄生成文件
    void setFileHandle( handleType handle);

public:
    ossPrimitiveFileOp();
    int Open(const char *pFilePath, unsigned int options = OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS);
    // 将屏幕作为文件输出
    void openStdout();
    void Close();
    bool isValid(void);
    // 在内存中读取一个定长的信息，返回已读长度
    int Read(const size_t size,
            void * const pBuffer,
            int * const pBytesRead);
    //int Read(void *pBuf, size_t len, int *const pReadSize);
    // 向文件写入buf中指定长度的信息
    int Write(const void *pBuf, size_t len);
    // 格式写入文件
    int fWrite(const char *fmt, ...);
    // 偏移
    offsetType getCurrentOffset (void) const;
    void seekToOffset(offsetType offset);
    int seekToEnd(void);
    int getSize(offsetType *const pFileSize);
    handleType getHandle(void) const {
        return _fileHandle;
    }
};

#endif