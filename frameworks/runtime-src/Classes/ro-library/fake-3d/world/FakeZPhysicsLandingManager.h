#ifndef _FAKE_Z_PHYSICS_LANDING_MANAGER_H
#define	_FAKE_Z_PHYSICS_LANDING_MANAGER_H

#include <cocos2d.h>
#include "../component/FakeZPhysicsComponent.h"

class FakeZPhysicsLandingManager;
class FakeZPhysicsLanding;
class FakeZPhysicsComponent;

/**
 *	@brief		Z物理世界の接触状況を示す奴。
 *	@author		Robert_Ordis
 *	@remarks	とか何とか言って、こいつは表にはほとんど出さねーぜ！
 */
class FakeZPhysicsLanding : public cocos2d::Ref {
public:
	FakeZPhysicsComponent*	getUpper() { return _upper; };
	FakeZPhysicsComponent*	getGround() { return _ground; };
	void*					getUserData() { return _userData; };
private:
	FakeZPhysicsComponent*	_upper;
	FakeZPhysicsComponent*	_ground;
	void*					_userData;
private:
	friend class FakeZPhysicsLandingManager;
	FakeZPhysicsLanding() {};
	virtual ~FakeZPhysicsLanding() {};

};


/**
 *	@brief		Z物理世界における接触状況を管理したいクラス。
 *	@author		Robert_Ordis
 *	@remarks	できればなんですが、オブジェクト指向的にはシングルトンにしないほうがいいのかもね。
 */
class FakeZPhysicsLandingManager {
public:
	///@brief	新しい着地データを作成する。
	void					putLandingData(FakeZPhysicsComponent *upper, FakeZPhysicsComponent *ground, void *userdata);
	///@brief	指定したコンポーネント（＝ノード）の着地した判定だした情報を引っ張り出す
	FakeZPhysicsLanding*	getLandedByUpper(FakeZPhysicsComponent *upper);
	///@brief	ユーザーデータを指定して削除を行う
	void					releaseWithUserData(void *userdata);
	///@brief	指定したコンポーネントについて解放を行う。
	void					releaseWithUpper(FakeZPhysicsComponent *upper);
	///@brief	指定した２者が接地関係にあるかどうか
	bool					checkIfLandingBetween(FakeZPhysicsComponent *a, FakeZPhysicsComponent *b);

public:
	FakeZPhysicsLandingManager();
	virtual ~FakeZPhysicsLandingManager();

private:
	cocos2d::Map<FakeZPhysicsComponent*, FakeZPhysicsLanding* > _objMaps;
};


#endif // !_FAKE_Z_PHYSICS_LANDING_MANAGER_H
