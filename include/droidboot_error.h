#ifndef DROIDBOOT_ERROR_H
#define DROIDBOOT_ERROR_H
typedef unsigned int droidboot_ret;

#define DROIDBOOT_EPERM 1         /**< Not owner */
#define DROIDBOOT_ENOENT 2        /**< No such file or directory */
#define DROIDBOOT_ESRCH 3         /**< No such context */
#define DROIDBOOT_EINTR 4         /**< Interrupted system call */
#define DROIDBOOT_EIO 5           /**< I/O error */
#define DROIDBOOT_ENXIO 6         /**< No such device or address */
#define DROIDBOOT_E2BIG 7         /**< Arg list too long */
#define DROIDBOOT_ENOEXEC 8       /**< Exec format error */
#define DROIDBOOT_EBADF 9         /**< Bad file number */
#define DROIDBOOT_ECHILD 10       /**< No children */
#define DROIDBOOT_EAGAIN 11       /**< No more contexts */
#define DROIDBOOT_ENOMEM 12       /**< Not enough core */
#define DROIDBOOT_EACCES 13       /**< Permission denied */
#define DROIDBOOT_EFAULT 14       /**< Bad address */
#define DROIDBOOT_ENOTBLK 15      /**< Block device required */
#define DROIDBOOT_EBUSY 16        /**< Mount device busy */
#define DROIDBOOT_EEXIST 17       /**< File exists */
#define DROIDBOOT_EXDEV 18        /**< Cross-device link */
#define DROIDBOOT_ENODEV 19       /**< No such device */
#define DROIDBOOT_ENOTDIR 20      /**< Not a directory */
#define DROIDBOOT_EISDIR 21       /**< Is a directory */
#define DROIDBOOT_EINVAL 22       /**< Invalid argument */
#define DROIDBOOT_ENFILE 23       /**< File table overflow */
#define DROIDBOOT_EMFILE 24       /**< Too many open files */
#define DROIDBOOT_ENOTTY 25       /**< Not a typewriter */
#define DROIDBOOT_ETXTBSY 26      /**< Text file busy */
#define DROIDBOOT_EFBIG 27        /**< File too large */
#define DROIDBOOT_ENOSPC 28       /**< No space left on device */
#define DROIDBOOT_ESPIPE 29       /**< Illegal seek */
#define DROIDBOOT_EROFS 30        /**< Read-only file system */
#define DROIDBOOT_EMLINK 31       /**< Too many links */
#define DROIDBOOT_EPIPE 32        /**< Broken pipe */
#define DROIDBOOT_EDOM 33         /**< Argument too large */
#define DROIDBOOT_ERANGE 34       /**< Result too large */
#define DROIDBOOT_ENOMSG 35       /**< Unexpected message type */
#define DROIDBOOT_EDEADLK 45      /**< Resource deadlock avoided */
#define DROIDBOOT_ENOLCK 46       /**< No locks available */
#define DROIDBOOT_ENOSTR 60       /**< STREAMS device required */
#define DROIDBOOT_ENODATA 61      /**< Missing expected message data */
#define DROIDBOOT_ETIME 62        /**< STREAMS timeout occurred */
#define DROIDBOOT_ENOSR 63        /**< Insufficient memory */
#define DROIDBOOT_EPROTO 71       /**< Generic STREAMS error */
#define DROIDBOOT_EBADMSG 77      /**< Invalid STREAMS message */
#define DROIDBOOT_ENOSYS 88       /**< Function not implemented */
#define DROIDBOOT_ENOTEMPTY 90    /**< Directory not empty */
#define DROIDBOOT_ENAMETOOLONG 91 /**< File name too long */
#define DROIDBOOT_ELOOP 92        /**< Too many levels of symbolic links */
#define DROIDBOOT_EOPNOTSUPP 95   /**< Operation not supported on socket */
#define DROIDBOOT_EPFNOSUPPORT 96 /**< Protocol family not supported */
#define DROIDBOOT_ECONNRESET 104   /**< Connection reset by peer */
#define DROIDBOOT_ENOBUFS 105      /**< No buffer space available */
#define DROIDBOOT_EAFNOSUPPORT 106 /**< Addr family not supported */
#define DROIDBOOT_EPROTOTYPE 107   /**< Protocol wrong type for socket */
#define DROIDBOOT_ENOTSOCK 108     /**< Socket operation on non-socket */
#define DROIDBOOT_ENOPROTOOPT 109  /**< Protocol not available */
#define DROIDBOOT_ESHUTDOWN 110    /**< Can't send after socket shutdown */
#define DROIDBOOT_ECONNREFUSED 111 /**< Connection refused */
#define DROIDBOOT_EADDRINUSE 112   /**< Address already in use */
#define DROIDBOOT_ECONNABORTED 113 /**< Software caused connection abort */
#define DROIDBOOT_ENETUNREACH 114  /**< Network is unreachable */
#define DROIDBOOT_ENETDOWN 115     /**< Network is down */
#define DROIDBOOT_ETIMEDOUT 116    /**< Connection timed out */
#define DROIDBOOT_EHOSTDOWN 117    /**< Host is down */
#define DROIDBOOT_EHOSTUNREACH 118 /**< No route to host */
#define DROIDBOOT_EINPROGRESS 119  /**< Operation now in progress */
#define DROIDBOOT_EALREADY 120     /**< Operation already in progress */
#define DROIDBOOT_EDESTADDRREQ 121 /**< Destination address required */
#define DROIDBOOT_EMSGSIZE 122        /**< Message size */
#define DROIDBOOT_EPROTONOSUPPORT 123 /**< Protocol not supported */
#define DROIDBOOT_ESOCKTNOSUPPORT 124 /**< Socket type not supported */
#define DROIDBOOT_EADDRNOTAVAIL 125   /**< Can't assign requested address */
#define DROIDBOOT_ENETRESET 126       /**< Network dropped connection on reset */
#define DROIDBOOT_EISCONN 127         /**< Socket is already connected */
#define DROIDBOOT_ENOTCONN 128        /**< Socket is not connected */
#define DROIDBOOT_ETOOMANYREFS 129    /**< Too many references: can't splice */
#define DROIDBOOT_ENOTSUP 134         /**< Unsupported value */
#define DROIDBOOT_EILSEQ 138          /**< Illegal byte sequence */
#define DROIDBOOT_EOVERFLOW 139       /**< Value overflow */
#define DROIDBOOT_ECANCELED 140       /**< Operation canceled */

#endif
