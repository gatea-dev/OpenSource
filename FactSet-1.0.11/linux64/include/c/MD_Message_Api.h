#if defined _WIN32 || defined __CYGWIN__
#define MD_MESSAGE_C_API __declspec(dllexport)
#else
#if __GNUC__ >= 4
#define MD_MESSAGE_C_API __attribute__((visibility("default")))
#else
#define MD_MESSAGE_C_API
#endif
#endif