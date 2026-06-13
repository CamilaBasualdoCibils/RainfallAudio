#pragma once

// Detect if we are building on Windows
#if defined(_WIN32) || defined(_WIN64)
    #define RAINFALL_API_WINDOWS 1
#else
    #define RAINFALL_API_WINDOWS 0
#endif

// Optionally define if building static library
#if defined(RAINFALLAPI_STATIC)
    #define RAINFALL_API
#else
    // If Windows and not static, define export/import macros
    #if RAINFALL_API_WINDOWS
        #if defined(RAINFALLAPI_EXPORTS)
            // Building the DLL
            #define RAINFALL_API __declspec(dllexport)
        #else
            // Using the DLL
            #define RAINFALL_API __declspec(dllimport)
        #endif
    #else
        // Non-Windows platforms: no special attributes needed
        #define RAINFALL_API __attribute__((visibility("default")))
    #endif
#endif