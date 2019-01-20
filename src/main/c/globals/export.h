#ifndef EXPORT_H
#define EXPORT_H

#ifdef _WIN32
#    ifdef MODULE_API_EXPORTS
#        define MODULE_API __declspec(dllexport)
#    else
#        define MODULE_API __declspec(dllimport)
#    endif
#else
#    define MODULE_API
#endif

#endif
