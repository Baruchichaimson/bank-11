#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char *argv[]) 
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s <shared_object.so> <function_name>\n", argv[0]);
        return 1;
    }

    const char *so_file = argv[1];
    const char *func_name = argv[2];

    void *handle = dlopen(so_file, RTLD_LAZY);
    if (!handle) 
    {
        fprintf(stderr, "Error loading %s: %s\n", so_file, dlerror());
        return 1;
    }

    dlerror();

    void (*func)(void) = dlsym(handle, func_name);
    char *error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "Error finding symbol %s: %s\n", func_name, error);
        dlclose(handle);
        return 1;
    }

    func();

    dlclose(handle);
    return 0;
}

