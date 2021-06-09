#ifndef FDSFE_C_API

#if defined _WIN32 || defined __CYGWIN__
#define FDSFE_C_API __declspec(dllexport)
#else
#if __GNUC__ >= 4
#define FDSFE_C_API __attribute__((visibility("default")))
#else
#define FDSFE_C_API
#endif
#endif

#endif //FDSFE_C_API
