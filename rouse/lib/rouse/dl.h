#include <dlfcn.h>

/*
 * Converting a void * to a function pointer isn't ISO C, so it triggers a
 * pedantic warning. This here is a standards-compliant workaround apparently,
 * according to my dlsym docs. It doesn't trigger the warning anyway.
 */
#define R_DL_ASSIGN(FUNC_PTR, VOID_PTR) *(void **)(&FUNC_PTR) = VOID_PTR

#define R_DL_OPEN(NAME) \
    dlopen("lib" NAME ".so", RTLD_NOW | RTLD_LOCAL)

#define R_DL_SYM(HANDLE, NAME) \
    dlsym(HANDLE, NAME)

#define R_DL_ERROR(HANDLE) \
    dlerror()
