#include "globalInit.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct GlobalInitializer
{
    char *name;
    LPFNGLOBALINIT func;
    void *handle;
};
static GlobalInitializer * g_globalInitializer = 0;
static int g_initializerCapacity = 0;
static int g_initializerSize = 0;
#define GLOBAL_INIT_SIZE 8
#define GLOBAL_INCREMENT 4

void registerGlobalInitializer(const char *name, LPFNGLOBALINIT lpfn)
{
    if(g_globalInitializer == NULL)
    {
        g_globalInitializer = (GlobalInitializer*)calloc(GLOBAL_INIT_SIZE, sizeof(GlobalInitializer));
        g_initializerCapacity = GLOBAL_INIT_SIZE;
        g_initializerSize = 0;
    }

    if(g_initializerSize == g_initializerCapacity)
    {
        g_initializerCapacity += GLOBAL_INCREMENT;
        g_globalInitializer = (GlobalInitializer*)realloc(g_globalInitializer, sizeof(GlobalInitializer)*g_initializerCapacity);
    }

    g_globalInitializer[g_initializerSize].func = lpfn;
    g_globalInitializer[g_initializerSize].handle = 0;
    g_globalInitializer[g_initializerSize++].name = strdup(name);
}

void globalInitialize()
{
    for(int i = 0; i < g_initializerSize; i++)
    {
        g_globalInitializer[i].handle = g_globalInitializer[i].func();
    }
}

void *findGlobalService(const char * name)
{
    for(int i = 0; i < g_initializerSize; i++)
    {
        if(!strcmp(g_globalInitializer[i].name, name))
            return g_globalInitializer[i].handle;
    }
    return 0;
}
