#include "core.hpp"
#include "ossPrimitiveFileOp.hpp"
#include "pd.hpp"

ossPrimitiveFileOp::ossPrimitiveFileOp()
{
    _fileHandle = OSS_INVALID_HANDLE_FD_VALUE;
    _bIsStdout = false;
}

bool ossPrimitiveFileOp::isValid()
{
    return ( OSS_INVALID_HANDLE_FD_VALUE != _fileHandle );
}

void ossPrimitiveFileOp::Close()
{
    if ( isValid() && ( !_bIsStdout ) )
    {
        oss_close(_fileHandle);
        _fileHandle = OSS_INVALID_HANDLE_FD_VALUE;
    }
}

int ossPrimitiveFileOp::Open(const char *pFilePath, unsigned int options)
{
    int rc = 0;
    int mode = O_RDWR;

    if (options & OSS_PRIMITIVE_FILE_OP_READ_ONLY)
    {
        mode = O_RDONLY;
    }
    else if (options & OSS_PRIMITIVE_FILE_OP_WRITE_ONLY)
    {
        mode = O_WRONLY;
    }

    if (options & OSS_PRIMITIVE_FILE_OP_OPEN_EXISTING)
    {        
    }
    else if (options & OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS)
    {
        mode |= O_CREAT;    //若此文件不存在则创建它。使用此选项时需要提供第三个参数mode ,表示该文件
                            //的访问权限。
    }
    if (options & OSS_PRIMITIVE_FILE_OP_OPEN_TRUNC)
    {
        mode |= O_TRUNC;    // 打开时从0开始写入
    }

    // 防止中断
    do {
        _fileHandle = oss_open(pFilePath, mode, 0644);
    } while ( (-1 == _fileHandle) && (EINTR == errno) );

    if (_fileHandle <= OSS_INVALID_HANDLE_FD_VALUE)
    {
        rc = errno;
        goto exit;
    }
exit:
    return rc;
}

void ossPrimitiveFileOp::openStdout()
{
    setFileHandle(STDOUT_FILENO);
    _bIsStdout = true;
}

offsetType ossPrimitiveFileOp::getCurrentOffset() const
{
    return oss_lseek(_fileHandle, 0, SEEK_CUR);
}

int ossPrimitiveFileOp::seekToEnd(void) 
{
   int rc = EDB_OK; 
   rc = oss_lseek( _fileHandle, 0, SEEK_END ) ;
   if (rc == -1) {
      PD_LOG(PDERROR, "seek file error, errno = %d", errno);
   }
   return rc;
}

// 跳转到指定的偏移
void ossPrimitiveFileOp::seekToOffset(offsetType offset)
{
    if ( ( oss_off_t )-1 != offset )
    {
        oss_lseek(_fileHandle, offset, SEEK_SET);
    }
}

/**
 * size: 读的大小
 * pBufferL 缓冲区
 * pBytesRead: 已经读的大小
 * */
int ossPrimitiveFileOp::Read(   const size_t size,
                                void * const pBuffer,
                                int * const pBytesRead)
{
    int retval = 0;
    ssize_t bytesRead = 0;  // 已经读的长度
    if ( isValid() )
    {
        do {
            bytesRead = oss_read(_fileHandle, pBuffer, size);
        } while ( (-1 == bytesRead) && (EINTR == errno) );
        if (-1 == bytesRead) {
            goto err_read;
        }
    }
    else 
    {
        goto err_read;
    }

    if ( pBytesRead ) 
    {
        *pBytesRead = bytesRead;
    }
exit:
    return retval;
err_read:
    *pBytesRead = 0;
    retval = errno;
    goto exit;
}

// size: 写入长度, size == 0表示写到字符串的结尾
int ossPrimitiveFileOp::Write(const void * pBuffer, size_t size)
{
    int rc = 0;
    size_t currentSize = 0;
    if ( 0 == size )
    {
        size = strlen( (char*)pBuffer );
    }

    if (isValid())
    {
        do {
            rc = oss_write( _fileHandle, &((char*)pBuffer)[currentSize], size-currentSize);
            if (rc >= 0)
                currentSize += rc; 
        } while (   (( -1 == rc) && ( EINTR == errno)) ||
                    (( -1 != rc) && ( currentSize != size)) );
        if ( -1 == rc ) 
        {
            rc = errno;
            goto exit;
        }
        rc = 0;
    }
exit:
    return rc;
}

// 自定义格式写入
int ossPrimitiveFileOp::fWrite ( const char * format, ... )
{
    int rc = 0;
    va_list ap;
    char buf[OSS_PRIMITIVE_FILE_OP_FWRITE_BUF_SIZE] = {0};

    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);

    rc = Write(buf, strlen(buf));
    return rc;
}

void ossPrimitiveFileOp::setFileHandle( handleType handle )
{
    _fileHandle = handle;
}

int ossPrimitiveFileOp::getSize( offsetType * const pFileSize )
{
    int rc = 0;
    oss_struct_stat buf = {0};

    if ( -1 == oss_fstat( _fileHandle, &buf) )
    {
        rc = errno;
        goto err_exit;
    }

    *pFileSize = buf.st_size;

exit:
    return rc;
err_exit:
    *pFileSize = 0;
    goto exit;
}