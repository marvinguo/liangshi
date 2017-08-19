#ifndef GLOBALINIT_H
#define GLOBALINIT_H

typedef void* (*LPFNGLOBALINIT)();
void registerGlobalInitializer(const char *name, LPFNGLOBALINIT lpfn);
class GlobalInitializerRegisterHelper
{
public:
    GlobalInitializerRegisterHelper(const char *name, LPFNGLOBALINIT lpfn)
    {
        registerGlobalInitializer(name, lpfn);
    }
};
#define REGISTER_GLOBAL_INITIALIZER(NAME, FUNC) \
    static GlobalInitializerRegisterHelper g_initRegister(NAME, FUNC)

void globalInitialize();
void *findGlobalService(const char * name);

#endif // GLOBALINIT_H
