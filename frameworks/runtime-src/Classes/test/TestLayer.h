#ifndef _TEST_LAYER_H
#define	_TEST_LAYER_H

#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include "ro-library/b2physics/world/B2WorldLayer.h"
#include "ro-library/fake-3d/world/FakeZPhysicsWorld.h"
#include "ro-library/fake-3d/listener/FakeZPhysicsContactListener.h"

USING_NS_CC;

class TestLayer :public B2WorldLayer, public FakeZPhysicsContactListener{
public:
	virtual bool	init();

	virtual void	update(float dt);

	CREATE_FUNC(TestLayer);
	
	static Scene*	createWithScene();

	virtual	~TestLayer();

public:		//B2WorldLayerからの継承
	virtual void	BeginContact(b2Contact *contact);
	virtual void	EndContact(b2Contact *contact);
	virtual void	PreSolve(b2Contact *contact, const b2Manifold* oldManifold);
	virtual void	PostSolve(b2Contact* contact, const b2ContactImpulse *impulse);
	virtual void	SayGoodbye(b2Joint *joint);
	virtual void	SayGoodbye(b2Fixture *fixture);

public:		//FakeZPhysicsContactListenerの継承
	virtual	std::tuple<FakeZPhysicsComponent*, float> getNearestGround(FakeZPhysicsComponent *cmp);
	virtual	void	onFloated(FakeZPhysicsComponent *cmp);
	virtual	void	onLanded(FakeZPhysicsComponent *cmp, FakeZPhysicsComponent *ground, float impact);

private:
	std::tuple<Sprite*, B2NodeComponent*, FakeZPhysicsComponent*> _makePhysicsNode(const std::string& path, float scale);
	FakeZPhysicsWorld	*_pFakeZWorld;
	Node				*_pFakeGround = nullptr;
};


#endif // !_TEST_LAYER_H

