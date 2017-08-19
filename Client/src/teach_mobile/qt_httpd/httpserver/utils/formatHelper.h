#ifndef FORMATHELPER_H
#define FORMATHELPER_H

#ifdef Q_OS_WIN
#define INT64_FMT "%I64d"
#else
#define INT64_FMT "%lld"
#endif


#endif // FORMATHELPER_H
