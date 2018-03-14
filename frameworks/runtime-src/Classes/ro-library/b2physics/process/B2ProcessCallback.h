#ifndef B2_PROCESS_CALLBACK_H
#define B2_PROCESS_CALLBACK_H
#include <cocos2d.h>
#include "../component/B2NodeComponent.h"

class B2NodeComponent;

typedef std::function<void(B2NodeComponent *sprA, B2NodeComponent *sprB)> B2ProcessCallbackFunc;

/**
 *	@brief	Box2Dの処理途中になんか最終的に渡したい処理が発生した時のための器
 *	@author Robert_Ordis
 *
 */
class B2ProcessCallback : public cocos2d::Ref {
public:
	//@brief	文字通りの作るやつ。B2NodeComponent2つをとりあえず
	static B2ProcessCallback *create(B2NodeComponent *objA, B2NodeComponent *objB);
	virtual bool init(B2NodeComponent *objA, B2NodeComponent *objB);
	
	virtual ~B2ProcessCallback();

	//@brief	やらせたい処理を実際に記述する。
	void setFunction(B2ProcessCallbackFunc func);

	//@brief	やりましょう！
	void process();

protected:
	B2ProcessCallback();
private:
	B2NodeComponent *_objA;
	B2NodeComponent *_objB;
	//関数部分
	B2ProcessCallbackFunc _function;
};

#endif
