/**
 *	@file	B2NodeComponent_INIT.cpp
 *	@brief	B2NodeComponentクラスの初期化系統関数
 *	@author	Robert_Ordis
 *
 */
#include "../B2NodeComponent.h"
#include "../../world/B2WorldLayer.h"
#include <cocos2d.h>
USING_NS_CC;

B2NodeComponent*	B2NodeComponent::create(B2WorldLayer* pWorldLayer, b2Body *pBody){
	B2NodeComponent* pRet = new (std::nothrow) B2NodeComponent();
	if (pRet && pRet->init(pWorldLayer, pBody)){
		pRet->autorelease();
	}
	else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool	B2NodeComponent::init(B2WorldLayer* pWorldLayer, b2Body *pBody) {
	bool condition = pBody->GetWorld() == pWorldLayer->getB2World();
	CCASSERT(condition, "B2Body must be created by the World same as parent WorldLayer!!");
	CCASSERT(pBody->GetUserData() == nullptr, 
		"Please empty the UserData of the body... Because it will be filled with THIS.");
	if (!Component::init()) {
		return false;
	}
	if (!condition) {
		return false;
	}
	
	CCLOG("[%x]%s::%s pBody->%x", this, typeid(this).name(), __func__, pBody);
	CCLOG("[%x]%s::%s owner->%x", this, typeid(this).name(), __func__, _owner);

	_pWorldLayer = pWorldLayer;
	_pBody = pBody;
	_pBody->SetUserData(this);
	_pBody->SetActive(false);
	_dependToWorldPos = false;
	_prevOwnerPos = Vec2::ZERO;
	_prevOwnerRot = 0;

	return true;
}

void	B2NodeComponent::setBody(b2Body *pBody) {
	CCASSERT(pBody != nullptr, "NEVER TRY TO SET NULL INTO BODY PARAM !!!");
	CCASSERT(pBody->GetWorld() == _pWorldLayer->getB2World(), "CANNOT SET OTHER WORLD'S BODY. IF YOU WANT OWNER TO TRANSFER TO OTHER WORLD, RE-CREATE B2COMPONENT.");
	pBody->SetTransform(_pBody->GetPosition(), _pBody->GetAngle());
	pBody->SetLinearVelocity(_pBody->GetLinearVelocity());
	pBody->SetAngularVelocity(_pBody->GetAngularVelocity());
	pBody->SetLinearDamping(_pBody->GetLinearDamping());
	pBody->SetAngularDamping(_pBody->GetAngularDamping());
	pBody->SetUserData(this);
	_pWorldLayer->onComponentBodyDestruction(this);
	_pBody = pBody;
}

void	B2NodeComponent::onExit() {
	CCLOG("[%x]%s::%s ", this, typeid(this).name(), __func__);
	_pWorldLayer->onComponentBodyDestruction(this);
	_pBody = nullptr;
	Component::onExit();
}

B2NodeComponent::~B2NodeComponent() {
	CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
	if (_pBody != nullptr) {
		_pWorldLayer->onComponentBodyDestruction(this);
	}
}
