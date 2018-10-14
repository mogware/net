#ifndef __NET_CONFIG__
#define __NET_CONFIG__

#if defined(_MSC_VER)
#define NET_MSVC
#if !defined(_DEBUG) && !defined(__NET_INLINE__)
#define __NET_INLINE__
#endif
#endif

#if defined(__GNUC__)
#define NET_GCC
#if __GNUC__ < 4
#define NET_GCC3
#endif
#if !defined (__NET_INLINE__)
#define __NET_INLINE__
#endif
#endif

#if defined(NET_LACKS_INLINE_FUNCTIONS) && !defined(NET_NO_INLINE)
#define NET_NO_INLINE
#endif

#if defined(NET_NO_INLINE)
#undef __NET_INLINE__
#endif

#if defined(__NET_INLINE__)
#define NET_INLINE inline
#else
#define NET_INLINE
#endif

#endif
