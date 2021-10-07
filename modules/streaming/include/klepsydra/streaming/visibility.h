// Generic helper definitions for shared library support
#if __GNUC__ >= 4
#define KPSR_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define KPSR_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define KPSR_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define KPSR_HELPER_DLL_IMPORT
#define KPSR_HELPER_DLL_EXPORT
#define KPSR_HELPER_DLL_LOCAL
#endif

// Now we use the generic helper definitions above to define KPSR_API and KPSR_LOCAL.
// KPSR_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// KPSR_LOCAL is used for non-api symbols.

#ifdef KPSR_DLL // defined if KPSR is compiled as a DLL
#ifdef KPSR_DLL_EXPORTS // defined if we are building the KPSR DLL (instead of using it)
#define KPSR_API KPSR_HELPER_DLL_EXPORT
#else
#define KPSR_API KPSR_HELPER_DLL_IMPORT
#endif // KPSR_DLL_EXPORTS
#define KPSR_LOCAL KPSR_HELPER_DLL_LOCAL
#else // KPSR_DLL is not defined: this means KPSR is a static lib.
#define KPSR_API
#define KPSR_LOCAL
#endif // KPSR_DLL
