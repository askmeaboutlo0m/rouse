#if defined(__EMSCRIPTEN__)

#   define R_DL_OPEN(NAME, OUT_HANDLE, OUT_ERROR) \
        _Static_assert(0, "R_DL_OPEN not supported on Emscripten")

#   define R_DL_SYM(HANDLE, NAME, OUT_ADDRESS, OUT_ERROR) \
        _Static_assert(0, "R_DL_SYM not supported on Emscripten")

#elif defined(_WIN32)

#   include <windows.h>

#   define R_DL_OPEN(NAME, OUT_HANDLE, OUT_ERROR) do { \
            HMODULE _dl_handle = LoadLibraryA(NAME ".dll"); \
            (OUT_HANDLE)       = _dl_handle; \
            if (!_dl_handle) { \
                (OUT_ERROR) = R_win32_strerror(GetLastError()); \
            } \
        } while (0)

#   define R_DL_SYM(HANDLE, NAME, OUT_ADDRESS, OUT_ERROR) do { \
            FARPROC _dl_address = GetProcAddress(HANDLE, NAME); \
            (OUT_ADDRESS)       = *(void **)(&_dl_address); \
            if (!_dl_address) { \
                (OUT_ERROR) = R_win32_strerror(GetLastError()); \
            } \
        } while (0)

#else

#   include <dlfcn.h>

#   define R_DL_OPEN(NAME, OUT_HANDLE, OUT_ERROR) do { \
            void *_dl_handle = dlopen("lib" NAME ".so", RTLD_NOW | RTLD_LOCAL); \
            (OUT_HANDLE)     = _dl_handle; \
            if (!_dl_handle) { \
                (OUT_ERROR) = dlerror(); \
            } \
        } while (0)

#   define R_DL_SYM(HANDLE, NAME, OUT_ADDRESS, OUT_ERROR) do { \
            void *_dl_address = dlsym(HANDLE, NAME); \
            (OUT_ADDRESS)     = _dl_address; \
            if (!_dl_address) { \
                (OUT_ERROR) = "symbol not found"; \
            } \
        } while (0)

#endif

/*
 * Converting a void * to a function pointer isn't ISO C, so it triggers a
 * pedantic warning. This here is a standards-compliant workaround apparently,
 * according to my dlsym docs. It doesn't trigger the warning anyway.
 */
#define R_DL_ASSIGN(FUNC_PTR, VOID_PTR) *(void **)(&FUNC_PTR) = VOID_PTR
