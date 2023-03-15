#ifndef SCORECONF_H
#define SCORECONF_H

#define S_USING_STATICSECTION
// #define S_USING_MODULE
// #define S_USING_SESSION
// #define S_USING_CONFIG
// #define S_USING_SCHEDULER

/* data type */
typedef int _err_t; /**< Type for error number */

/* error code definitions */
#define _EOK 0      /**< There is no error */
#define _ERROR 1    /**< A generic error happens */
#define _ETIMEOUT 2 /**< Timed out */
#define _EFULL 3    /**< The resource is full */
#define _EEMPTY 4   /**< The resource is empty */
#define _ENOMEM 5   /**< No memory */
#define _ENOSYS 6   /**< No system */
#define _EBUSY 7    /**< Busy */
#define _EIO 8      /**< IO error */
#define _EINTR 9    /**< Interrupted system call */
#define _EINVAL 10  /**< Invalid argument */

#endif // SCORECONF_H
