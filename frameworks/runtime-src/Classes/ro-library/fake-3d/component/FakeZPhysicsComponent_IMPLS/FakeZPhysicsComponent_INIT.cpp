/**
 *	@file	FakeZPhysicsCoponent_INIT.cpp
 *	@brief	FakeZPhysicsCoponentクラス初期化部分寄せ集め
 *	@author	Robert_Ordis
 *
 */

#include "../FakeZPhysicsComponent.h"
#include "../../world/FakeZPhysicsWorld.h"
#include <cocos2d.h>
USING_NS_CC;

FakeZPhysicsComponent*	FakeZPhysicsComponent::create(FakeZPhysicsWorld *pWorld) {
	FakeZPhysicsComponent* pRet = new (std::nothrow) FakeZPhysicsComponent();
	if (pRet && pRet->init(pWorld)) {
		pRet->autorelease();
	}
	else {
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool	FakeZPhysicsComponent::init(FakeZPhysicsWorld *pWorld) {
	CCLOG("[%x]%s::%s world->%x", this, typeid(this).name(), __func__, pWorld);
	CCASSERT(pWorld != nullptr, "PLEASE SPECIFY THE ACTUAL FAKE_Z_PHYSICS_WORLD!!!");
	if (!Component::init()) {
		return false;
	}

	this->_pWorld = pWorld;
	this->_damping = 1.0;
	this->_gravityRatio = 1.0;
	this->_restitution = 1.0;
	this->_zVelocity = 0.0;
	this->_landing = false;
	this->_priority = 0;
	this->_globalZ = 0;
	this->_additionalTransform = Mat4();
	this->_tunnelAllowed = true;
	return true;
}

void	FakeZPhysicsComponent::onEnter() {
	//_owner->setAdditionalTransform(&_additionalTransform);
}

void	FakeZPhysicsComponent::onExit() {
	CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
	_pWorld->onDestroyComponent(this);
	_pWorld = nullptr;
	if (_owner != nullptr) {
		_owner->setAdditionalTransform(nullptr);
	}
	Component::onExit();
}

FakeZPhysicsComponent::~FakeZPhysicsComponent() {
	//よくよく考えたら特にさしたる処理もないか…
	CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
	if (_pWorld != nullptr) {
		_pWorld->onDestroyComponent(this);
	}
}

