#include "FakeZPhysicsWorld.h"
#include "../component/FakeZPhysicsComponent.h"
#include "FakeZPhysicsLandingManager.h"
#include <cocos2d.h>

USING_NS_CC;

FakeZPhysicsComponent *FakeZPhysicsWorld::createComponent() {
	auto cmp = FakeZPhysicsComponent::create(this);
	cmp->setName(FAKE_Z_COMPONENT);
	_renderRatio = Vec2(0, 1);
	_bearedComponents.push_back(cmp);
	_pLandManager = new FakeZPhysicsLandingManager();
	return cmp;
}

void	FakeZPhysicsWorld::onDestroyComponent(FakeZPhysicsComponent *cmp) {
	//CCLOG("[%x] %s::%s, contained vector::%d", this, typeid(this).name(), __func__, _bearedComponents.size());
	_bearedComponents.erase(
		std::remove(_bearedComponents.begin(), _bearedComponents.end(), cmp), 
		_bearedComponents.end());
	_pLandManager->releaseWithUpper(cmp);
	//CCLOG("[%x] %s::%s, new contained vector::%d", this, typeid(this).name(), __func__, _bearedComponents.size());
}

void	FakeZPhysicsWorld::step(float dt) {
	_elapsed += dt;
	for (auto cmp : _bearedComponents) {
		cmp->processVelocity(dt);
	}
	if (_elapsed < _contactTick) {
		return;
	}
	//CCLOG("[%x] %s::%s, solve contact", this, typeid(this).name(), __func__);
	for (auto cmp : _bearedComponents) {
		cmp->solveContact(dt);
	}
	_elapsed = 0;
}

FakeZPhysicsWorld::FakeZPhysicsWorld(Node *baseNode) {
	CCASSERT(baseNode != nullptr, "PLEASE SPECIFY THE OWNING LAYER NODE...");
	_pBaseNode = baseNode;
	_pCListener = nullptr;
	_landingThreshold = 5;
}

FakeZPhysicsWorld::~FakeZPhysicsWorld() {
	CC_SAFE_DELETE(_pLandManager);
}
