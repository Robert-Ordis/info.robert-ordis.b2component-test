#ifndef _FAKE_Z_PHYSICS_WORLD_H
#define	_FAKE_Z_PHYSICS_WORLD_H
#include <cocos2d.h>
#include "../component/FakeZPhysicsComponent.h"
#include "FakeZPhysicsLandingManager.h"
#include "../listener/FakeZPhysicsContactListener.h"

#define	FAKE_Z_COMPONENT	"FakeZComponent"
#define	MASS_RATIO_LIMIT	0.05

class FakeZPhysicsComponent;
class FakeZPhysicsContactListener;
class FakeZPhysicsLandingManager;

/**
 *	@brief		Z軸の動きを物理チックにやりたくて作った世界クラス
 *	@author		Robert_Ordis
 *	@remarks	まだそこまで固まってるわけじゃないけど、コイツ関連はZの動きを定義するだけ。
 *	@remarks	んで、こいつを継承したクラスにいろいろ聞いてFake3DPhysicsComponentが自動で動いてくれるっていう考え。
 */
class FakeZPhysicsWorld {
public:
	///@brief	キホンとなるZ方向の重力加速度を得る
	float			getGravity()const { return _gravity; };
	///@brief	キホンとなるZ方向の重力加速度をセットする
	void			setGravity(float arg) { _gravity = arg; };
	///@brief	Z座標のレンダ方法を取得
	cocos2d::Vec2	getRenderRatio()const { return _renderRatio; };
	///@brief	Z座標のレンダ方法を設定
	void			setRenderRatio(cocos2d::Vec2 arg) { _renderRatio = arg; };
	///@brief	コンポーネントのノードへの座標反映レートを得る
	float			getRenderTick() { return _renderTick; };
	///@brief	コンポーネントのノードへの座標反映レートをセットする
	float			setRenderTick(float arg) { _renderTick = arg; };
	///@brief	衝突検出の感覚を得る
	float			getContactTick() { return _contactTick; };
	///@brief	衝突検出の間隔を定義する
	void			setContactTick(float arg) { _contactTick = arg; };
	cocos2d::Node	*getBaseNode(){ return _pBaseNode; };

	///@brief	配下のZエセ物理コンポーネントを生成する。
	FakeZPhysicsComponent *createComponent();

	///@brief	該当するコンポーネント（の位置になろう）について、最も近く、立ち位置が下の奴と高さ比を返す。
	///@remarks	何もない場合はNULLを返すぞ
	///@return	相手のコンポーネント、相手の高さを決定する比（斜面相手を想定）
	virtual	std::tuple<FakeZPhysicsComponent*, float> getNearestGround(FakeZPhysicsComponent *cmp) {
		if (_pCListener != nullptr) {
			return _pCListener->getNearestGround(cmp);
		}
		return std::tuple<FakeZPhysicsComponent*, float>(nullptr, 0);
	}

	///@brief	指定されたコンポーネントがすっ飛んだ事を知らせる。
	///@remarks	例えば、関連しているb2FrictionJointの係数をリセットするとかに使えるかもね。
	virtual	void	onFloated(FakeZPhysicsComponent *cmp) {
		if (_pCListener != nullptr) {
			_pCListener->onFloated(cmp);
		}
	};

	///@brief	指定されたコンポーネントが着地したことを知らせる。
	///@remarks	例えば、関連しているb2FrictionJointの係数を復活させるとカニ使えるかも？
	virtual	void	onLanded(FakeZPhysicsComponent *cmp, FakeZPhysicsComponent *ground, float impact) {
		if (_pCListener != nullptr) {
			_pCListener->onLanded(cmp, ground, impact);
		}
	};

	///@brief	Zコンポーネントが破棄された場合の何とやら。
	///@remarks	継承するものへ次ぐ。必ず親クラスでのこいつは呼べ！
	virtual	void	onDestroyComponent(FakeZPhysicsComponent *cmp);


	///@brief	配下のコンポーネントの速度を更新する
	///@remarks	必ず呼ぶこと！
	void	step(float dt);

	///@brief	衝突とかのイベントを見てハンドリングしてくれたり地面とのマッチングしてくれる人セットするお
	void	setContactListener(FakeZPhysicsContactListener *arg) {
		_pCListener = arg;
	};

	///@brief	着地判定の閾値を得る（衝突計算には影響しません）
	float	getLandingThreshold() { return _landingThreshold; };

	///@brief	着地判定の閾値をセットする（マイナスとかやると着地できないぞ）
	void	setLandingThreshold(float arg) { _landingThreshold = arg; };
	
	///@brief	着地系統のマネージャを取得する
	FakeZPhysicsLandingManager*	getLandingManager() { return _pLandManager; };

	///@brief	高さ計算のベースのノードを示しつつ作成するコンストラクタ
	///@remarks	とはいっても、この子をメンバにしたレイヤじゃなきゃダメだよ。
	FakeZPhysicsWorld(cocos2d::Node *pBaseNode);
	///@brief	ですとらくた。
	virtual		~FakeZPhysicsWorld();

private:
	float			_gravity;		//重力加速度
	cocos2d::Vec2	_renderRatio;	//Z座標をどんな感じで書くか
	std::vector<FakeZPhysicsComponent *>	_bearedComponents;
	float			_renderTick;	//どんな塩梅でノード座標に反映させるか。
	float			_contactTick;	//どんな塩梅でコンタクトやらせるか。
	cocos2d::Node	*_pBaseNode;
	float			_elapsed;
	float			_landingThreshold;	//着地の閾値（つーか遊び）
	
	FakeZPhysicsLandingManager	*_pLandManager;
	FakeZPhysicsContactListener	*_pCListener;
};

#endif // !_FAKE_Z_PHYSICS_WORLD_H
