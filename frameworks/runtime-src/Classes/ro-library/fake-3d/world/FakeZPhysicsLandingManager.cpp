#include "FakeZPhysicsLandingManager.h"
#include "../component/FakeZPhysicsComponent.h"

FakeZPhysicsLandingManager::FakeZPhysicsLandingManager() {

}

FakeZPhysicsLandingManager::~FakeZPhysicsLandingManager() {
	_objMaps.clear();
}

void	FakeZPhysicsLandingManager::putLandingData(FakeZPhysicsComponent *upper, FakeZPhysicsComponent *ground, void *userdata){
	CCASSERT(_objMaps.at(upper) == nullptr, "For memory management reason, please release the previous object mannually befor inserting it.");
	auto landing = new FakeZPhysicsLanding();
	landing->_upper = upper;
	landing->_ground = ground;
	landing->_userData = userdata;
	landing->autorelease();
	_objMaps.insert(upper, landing);
}

FakeZPhysicsLanding*	FakeZPhysicsLandingManager::getLandedByUpper(FakeZPhysicsComponent *upper) {
	return _objMaps.at(upper);
}

void	FakeZPhysicsLandingManager::releaseWithUserData(void *userdata) {
	std::vector<FakeZPhysicsComponent*> eraseList;
	for (auto tmp : _objMaps) {
		if (tmp.second->getUserData() == userdata) {
			eraseList.push_back(tmp.first);
		}
	}
	for (auto tmp : eraseList) {
		_objMaps.erase(tmp);
	}
}

void	FakeZPhysicsLandingManager::releaseWithUpper(FakeZPhysicsComponent *upper) {
	_objMaps.erase(upper);
}

bool	FakeZPhysicsLandingManager::checkIfLandingBetween(FakeZPhysicsComponent *a, FakeZPhysicsComponent *b) {
	if (a == nullptr || b == nullptr) {
		return false;
	}
	auto tmp = _objMaps.at(a);
	if (tmp != nullptr && tmp->getGround() == b) {
		return true;
	}
	tmp = _objMaps.at(b);
	if (tmp != nullptr && tmp->getGround() == a) {
		return true;
	}
	return false;
}
