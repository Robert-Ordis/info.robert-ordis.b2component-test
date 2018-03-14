#include <cocos2d.h>
#include "../component/B2NodeComponent.h"
#include "B2ProcessCallback.h"

USING_NS_CC;

B2ProcessCallback::B2ProcessCallback() {

}

B2ProcessCallback* B2ProcessCallback::create(B2NodeComponent *sprA, B2NodeComponent *sprB){
	B2ProcessCallback* pRet = new (std::nothrow) B2ProcessCallback();
	if (pRet && pRet->init(sprA, sprB))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool B2ProcessCallback::init(B2NodeComponent *objA, B2NodeComponent *objB) {
	CCASSERT(objA != nullptr && objB != nullptr, 
		"Please set both actually ref...PLEASE");
	
	_objA = objA;
	_objB = objB;
	_function = [=](B2NodeComponent *objA, B2NodeComponent *objB) {
		CCLOG("default lambda expression of a[%x] and b[%x]", objA, objB);
	};
	return true;
}

void B2ProcessCallback::setFunction(B2ProcessCallbackFunc func){
	_function = func;
}

void B2ProcessCallback::process() {
	_function(_objA, _objB);
}

B2ProcessCallback::~B2ProcessCallback() {
	_objA->release();
	_objB->release();
}
