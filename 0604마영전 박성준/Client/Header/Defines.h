#ifndef Defines_h__
#define Defines_h__



#include "Typedef.h"
#include "Macro.h"
#include "Function.h"
#include "Enum.h"

extern	HWND		g_hWnd;
extern  HINSTANCE	g_hInst;		// 인스턴스, dinput에서 사용

using namespace std;

#endif // Defines_h__

// OOP에서 객체는 클래스와 인스턴스를 포함한 개념이다

// oop특성 : 추상화 -> 클래스의 4대 속성

// 객관적 사실을 주관적으로 표현하는 것 (예) 공기가 무겁다, 꽃은 아름답다

// 현실에 존재하는 어떤 객체(사물)를 프로그램으로 표현하는 과정

// 데이터 + 기능 => 클래스(사용자 정의 자료형) 

// c : 구조체, 열거체, 공용체
// c++ : c + 클래스

// 클래스라는 자료형을 통해서 stack, heap 할당-> 메모리에 어떤 것들을 배치해 놓고 사용할 준비