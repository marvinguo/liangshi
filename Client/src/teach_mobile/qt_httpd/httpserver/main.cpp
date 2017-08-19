#include <QtCore/QCoreApplication>
#include "httpserver.h"
#include "utils/asyncLog.h"
#include "globalInit.h"
#include <signal.h>

static void signal_handler(int sig_num)
{
    switch(sig_num)
    {
#ifndef WIN32
    case SIGQUIT:
    case SIGSTOP:
    case SIGHUP:
#endif
    case SIGTERM:
    case SIGABRT:
        fprintf(stdout, "exit by signal %d\n", sig_num);
        fflush(stdout);
        exit(2);
        break;
    case SIGFPE:
    case SIGSEGV:
    case SIGILL:
        fprintf(stdout, "exit by error %d\n", sig_num);
        fflush(stdout);
        exit(1);
        break;
    }
}

static void register_signal()
{
#ifndef WIN32
    struct sigaction sigact;
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;

    sigaction(SIGHUP, &sigact, NULL);
    sigaction(SIGQUIT, &sigact, NULL);
    sigaction(SIGINT, &sigact, NULL);
    sigaction(SIGTERM, &sigact, NULL);
    sigaction(SIGABRT, &sigact, NULL);
    sigaction(SIGFPE, &sigact, NULL);
    sigaction(SIGSEGV, &sigact, NULL);
    sigaction(SIGILL, &sigact, NULL);

    /* ignore SIGPIPE, this signal will be triggered when remote close socket abnormally */
    sigact.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sigact, NULL);
#else
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGILL, signal_handler);
#endif
}

int main(int argc, char *argv[])
{
    register_signal();
    QCoreApplication a(argc, argv);

    HttpServerConfig & config = HttpServerConfig::instance();
    config.loadConfig("httpserver.conf");

    globalInitialize();

    startServer();

    return a.exec();
}
