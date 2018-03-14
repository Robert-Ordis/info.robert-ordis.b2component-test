#include "B2WorldLayer.h"
#include "../utils/B2RangeConverter.h"
#include <cocos2d.h>
#include <Box2D/Box2D.h>


USING_NS_CC;

B2WorldLayer::B2WorldLayer() 
	: _pB2World(nullptr)
	, _b2TaskQueue(Vector<B2ProcessCallback*>())
{}

bool	B2WorldLayer::init(float ptm_ratio) {
	CCASSERT(ptm_ratio > 0, "PTM_RATIO MUST BE GRATER THAN ZERO.");
	if (!Layer::init()) {
		return false;
	}
	CCLOG("[%x]%s::%s initiated as Layer", this, typeid(this).name(), __func__);

	auto gravity = b2Vec2(0, 0);
	_pB2World = new b2World(gravity);
	CCLOG("[%x]%s::%s initiated box2dWorld[%x]", this, typeid(this).name(), __func__, _pB2World);
	
	_pB2World->SetContactListener(this);
	_pB2World->SetDestructionListener(this);
	_ptmRatio = ptm_ratio;
	return true;
}

b2Vec2	B2WorldLayer::getGravityInB2Range() const {
	return _pB2World->GetGravity();
}

void	B2WorldLayer::setGravityInB2Range(b2Vec2 gravity) {
	_pB2World->SetGravity(gravity);
}

Vec2	B2WorldLayer::getGravityInCCRange() const {
	Vec2 retVal = Vec2(0, 0);
	auto b2Gravity = _pB2World->GetGravity();
	return B2RangeConverter::convertVec2ToCC(b2Gravity, _ptmRatio);
}

void	B2WorldLayer::setGravityInCCRange(cocos2d::Vec2 gravity) {
	_pB2World->SetGravity(B2RangeConverter::convertVec2ToB2(gravity, _ptmRatio));
}

void	B2WorldLayer::SayGoodbye(b2Joint *joint) {
	CCLOG("[%x]%s::%s good bye (joint) %x", this, typeid(this).name(), __func__, joint);
}

void	B2WorldLayer::SayGoodbye(b2Fixture *fixture) {
	CCLOG("[%x]%s::%s good bye (fixture) %x", this, typeid(this).name(), __func__, fixture);
}

void	B2WorldLayer::onComponentBodyDestruction(B2NodeComponent *exited) {
	CCLOG("[%x]%s::%s recved destruction of %x", this, typeid(this).name(), __func__, exited);
	_pB2World->DestroyBody(exited->getBody());
}

B2WorldLayer::~B2WorldLayer(){
	CC_SAFE_DELETE(_pB2World);
}