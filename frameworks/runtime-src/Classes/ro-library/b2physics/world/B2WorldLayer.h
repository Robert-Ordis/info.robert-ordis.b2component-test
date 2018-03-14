#ifndef _B2_WORLD_LAYER_H
#define	_B2_WORLD_LAYER_H

#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include "../component/B2NodeComponent.h"
#include "../process/B2ProcessCallback.h"

class B2NodeComponent;
class B2ProcessCallback;
/**
 *	@file	B2WorldLayer.h	
 *	@brief	Box2Dを取り扱うためのレイヤークラス
 *	@author	Robert_Ordis
 *	@detail	まだ「これだ！」っていうのは決め手はないよ。
 */
class B2WorldLayer : public cocos2d::Layer, public b2ContactListener, public b2DestructionListener {
public:

	///@brief	cocos2dx系統によくある初期化関数
	virtual bool	init(float ptmRatio);

public:		//b2ContactListenerから継承。
	
	///@brief	衝突開始時のコールバック
	virtual void 	BeginContact(b2Contact *contact) = 0;
	///@brief	衝突終了時のコールバック
	virtual void	EndContact(b2Contact *contact) = 0;
	///@brief	衝突開始前のコールバック
	virtual void	PreSolve(b2Contact* contact, const b2Manifold* oldManifold) = 0;
	///@brief	衝突終了後のコールバック
	virtual void	PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) = 0;

	
public:		//b2DestructionListenerからの継承
	///@brief	B2Joint破棄の時のコールバック
	virtual void	SayGoodbye(b2Joint *joint);
	///@brief	B2Fixture破棄時のコールバック
	virtual void	SayGoodbye(b2Fixture *fixture);

public:		//オリジナル実装
	///@brief	Box2Dのレンジでの重力を読む
	b2Vec2			getGravityInB2Range() const;
	///@brief	Box2Dのレンジで重力を入力する
	void			setGravityInB2Range(b2Vec2 gravity);
	///@brief	Cocos2d-xレンジでの重力を読む
	cocos2d::Vec2	getGravityInCCRange() const;
	///@brief	Cocos2d-xレンジで重力を入力する
	void			setGravityInCCRange(cocos2d::Vec2 gravity);
	
	//オリジナルのうち、ちょっと怖いやつ
	///@brief	配下のコンポーネントが死んだら呼ばれるやつ。デフォルトではそのまま対応するぼでーを殺そう。
	virtual	void	onComponentBodyDestruction(B2NodeComponent *exited);

	//単純なゲッター/セッター
	///@brief	Box2D世界を参照する
	b2World		*getB2World() { return _pB2World; };
	///@brief	PTM_RATIOを取得する
	float		getPtmRatio() { return _ptmRatio; };
	
	///@brief	タスクをキューに詰め込む（派生クラスで呼び出そう）
	void		_addB2Task(B2ProcessCallback *task) {
		_b2TaskQueue.pushBack(task);
	}
	///@brief	キューしたタスクを実行する（派生クラスで呼び出そう）
	void		_execB2Task() {
		B2ProcessCallback *task = nullptr;
		while (_b2TaskQueue.size() != 0) {
			task = _b2TaskQueue.at(0);
			task->process();
			_b2TaskQueue.eraseObject(task);
		}
	}
	///@brief	キューしたタスクをすべて破棄する（派生クラスで呼び出そう）
	void		_cancelAllB2Task() {
		_b2TaskQueue.clear();
	}
	///@brief	指定のタスクを破棄する（派生クラスで呼び出せるか？）
	void		_cancelB2Task(B2ProcessCallback *task) {
		_b2TaskQueue.eraseObject(task);
	}

	//デストラクタとか
	B2WorldLayer();
	virtual		~B2WorldLayer();

protected:

private:
	b2World	*_pB2World;
	float	_ptmRatio;
	cocos2d::Vector<B2ProcessCallback*> _b2TaskQueue;
};


#endif // !_B2_WORLD_LAYER_H
