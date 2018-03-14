/**
 *	@file	B2NodeComponent_UPDATE.cpp
 *	@brief	updateやそれに伴う処理を書いたやつ
 *	@author	Robert_Ordis
 */
#include "../B2NodeComponent.h"
#include "../../utils/B2RangeConverter.h"
#include <cocos2d.h>

USING_NS_CC;

void	B2NodeComponent::update(float dt) {
	//オーナーノードの現状のレイヤーポジションと回転を読み込む
	if (_owner == nullptr || _owner->getParent() == nullptr) {
		//とはいうものの、破棄されたりとかしてオーナーいないならやめる。
		return;
	}
	if (!_effectToNode) {
		//それか、オーナーいても物理との同期をやめるつもりならやめる。
		return;
	}
	Vec2	currOwnerPos = _owner->getPosition();
	float	currOwnerRot = _owner->getRotation();

	if (_dependToWorldPos) {
		//ノード自身のグローバル座標系をもとに計算する、というのなら。って奴。
		//ただし、現在非対応。
		return;
	}

	if(!_pBody->IsActive()){
		_pBody->SetActive(true);
	}
	
	//CCLOG("[%x]%s::%s prev pos->(%f, %f)", this, typeid(this).name(), __func__, _prevOwnerPos.x, _prevOwnerPos.y);
	//CCLOG("[%x]%s::%s curr pos->(%f, %f)", this, typeid(this).name(), __func__, currOwnerPos.x, currOwnerPos.y);
	auto bvel = _pBody->GetLinearVelocity();
	//CCLOG("[%x]%s::%s body->%x", this, typeid(this).name(), __func__, _pBody);
	//CCLOG("[%x]%s::%s linear vel:(%f, %f)", this, typeid(this).name(), __func__, bvel.x, bvel.y);

	if (_prevOwnerPos != currOwnerPos) {
		//手動でポジションいじられた形跡がある。
		//CCLOG("[%x]%s::%s pos->reset", this, typeid(this).name(), __func__);
		_updatePhysicsPosToNode();
	}
	else {
		//特にポジションいじられてないので物理に任せる
		//CCLOG("[%x]%s::%s pos->update", this, typeid(this).name(), __func__);
		_updateNodePosToPhysics();
	}

	if (_prevOwnerRot != currOwnerRot) {
		//手動で角度いじられた形跡あり。
		//CCLOG("[%x]%s::%s rot->reset", this, typeid(this).name(), __func__);
		_updatePhysicsRotToNode();
	}
	else {
		//特に角度をいじっていないお。
		//CCLOG("[%x]%s::%s rot->update", this, typeid(this).name(), __func__);
		_updateNodeRotToPhysics();
	}

	//TODO: _dependToWorldPos依存でこれからの処理も変わるよ。
	_prevOwnerPos = _owner->getPosition();
	_prevOwnerRot = _owner->getRotation();
	//CCLOG("[%x]%s::%s new pos->(%f, %f)", this, typeid(this).name(), __func__, _prevOwnerPos.x, _prevOwnerPos.y);
}

inline void	B2NodeComponent::_updatePhysicsPosToNode() {
	//setPosition系で手動でポジションをいじられた場合。
	//物理ポジションをそれに従って変更する。

	//ノード自身のローカルポジションをワールドポジションに変換する。
	auto physicsPos = _owner->getPosition();
	if (_owner->getParent() != _pWorldLayer) {
		//直近の親ノードが世界じゃなかった場合。
		//世界座標取ってそれを物理系に変換する。
		physicsPos = _owner->getParent()->convertToWorldSpace(physicsPos);
		_pWorldLayer->convertToWorldSpace(physicsPos);
	}

	auto b2PhysicsPos = B2RangeConverter::convertVec2ToB2(physicsPos, _pWorldLayer->getPtmRatio());
	_pBody->SetTransform(b2PhysicsPos, _pBody->GetAngle());
}

inline void	B2NodeComponent::_updateNodePosToPhysics() {
	//手動でのポジションいじりがなかったので普通にポジションを物理に従って更新する

	//まずは物理ぼでーの位置（＝B2WorldLayer上の位置）を取得。
	auto b2PhysicsPos = _pBody->GetPosition();

	auto nextPos = B2RangeConverter::convertVec2ToCC(b2PhysicsPos, _pWorldLayer->getPtmRatio());
	if (_owner->getParent() == _pWorldLayer) {
		//直接世界の下にいるのなら話は早い。
		_owner->setPosition(nextPos);
		return;
	}
	//そうじゃあないのなら…。
	//次に、それをせっせとノードのローカル位置にまで変換する
	//手順はワールドに落とし込んでからってアレ
	nextPos = _pWorldLayer->convertToWorldSpace(nextPos);
	nextPos = _owner->getParent()->convertToNodeSpace(nextPos);
	_owner->setPosition(nextPos);
}

inline void	B2NodeComponent::_updatePhysicsRotToNode() {
	//手動で角度をいじられたので物理をそれに追従させる。
	float degree = _owner->getRotation();

	//角度はgetRotationで得られるのがやっぱ相対角度だった模様。
	//なので、ワールドが親でない場合はワールドに何とかして追いついて角度を探し出してみよう。
	if (_owner->getParent() == _pWorldLayer) {
		//ワールドが親だったらこれまた話は早い。
		//注意：ワールド自身の回転は見た目に影響を及ぼすため、禁止の方向で。ふさがないけど。
		//物理の回転にそのままノードの回転を突っ込む。
		_pBody->SetTransform(_pBody->GetPosition(), -CC_DEGREES_TO_RADIANS(degree));
		return;
	}

	//そうじゃない場合はワールドまで追っかけて何とかしてみる。
	for (auto node = _owner->getParent(); node != nullptr; node = node->getParent()) {
		degree += node->getRotation();
		if (node == _pWorldLayer) {
			break;
		}
	}
	_pBody->SetTransform(_pBody->GetPosition(), -CC_DEGREES_TO_RADIANS(degree));
}

inline void	B2NodeComponent::_updateNodeRotToPhysics() {
	//角度をいじられてはいないのでノード角度を物理に追従させる。
	float radian = _pBody->GetAngle();

	if (_owner->getParent() == _pWorldLayer){
		//物理世界に直接乗ってるノードなら別におっけー。
		_owner->setRotation(-CC_RADIANS_TO_DEGREES(radian));
		return;
	}

	//そうじゃない場合は物理界の角度から（ルート|世界～直近の親）までの角度を引いたものを使う。
	float degree = 0;
	for (auto node = _owner->getParent(); node != nullptr; node = node->getParent()) {
		degree += node->getRotation();
		if (node == _pWorldLayer) {
			break;
		}
	}
	_owner->setRotation(-CC_RADIANS_TO_DEGREES(radian) - degree);
}