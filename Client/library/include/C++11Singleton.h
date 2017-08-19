#ifndef _SINGLETON_H_
#define _SINGLETON_H_
#include "simple_ptr.h"
#include <functional>
#if defined(_ENABLE_MULTITHREAD)
#include <mutex>
#endif

namespace purelib {
namespace gc {

    /// CLASS TEMPLATE singleton, support delay init with variadic args
    template<typename _Ty>
    class singleton
    {
    public:
        template<typename ..._Args>
        static _Ty* instance(_Args...args)
        {
            if (nullptr == singleton<_Ty>::__single__.get())
            {
#if defined(_ENABLE_MULTITHREAD)
                singleton<_Ty>::__mutex__.lock();
#endif
                if (nullptr == singleton<_Ty>::__single__.get())
                {
                    singleton<_Ty>::__single__.reset(new _Ty);
                    delay_init(args...);
                }
#if defined(_ENABLE_MULTITHREAD)
                singleton<_Ty>::__mutex__.unlock();
#endif
            }
            return singleton<_Ty>::__single__.get();
        }

        static void destroy(void)
        {
            if (singleton<_Ty>::__single__.get() != nullptr)
            {
                singleton<_Ty>::__single__.reset();
            }
        }

    private:

        template<typename _Fty, typename...Args>
        static void delay_init(const _Fty& memf, Args...args)
        { // init use specific member func with more than 1 args
            std::mem_fn(memf)(singleton<_Ty>::__single__.get(), args...);
        }

        template<typename _Fty, typename Arg>
        static void delay_init(const _Fty& memf, const Arg& arg)
        { // init use specific member func with 1 arg
            std::mem_fun(memf)(singleton<_Ty>::__single__.get(), arg);
        }

        template<typename _Fty>
        static void delay_init(const _Fty& memf)
        { // init use specific member func no args
            std::mem_fun(memf)(singleton<_Ty>::__single__.get());
        }

        static void delay_init(void)
        { // no init
        }

    private:
        static simple_ptr<_Ty> __single__;
#if defined(_ENABLE_MULTITHREAD)
        static std::mutex    __mutex__;
#endif
    };

    template<typename _Ty>
    simple_ptr<_Ty> singleton<_Ty>::__single__;
#if defined(_ENABLE_MULTITHREAD)
    template<typename _Ty>
    std::mutex    singleton<_Ty>::__mutex__;
#endif
};

};

#endif

/*
* Copyright (c) 2012-2014 by X.D. Guo  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
V3.0:2011 */