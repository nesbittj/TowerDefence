#pragma once

template <class _type> class cQueue
{
protected:
	_type* mData;
	int mNext;
	int mSize;
	const int mMaxSize;

public:
	cQueue(int _MAX_SIZE = 100);
	cQueue(const cQueue<_type> &_Queue);
	~cQueue();

	void Enqueue(const _type _value);
	_type Dequeue();
	int Contains(const _type _value);
	inline int GetSize();
	inline void SetNextZero();
};