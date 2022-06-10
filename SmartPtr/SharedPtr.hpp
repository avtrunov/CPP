template <typename T>
using deleterFunc = void(*)(T*);

template <typename T>
class ControlUnit
{
private:
	unsigned int ptrCount = 1;
	deleterFunc<T> deleter = [](T* toDelete) {delete toDelete; };
public:
	ControlUnit() noexcept {}
	ControlUnit(deleterFunc<T> customDeleter) noexcept :deleter(customDeleter) {}
	void IncrementCnt() noexcept { ptrCount++; }  
	void DecrementCnt() noexcept { ptrCount--; }
	unsigned int PtrCount() const noexcept { return ptrCount; }
	void DeleteObject(T* obj) const noexcept { deleter(obj); }
};


template <typename T>
class SharedPtr
{
private:
	T* objPtr;
	ControlUnit<T>* cUnitPtr;
public:
	SharedPtr() :objPtr(nullptr), cUnitPtr(nullptr) {}
	SharedPtr(T* ptr) : objPtr(ptr), cUnitPtr(ptr != nullptr ? new ControlUnit<T> : nullptr) { }
	SharedPtr(T* ptr, deleterFunc<T> func) : objPtr(ptr), cUnitPtr(ptr != nullptr ? new ControlUnit<T>(func) : nullptr) { }
	SharedPtr(const SharedPtr& rhs) noexcept :objPtr(rhs.objPtr), cUnitPtr(rhs.cUnitPtr)
	{ 
		cUnitPtr->IncrementCnt(); 
	}
	SharedPtr(SharedPtr&& rhs) noexcept : objPtr(rhs.objPtr), cUnitPtr(rhs.cUnitPtr)
	{
		rhs.objPtr = nullptr;
		rhs.cUnitPtr = nullptr;
	}
	SharedPtr& operator=(const SharedPtr& rhs);
	SharedPtr& operator=(SharedPtr&& rhs) noexcept;
	~SharedPtr() { this->Reset(); }
	T& operator*() { return *objPtr; }
	T* operator->() { return objPtr; }
	void Reset() noexcept;
	unsigned int Count() const noexcept { return (cUnitPtr != nullptr) ? cUnitPtr->PtrCount() : 0; }
	bool operator==(const SharedPtr& rhs) const noexcept { return objPtr == rhs.objPtr; }
	bool operator!=(const SharedPtr& rhs) const noexcept { return objPtr != rhs.objPtr; }
};

template <typename T>
void SharedPtr<T>::Reset() noexcept
{
	if (cUnitPtr != nullptr && objPtr != nullptr)
	{
		if (cUnitPtr->PtrCount() == 1)
		{
			cUnitPtr->DeleteObject(objPtr);
			delete cUnitPtr;
		}
		else
			cUnitPtr->DecrementCnt();
		objPtr = nullptr;
		cUnitPtr = nullptr;
	}
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& rhs)
{
	if (this == &rhs)
		return *this;
	this->Reset();
	objPtr = rhs.objPtr;
	cUnitPtr = rhs.cUnitPtr;
	cUnitPtr->IncrementCnt();
	return *this;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& rhs) noexcept
{
	if (this == &rhs)
		return *this;
	this->Reset();
	objPtr = rhs.objPtr;
	cUnitPtr = rhs.cUnitPtr;
	rhs.cUnitPtr = nullptr;
	rhs.objPtr = nullptr;
	return *this;
}

template <typename T>
SharedPtr<T> MakeShared(const T&& value = { 0 })
{
	return SharedPtr<T>{new T{ value }};
}

template <typename T>
SharedPtr<T> MakeShared(const T&& value, deleterFunc<T> f)
{
	return SharedPtr<T>(new T(value), f);
}

template <typename T>
SharedPtr<T> MakeShared(const T& value)
{
	return SharedPtr<T>{new T{ value }};
}

template <typename T>
SharedPtr<T> MakeShared(const T& value, deleterFunc<T> f)
{
	return SharedPtr<T>(new T(value), f);
}