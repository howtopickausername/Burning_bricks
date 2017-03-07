#pragma once
#include <chrono>

typedef std::ratio<64, 1> FramesRate;
typedef std::chrono::duration<long long, std::ratio<1, 64> > FrameTime;

template<typename T>
void RELEASE(T & t) {
	t.Release();
}

template<typename T>
void RELEASE(T *const t) {
	if (t){
		t->Release();
	}
}