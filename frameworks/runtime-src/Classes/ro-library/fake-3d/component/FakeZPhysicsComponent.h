#ifndef _FAKE_Z_PHYSICS_COMPONENT_H
#define	_FAKE_Z_PHYSICS_COMPONENT_H

#include <cocos2d.h>
#include "../world/FakeZPhysicsWorld.h"
class FakeZPhysicsWorld;
/**
 *	@brief		Zポジ処理を物理っぽくふるまわせるためのコンポーネント
 *	@author		Robert_Ordis
 *	@remarks	とりあえずナニカ適当な２Dあたり判定（もしくは物理ライブラリ）に
 *	@remarks	寄生して生きることを前提としているコンポーネントおよびワールド。
 *	@remarks	高校物理の弾性衝突をZ軸でやるってだけのやつよ。
 *	@remarks	相変わらずaddChildされたノードには対応できないのであしからず。
 *
 */
class FakeZPhysicsComponent : public cocos2d::Component {
public:
	
	///@brief	初期化。
	virtual	bool	init(FakeZPhysicsWorld* pWorld);

	///@brief	アプデ。これが肝となってオーナーのZ座標を操作する。
	virtual	void	update(float dt);

	///@brief	エンター
	virtual void	onEnter();

	///@brief	破棄時の。
	virtual	void	onExit();

	///@brief	デストラクタ
	virtual			~FakeZPhysicsComponent();

	///@brief	ローカルな高さがmin -> maxheightのうちの何割に当たるか取る
	float			convertToSlopeRatio(float localHeight);
	
public:	//単純なゲッター/セッターども。めんどいのでヘッダ内記述
	///@brief	重力作用係数を取得
	float	getGravityRatio() { return _gravityRatio; };
	
	///@brief	重力作用係数を設定
	void	setGravityRatio(float arg) { _gravityRatio = arg; };

	///@brief	減衰率を取得
	float	getDamping() { return _damping; };

	///@brief	減衰率を設定
	float	setDamping(float arg) { _damping = arg; };

	///@brief	反発率を取得
	float	getRestitution() { return _restitution; };

	///@brief	反発率を設定
	void	setRestitution(float arg) { _restitution = arg; };

	///@brief	Z速度を取得
	float	getZVelocity() const { return _zVelocity; };

	///@brief	Z速度を直接設定
	void	setZVelocity(float arg) { _zVelocity = arg; };

	///@brief	親のエセ物理世界を取得
	FakeZPhysicsWorld	*getWorld() { return _pWorld; };

	///@brief	最小身長を設定
	void	setMinStature(float arg) { _minStature = arg; };

	///@brief	最大身長を設定
	void	setMaxStature(float arg) { _maxStature = arg; };

	///@brief	比率から慎重を求める
	float	getStature(float rate) {
		return _minStature + (_maxStature - _minStature) * rate;
	}

	///@brief	一時的に接地ステータスを下げる
	void	downLandingFlag() { _landing = false; };

	///@brief	優先度取得
	int		getPriority() { return _priority; };

	///@brief	優先度セット
	void	setPriority(int arg) { _priority = arg; };

	///@brief	質量取得
	float	getMass() { return _mass; };

	///@brief	質量セット
	void	setMass(float arg) { _mass = arg; };

	///@brief	描画補正を得る
	float	getRenderHeight()const { return _renderHeight; };

	///@brief	描画補正をセットする
	void	setRenderHeight(float arg) { _renderHeight = arg; };

	///@brief	とりあえず、世界基準とかでのZ座標をたたき出す。
	float	getWorldZPosition() const;

	///@brief	速度を使った判定のすり抜けを許すかどうか
	bool	isTunnelingAllowed()const { return _tunnelAllowed; };

	///@brief	速度を使った判定のすり抜けの可否を設定する
	void	setTunnelingAllowed(bool arg) { _tunnelAllowed = arg; };

private:	//各プロパティ
	float				_gravityRatio;		//世界の重力をどれだけ真に受けるか（水中ならむしろマイナスってる感じ。）
	float				_zVelocity;			//Z速度
	float				_damping;			//減速度。基本は１として自身の加速も減速もしない。1未満でその割合で減速。
	float				_restitution;		//反発率。基本は1で100%反発させる。
	FakeZPhysicsWorld	*_pWorld;			//親世界。この方にいろいろと問い合わせて次の動作を決定する。
	bool				_landing;			//trueで接地状態。
	float				_minStature;		//最小の身長
	float				_maxStature;		//最大の身長
	float				_mass;				//質量
	float				_globalZ;			//世界でのZポジ
	float				_prevNodeZ;			//前回レンダ時のノードのZポジ
	int					_priority;
	float				_renderHeight;		//描画補正つける高さ
	bool				_tunnelAllowed;		//あたり判定のすり抜けを許すかどうか
	float				_zDeltaCorrect;		//計算で生じた位置補正
	float				_lastFloor;			//最後に踏んだ床の高さ

private:	//FakeZPhysicsWorldから呼び出す奴。
	friend class FakeZPhysicsWorld;
	///@brief	コンポーネントのインスタンス生成。ワールドを指定する（必須）
	static	FakeZPhysicsComponent*	create(FakeZPhysicsWorld* pWorld);
	void	solveContact(float dt);
	void	processVelocity(float dt);
	float	_elapsed;

private:	//完全プライベート。
	cocos2d::Mat4	_additionalTransform;
};

#endif // !_FAKE_Z_PHYSICS_COMPONENT_H
