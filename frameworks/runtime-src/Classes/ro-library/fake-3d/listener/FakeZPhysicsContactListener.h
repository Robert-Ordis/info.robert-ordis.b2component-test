#ifndef _FAKE_Z_PHYSICS_CONTACT_LISTENER_H
#define	_FAKE_Z_PHYSICS_CONTACT_LISTENER_H
#include "../world/FakeZPhysicsWorld.h"
#include "../component/FakeZPhysicsComponent.h"

class FakeZPhysicsWorld;
class FakeZPhysicsComponent;

/**
 *	@brief		物理世界と世界を持たせるメンバを別々にするためのりすなー
 *	@author		Robert_Ordis
 *	@remarks	結構迷走してるよねー
 */
class FakeZPhysicsContactListener {
public:
	///@brief	該当するコンポーネント（の位置になろう）について、最も近く、立ち位置が下の奴と高さ比を返す。
	///@remarks	何もない場合はNULLを返すぞ
	///@return	相手のコンポーネント、相手の高さを決定する比（斜面相手を想定）
	virtual	std::tuple<FakeZPhysicsComponent*, float> getNearestGround(FakeZPhysicsComponent *cmp) = 0;

	///@brief	指定されたコンポーネントがすっ飛んだ事を知らせる。
	///@remarks	例えば、関連しているb2FrictionJointの係数をリセットするとかに使えるかもね。
	virtual	void	onFloated(FakeZPhysicsComponent *cmp) = 0;

	///@brief	指定されたコンポーネントが着地したことを知らせる。
	///@remarks	例えば、関連しているb2FrictionJointの係数を復活させるとカニ使えるかも？
	virtual	void	onLanded(FakeZPhysicsComponent *cmp, FakeZPhysicsComponent *ground, float impact) = 0;


};

#endif // !_FAKE_Z_PHYSICS_CONTACT_LISTENER_H
