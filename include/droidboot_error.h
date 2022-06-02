#ifndef DROIDBOOT_ERROR_H
#define DROIDBOOT_ERROR_H
typedef unsigned int droidboot_ret;

typedef enum droidboot_error {
    DROIDBOOT_EOK = 0,           /**< No errors */
    DROIDBOOT_EPERM = 1,         /**< Not owner */
    DROIDBOOT_ENOENT = 2,        /**< No such file or directory */
    DROIDBOOT_ESRCH = 3,         /**< No such context */
    DROIDBOOT_EINTR = 4,         /**< Interrupted system call */
    DROIDBOOT_EIO = 5,           /**< I/O error */
    DROIDBOOT_ENXIO = 6,         /**< No such device or address */
    DROIDBOOT_E2BIG = 7,         /**< Arg list too long */
    DROIDBOOT_ENOEXEC = 8,       /**< Exec format error */
    DROIDBOOT_EBADF = 9,         /**< Bad file number */
    DROIDBOOT_ECHILD = 10,       /**< No children */
    DROIDBOOT_EAGAIN = 11,       /**< No more contexts */
    DROIDBOOT_ENOMEM = 12,       /**< Not enough core */
    DROIDBOOT_EACCES = 13,       /**< Permission denied */
    DROIDBOOT_EFAULT = 14,       /**< Bad address */
    DROIDBOOT_ENOTBLK = 15,      /**< Block device required */
    DROIDBOOT_EBUSY = 16,        /**< Mount device busy */
    DROIDBOOT_EEXIST = 17,       /**< File exists */
    DROIDBOOT_EXDEV = 18,        /**< Cross-device link */
    DROIDBOOT_ENODEV = 19,       /**< No such device */
    DROIDBOOT_ENOTDIR = 20,      /**< Not a directory */
    DROIDBOOT_EISDIR = 21,       /**< Is a directory */
    DROIDBOOT_EINVAL = 22,       /**< Invalid argument */
    DROIDBOOT_ENFILE = 23,       /**< File table overflow */
    DROIDBOOT_EMFILE = 24,       /**< Too many open files */
    DROIDBOOT_ENOTTY = 25,       /**< Not a typewriter */
    DROIDBOOT_ETXTBSY = 26,      /**< Text file busy */
    DROIDBOOT_EFBIG = 27,        /**< File too large */
    DROIDBOOT_ENOSPC = 28,       /**< No space left on device */
    DROIDBOOT_ESPIPE = 29,       /**< Illegal seek */
    DROIDBOOT_EROFS = 30,        /**< Read-only file system */
    DROIDBOOT_EMLINK = 31,       /**< Too many links */
    DROIDBOOT_EPIPE = 32,        /**< Broken pipe */
    DROIDBOOT_EDOM = 33,         /**< Argument too large */
    DROIDBOOT_ERANGE = 34,       /**< Result too large */
    DROIDBOOT_ENOMSG = 35,       /**< Unexpected message type */
    DROIDBOOT_EDEADLK = 45,      /**< Resource deadlock avoided */
    DROIDBOOT_ENOLCK = 46,       /**< No locks available */
    DROIDBOOT_ENOSTR = 60,       /**< STREAMS device required */
    DROIDBOOT_ENODATA = 61,      /**< Missing expected message data */
    DROIDBOOT_ETIME = 62,        /**< STREAMS timeout occurred */
    DROIDBOOT_ENOSR = 63,        /**< Insufficient memory */
    DROIDBOOT_EPROTO = 71,       /**< Generic STREAMS error */
    DROIDBOOT_EBADMSG = 77,      /**< Invalid STREAMS message */
    DROIDBOOT_ENOSYS = 88,       /**< Function not implemented */
    DROIDBOOT_ENOTEMPTY = 90,    /**< Directory not empty */
    DROIDBOOT_ENAMETOOLONG = 91, /**< File name too long */
    DROIDBOOT_ELOOP = 92,        /**< Too many levels of symbolic links */
    DROIDBOOT_EOPNOTSUPP = 95,   /**< Operation not supported on socket */
    DROIDBOOT_EPFNOSUPPORT = 96, /**< Protocol family not supported */
    DROIDBOOT_ECONNRESET = 104,   /**< Connection reset by peer */
    DROIDBOOT_ENOBUFS = 105,      /**< No buffer space available */
    DROIDBOOT_EAFNOSUPPORT = 106, /**< Addr family not supported */
    DROIDBOOT_EPROTOTYPE = 107,   /**< Protocol wrong type for socket */
    DROIDBOOT_ENOTSOCK = 108,     /**< Socket operation on non-socket */
    DROIDBOOT_ENOPROTOOPT = 109,  /**< Protocol not available */
    DROIDBOOT_ESHUTDOWN = 110,    /**< Can't send after socket shutdown */
    DROIDBOOT_ECONNREFUSED = 111, /**< Connection refused */
    DROIDBOOT_EADDRINUSE = 112,   /**< Address already in use */
    DROIDBOOT_ECONNABORTED = 113, /**< Software caused connection abort */
    DROIDBOOT_ENETUNREACH = 114,  /**< Network is unreachable */
    DROIDBOOT_ENETDOWN = 115,     /**< Network is down */
    DROIDBOOT_ETIMEDOUT = 116,    /**< Connection timed out */
    DROIDBOOT_EHOSTDOWN = 117,    /**< Host is down */
    DROIDBOOT_EHOSTUNREACH = 118, /**< No route to host */
    DROIDBOOT_EINPROGRESS = 119,  /**< Operation now in progress */
    DROIDBOOT_EALREADY = 120,     /**< Operation already in progress */
    DROIDBOOT_EDESTADDRREQ = 121, /**< Destination address required */
    DROIDBOOT_EMSGSIZE = 122,        /**< Message size */
    DROIDBOOT_EPROTONOSUPPORT = 123, /**< Protocol not supported */
    DROIDBOOT_ESOCKTNOSUPPORT = 124, /**< Socket type not supported */
    DROIDBOOT_EADDRNOTAVAIL = 125,   /**< Can't assign requested address */
    DROIDBOOT_ENETRESET = 126,       /**< Network dropped connection on reset */
    DROIDBOOT_EISCONN = 127,         /**< Socket is already connected */
    DROIDBOOT_ENOTCONN = 128,        /**< Socket is not connected */
    DROIDBOOT_ETOOMANYREFS = 129,    /**< Too many references: can't splice */
    DROIDBOOT_ENOTSUP = 134,         /**< Unsupported value */
    DROIDBOOT_EILSEQ = 138,          /**< Illegal byte sequence */
    DROIDBOOT_EOVERFLOW = 139,       /**< Value overflow */
    DROIDBOOT_ECANCELED = 140,       /**< Operation canceled */
} droidboot_error;

#endif
