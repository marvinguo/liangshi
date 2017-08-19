#pragma once

#include "Qt.h"

template <typename T>
class QtSingleton
{
protected:
	explicit QtSingleton(T* instance);
	~QtSingleton();

public:
	static T* Instance();

private:
	Q_DISABLE_COPY(QtSingleton);

private:
	static QAtomicPointer<T> instance;
};

template <typename T>
QtSingleton<T>::QtSingleton(T* instance)
{
	bool ok = this->instance.testAndSetRelaxed(0, instance);
	Q_ASSERT_X(ok, Q_FUNC_INFO, "Only one instance of this class is permitted");
}

template <typename T>
QtSingleton<T>::~QtSingleton()
{
	T* old_instance = instance.fetchAndStoreRelaxed(0);
	Q_ASSERT_X(0 != old_instance, Q_FUNC_INFO, "The singleton instance is invalid");
}

template <typename T>
T* QtSingleton<T>::Instance()
{
	//gcc 4.8 编译不过去了
	//Q_ASSERT_X(instance != NULL, Q_FUNC_INFO, "The singleton has not yet been created");
	return instance;
}

template <typename T>
QAtomicPointer<T> QtSingleton<T>::instance;

