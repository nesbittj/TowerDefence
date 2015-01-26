#include "cQueue.h"

template <class _type> cQueue<_type>::cQueue(int _MAX_SIZE)
{
	mDate = new _type[mMaxSize + 1];
	mNext = 0;
	mSize = 0;
}

template <class _type> cQueue<_type>::cQueue(const cQueue<_type> &_Queue) :
mMaxSize(_Queue.mMaxSize)
{
	mNext = _Queue.mNext;
	mSize = _Queue.mSize;

	mData = new _type[mMaxSize + 1];
	for(int i = 0; i < mMaxSize; i++)
		mData[i] = _Queue.mData[i];
}
											
template <class _type> cQueue<_type>::~cQueue()
{
	delete[] mData;
}

template <class _type> void cQueue<_type>::Enqueue(const _type _value)
{
	if(mSize >= mMaxSize) return;

	mData[mNext++] = _value;
	++mSize;

	if(mNext > mMaxSize)
		mNext -= (mMaxSize + 1);
}

template <class _type> _type cQueue<_type>::Dequeue()
{
	if(mSize <= 0) return mData[0];

	_type l_result = mData[mNext--];
	--mSize;

	if(mNext <= 0)
		mNext += (mMaxSize + 1);

	return l_result;
}

template <class _type> int cQueue<_type>::Contains(const _type _value)
{
	int result = -1;
	for(int i = 0; i < mMaxsize; i++)
	{
		if(mData[i] == _value) result = mData;
	}
	return result;
}

template <class _type> int cQueue<_type>::GetSize() { return mSize; }
template <class _type> void cQueue<_type>::SetNextZero() { mNext = 0; }