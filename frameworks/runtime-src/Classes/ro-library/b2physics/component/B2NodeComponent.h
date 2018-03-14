#ifndef _B2_NODE_COMPONENT_H
#define	_B2_NODE_COMPONENT_H
#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include "../world/B2WorldLayer.h"

class B2WorldLayer;

///TODO: 別の物理世界へ移籍するとき、プロパティ丸コピできる方法が欲しい。
/**
 *	@file		B2NodeComponent.h
 *	@brief		Box2Dによる物理処理をコンポーネントに落とし込む試み。
 *	@author		Robert_Ordis
 *	@remarks	もしかすると、ここにまた衝突時のコールバックとかつけるのかもしれない。
 *	@remarks	今現在、この子はaddChildされたノードには対応できないよ。
 *	@remarks	多関節モノをやりたいのならJoint使ってやったほうがいいのかもね。
 */
class B2NodeComponent : public cocos2d::Component {
public:
	///@brief	コンポーネントのインスタンス生成。ワールドレイヤと「そこから作った」ボデーを指定する。
	static	B2NodeComponent*	create(B2WorldLayer* pWorldLayer, b2Body *pBody);

	///@brief	初期化
	virtual	bool				init(B2WorldLayer* pWorldLayer, b2Body *pBody);

	///@brief	アプデ。こいつを追従させる。
	virtual	void				update(float dt);

	///@brief	破棄時の奴。
	virtual void				onExit();

	//単純なゲッター/セッター
	///@brief	ぼでーを見る。
	b2Body		*getBody(){ return _pBody; }
	///@brief	新しくボデーをセットする。
	void		setBody(b2Body *pBody);

	///@brief	グローバルな座標系に物理ワープを追従させるかを設定
	void		setDependToWorldPosition(boolean flg) { _dependToWorldPos = flg; };
	///@brief	グローバルな座標系に物理ワープが追従するか見る
	bool		getDependToWorldPosition() { return _dependToWorldPos; };

	///@brief	物理の回転をノードのアングルにも影響させるかどうか
	bool		isEnableAngleSynchro() { return _enableAngleSynchro; };
	///@brief	物理の回転をノードのアングルにも影響させるかどうかをセットする
	void		setEnableAngleSynchro(bool flg) { _enableAngleSynchro = flg; };
	
	///@brief	物理本体へ影響させるかどうか
	bool		isEffectToNode() { return _effectToNode; };
	///@brief	物理本体へ影響させるかを判定する。
	void		setEffectToNode(bool flg) { _effectToNode = flg; };

	virtual		~B2NodeComponent();

protected:
	B2WorldLayer	*_pWorldLayer;
	b2Body			*_pBody;
	cocos2d::Vec2	_prevOwnerPos;
	float			_prevOwnerRot;
	bool			_dependToWorldPos;
	bool			_enableAngleSynchro;
	void			_updatePhysicsPosToNode();
	void			_updateNodePosToPhysics();
	void			_updatePhysicsRotToNode();
	void			_updateNodeRotToPhysics();
	bool			_effectToNode;
};


#endif // !_B2_NODE_COMPONENT_H
