/**
 *	@file	FakeZPhysicsCoponent_UPDATE.cpp
 *	@brief	FakeZPhysicsCoponentクラスupdate部分寄せ集め
 *	@author	Robert_Ordis
 *
 */

#include "../FakeZPhysicsComponent.h"
#include "../../world/FakeZPhysicsWorld.h"
#include <cocos2d.h>
#define	FLOAT_ISINCALCULABLE(val) (isnan(val) || isinf(val))
USING_NS_CC;

float	FakeZPhysicsComponent::getWorldZPosition() const {
	return _globalZ;
}

static inline void correctMass(float *myMass, float *grMass) {
	if (*myMass == 0.0f && *grMass == 0.0f) {
		*myMass = 1;
		*grMass = 1;
	}
	else if (FLOAT_ISINCALCULABLE(*myMass) && FLOAT_ISINCALCULABLE(*grMass)) {
		*myMass = 0;
		*grMass = 0;
	}
	else if (FLOAT_ISINCALCULABLE(*myMass)) {
		*myMass = 100;
		*grMass = 0;
	}
	else if (FLOAT_ISINCALCULABLE(*grMass)) {
		*grMass = 100;
		*myMass = 0;
	}
}

void	FakeZPhysicsComponent::solveContact(float dt) {
	
	FakeZPhysicsComponent *ground;
	float	heightRatio;
	//そのあと、世界にあることないこと問い合わせる。
	std::tie(ground, heightRatio) = _pWorld->getNearestGround(this);
	//CCLOG("[%x]%s::%s, gotten:%x ratio:%f", this, typeid(this).name(), __func__, ground, heightRatio);
	//自分の下に地面がいるってわかったのなら。しかるべき処置を行う。
	if (ground != nullptr && ground != this) {
		//newPosにおける相手のgloPos + getStature(heightRatio)との衝突/離別の検証
		//CCLOG("[%x]%s::%s, START WITH GROUND", this, typeid(this).name(), __func__);
		//ただしそれは自分の今の速度が相手（地面）の速度を下回っていたならの話だ。
		//さらに言えば、お互いが接触したとわかった場合の事だ。
		
		float grHeight = ground->getStature(heightRatio) + ground->getWorldZPosition();
		float myHeight = this->getWorldZPosition();

		if (!_tunnelAllowed || !ground->_tunnelAllowed) {
			//grHeight += ground->_zVelocity * dt;
			myHeight += _zVelocity * dt;
		}

		//CCLOG("[%x]%s::%s, myHeight:%f, grHeight:%f", this, typeid(this).name(), __func__, myHeight, grHeight);

		//新たに着地するときは判定は厳密に。
		//そこからの継続着地判定は遊びをもって。
		bool newLandingState = (grHeight + (_landing ? _pWorld->getLandingThreshold() : 0)) >= myHeight;
		float relV = ground->_zVelocity - _zVelocity;

		if (_landing != newLandingState) {
			if (newLandingState) {
				//_globalZ = grHeight;
				_pWorld->onLanded(this, ground, relV);
			}
			else {
				_pWorld->onFloated(this);
			}
		}
		
		_lastFloor = grHeight;
		if(grHeight >= myHeight && relV > 0){
			//比率計算用にいろいろ用意する。
			float myMass = _mass;
			float grMass = ground->_mass;
			correctMass(&myMass, &grMass);
			float massTotal = myMass + grMass;
			float massTotalInv;
			if (massTotal != 0) {
				massTotalInv = 1 / massTotal;
				if (_mass * massTotalInv < MASS_RATIO_LIMIT || _mass * massTotalInv < MASS_RATIO_LIMIT) {
					myMass = _mass < ground->_mass ? 0 : massTotal;
					grMass = massTotal - myMass;
				}
			}
			else {
				//馬鹿な事しやがったときのための遊び心なv
				massTotalInv = 100000;
			}

			//最終的な反発
			float boundRatio = 0;
			
			if (!(_landing && newLandingState)) {
				//もともと地面につき続けているなら弾性はなしとしてみるがそれ以外は
				boundRatio = _restitution * ground->_restitution;
			}

			//自分用の最終的に加わる速度の計算
			float muTotal = _zVelocity * myMass + ground->_zVelocity * grMass;
			
			_zVelocity = (relV * grMass * boundRatio + muTotal) * massTotalInv;
			ground->_zVelocity = (-relV * myMass * boundRatio + muTotal) * massTotalInv;
		}

		_landing = newLandingState;
	}
	else {
		bool newLandingState = false;
		_lastFloor = -INFINITY;
		if (_landing != newLandingState) {
			_pWorld->onFloated(this);
		}
	}
}

//Z速度を進める。衝突処理はこの後。
void	FakeZPhysicsComponent::processVelocity(float dt) {
	//まず前回最終的に算出したZ速度で自分の立ち位置をハッキリさせておく。
	
	_globalZ += _zVelocity * dt;
	_zDeltaCorrect = 0;
	if (!FLOAT_ISINCALCULABLE(_lastFloor) && _globalZ < _lastFloor) {
		_globalZ = _lastFloor;
	}
	
	//次に、世界の重力加速度、自身の影響度をもとに速度を変えておく。
	_zVelocity += _pWorld->getGravity() * _gravityRatio * dt;
	if (_damping != 1.0) {
		_zVelocity *= _damping;
	}
	//CCLOG("[%x]%s::%s PROCEEDED", this, typeid(this).name(), __func__);
	//CCLOG("[%x]%s::%s zPos->%f", this, typeid(this).name(), __func__, _globalZ);
	//CCLOG("[%x]%s::%s zVel->%f", this, typeid(this).name(), __func__, _zVelocity);
}

void	FakeZPhysicsComponent::update(float dt) {
	if (_owner == nullptr || _owner->getParent() == nullptr) {
		return;
	}
	_elapsed += dt;
	if (_elapsed < _pWorld->getRenderTick()) {
		return;
	}
	//もしノードのZポジが改ざんされているようであれば
	//物理もそれに応じて改変する。
	float parentGZ = _pWorld->getBaseNode()->getPositionZ();
	for (auto n = _owner->getParent(); n != nullptr && n != _pWorld->getBaseNode(); n = n->getParent()) {
		parentGZ += n->getPositionZ();
	}
	if (_prevNodeZ != _owner->getPositionZ()) {
		//ノード位置の改ざん検出
		_globalZ = parentGZ + _owner->getPositionZ();
	}
	else {
		//特に改ざんされていないので物理に従わせる。
		_owner->setPositionZ(_globalZ - parentGZ);
	}
	_prevNodeZ = _owner->getPositionZ();
	
	Vec2 renderRatio = _pWorld->getRenderRatio();
	float tx = renderRatio.x;
	float ty = renderRatio.y;
	
#if 0
	float abs = sqrtf(tx*tx + ty*ty);
	float sint = ty / abs;
	float cost = tx / abs;
	abs *= (_prevNodeZ + _rederHeight);
	float o_an = CC_DEGREES_TO_RADIANS(_owner->getRotation());
	float sino = sinf(o_an);
	float coso = cosf(o_an);
	tx = abs * (cost*coso - sint*sino);
	ty = abs * (sint * coso - cost * sino);
#else
	float abs = sqrtf(tx*tx + ty*ty) * (_prevNodeZ + _renderHeight)/ _owner->getScale();

	float ta = atan2f(ty, tx);

	tx = abs * cosf(CC_DEGREES_TO_RADIANS(_owner->getRotation()) + ta);
	ty = abs * sinf(CC_DEGREES_TO_RADIANS(_owner->getRotation()) + ta);
#endif
	_additionalTransform = Mat4();
	_additionalTransform.translate(tx, ty, 0);
	_owner->setAdditionalTransform(&_additionalTransform);

	int zOrder = (int)(_owner->getPositionZ());
	if (renderRatio.x != 0.0f) {
		//zOrder -= (int)(renderRatio.x * _owner->getPositionX());
	}
	if (renderRatio.y != 0.0f) {
		//zOrder -= (int)(renderRatio.y * _owner->getPositionY());
	}

	_owner->setZOrder(zOrder);
	

	_elapsed = 0;
}
