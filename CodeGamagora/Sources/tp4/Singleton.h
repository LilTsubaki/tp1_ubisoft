//**********************************************************************************************************************
#pragma once

//**********************************************************************************************************************
template <typename T>
class Singleton
{
public:

	static T& GetInstance()
	{
		return (static_cast<T &>(*_singleton));
	}

protected:

	Singleton()
	{
		_singleton = static_cast<T*>(this);
	}

	~Singleton()
	{
		_singleton = nullptr;
	}

protected:
	// singleton
	static T *_singleton;
};


template <typename T>
T *Singleton<T>::_singleton = nullptr;
