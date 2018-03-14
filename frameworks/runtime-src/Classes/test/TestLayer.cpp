#include <cocos2d.h>
#include "TestLayer.h"
#include "ro-library/b2physics/debug/B2DebugDrawLayer.h"
#include "ro-library/b2physics/utils/B2RangeConverter.h"
USING_NS_CC;

//------static functions------

Scene*	TestLayer::createWithScene() {
	auto	layer = TestLayer::create();
	auto	scene = Scene::create();

	scene->addChild(layer);
	for (int i = 0; i < 9; i++) {
		layer = TestLayer::create();
		layer->setPosition(Vec2(0, 50*i));
		scene->addChild(layer);
	}
	
	auto dlayer = B2DebugDrawLayer::create(layer->getB2World(), layer->getPtmRatio());

	// シーンに子としてレイヤーを追加
	scene->addChild(dlayer);

	return scene;

}

Node *watchnode;
B2NodeComponent *watchcmp;
//------member functions------
bool	TestLayer::init() {
	bool res = B2WorldLayer::init(32.0);
	CCLOG("[%x]%s::%s, init result->%d", this, typeid(this).name(), __func__, res);
	if (!res) {
		return false;
	}

	this->getB2World()->SetContactListener(this);

	_pFakeZWorld = new FakeZPhysicsWorld(this);
	_pFakeZWorld->setGravity(-9.8 * getPtmRatio() * 3.8);
	_pFakeZWorld->setContactListener(this);
	CCLOG("world:%x", _pFakeZWorld);

	Sprite *spr;
	B2NodeComponent *cmp;
	FakeZPhysicsComponent *zcmp;
	for (int i = 0; i < 100; i++){
		std::tie(spr, cmp, zcmp) = _makePhysicsNode("HelloWorld.png", 0.25);
		spr->setPosition(Vec2(20+ 61 * i, 20 + 61 * i));
		spr->setPositionZ(400 + 50 * i);
		//cmp->getBody()->SetLinearVelocity(b2Vec2(1, 1));
		//cmp->getBody()->SetAngularVelocity(1);
		zcmp->setMaxStature(10);
		zcmp->setMinStature(10);
		zcmp->setTunnelingAllowed(false);
		this->addChild(spr);
	}
	
	watchnode = spr;
	watchcmp = cmp;
	//this->addChild(spr);
	
	

	CCLOG("spr:%x, cmp:%x", spr, cmp);
	CCLOG("->body:%x, fixtures:%x", cmp->getBody(), cmp->getBody()->GetFixtureList());
	spr = nullptr;
	cmp = nullptr;
	std::tie(spr, cmp, zcmp) = _makePhysicsNode("HelloWorld.png", 2.5);
	spr->setPosition(Vec2(240, 240));
	spr->setPositionZ(100);
	spr->setRotation(90);
	//cmp->getBody()->SetAngularVelocity(-1);
	//cmp->getBody()->SetLinearDamping(1);
	zcmp->setMaxStature(10);
	zcmp->setMinStature(10);
	zcmp->setZVelocity(400);
	//zcmp->setRestitution(0.2);
	//zcmp->setPriority(2);
	//cmp->setEffectToNode(false);
	this->addChild(spr);
	//watchnode->addChild(spr);

	CCLOG("spr:%x, cmp:%x", spr, cmp);
	CCLOG("->body:%x, fixtures:%x", cmp->getBody(), cmp->getBody()->GetFixtureList());
	spr = nullptr;
	cmp = nullptr;
	std::tie(spr, cmp, zcmp) = _makePhysicsNode("HelloWorld.png", 150);
	spr->setPosition(Vec2(350, 350));
	spr->setPositionZ(-100);
	spr->setRotation(90);
	cmp->getBody()->SetAngularVelocity(-1);
	cmp->getBody()->SetLinearDamping(1);
	
	//cmp->setEffectToNode(false);
	zcmp->setMass(NAN);
	zcmp->setGravityRatio(0);
	zcmp->setZVelocity(0);
	zcmp->setMaxStature(100);
	zcmp->setMinStature(100);
	zcmp->setRenderHeight(99);
	zcmp->setTunnelingAllowed(false);
	//zcmp->setRestitution(0);
	_pFakeGround = spr;

	this->addChild(spr);


	CCLOG("spr:%x, cmp:%x", spr, cmp);
	CCLOG("->body:%x, fixtures:%x", cmp->getBody(), cmp->getBody()->GetFixtureList());
	
	this->scheduleUpdate();


	return true;
}

void	TestLayer::update(float dt) {
	//dt *= 0.5;
	//CCLOG("[%x]%s,%s: rotation->%f", this, typeid(this).name(), __func__, watchnode->getRotation());
	Camera::getDefaultCamera()->setScale(8);
	Camera::getDefaultCamera()->setPosition(Vec2(-450, -250));
	static float elapsed = 0.0;
	static float next = 10;
	this->getB2World()->Step(dt, 8, 4);
	this->_execB2Task();
	this->_pFakeZWorld->step(dt);
	elapsed += dt/6;
	static int count = 0;
	if (elapsed > next && _pFakeGround != nullptr) {
		auto cmp = dynamic_cast<FakeZPhysicsComponent *>(_pFakeGround->getComponent(FAKE_Z_COMPONENT));
		//cmp->setZVelocity(cmp->getZVelocity() + 0.05);
		if (elapsed > (next * 2)) {
			count++;
			_pFakeGround->setPositionZ(10 * (-count));
			if (cmp->getZVelocity() > 40) {
				cmp->setZVelocity(0);
			}
			elapsed = next;
		}
		if (count > 1) {
			this->removeChild(_pFakeGround);
			_pFakeGround = nullptr;
		}
	}
	
}

std::tuple<Sprite*, B2NodeComponent*, FakeZPhysicsComponent*> TestLayer::_makePhysicsNode(const std::string& path, float scale) {
	auto	*spr = Sprite::create(path);
	spr->setScale(scale);
	auto	sprSize = spr->getContentSize() * scale;
	b2BodyDef blockBodyRef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	blockBodyRef.type = b2_dynamicBody;

	shape.SetAsBox(sprSize.width / getPtmRatio() / 2, sprSize.height / getPtmRatio() / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 10 * scale;
	fixtureDef.friction = 0.9;
	fixtureDef.restitution = 0;
	
	auto body = getB2World()->CreateBody(&blockBodyRef);
	body->CreateFixture(&fixtureDef);
	auto	component = B2NodeComponent::create(this, body);
	component->setName("B2Component");
	component->setEffectToNode(true);
	spr->addComponent(component);

	auto zcomponent = _pFakeZWorld->createComponent();
	zcomponent->setMass(body->GetMass());
	zcomponent->setRestitution(0.6);
	spr->addComponent(zcomponent);
	


	return std::tuple<Sprite*, B2NodeComponent*, FakeZPhysicsComponent*>(spr, component, zcomponent);

}

TestLayer::~TestLayer() {
	CC_SAFE_DELETE(_pFakeZWorld);
}

void	TestLayer::BeginContact(b2Contact *contact) {
	//CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
	auto bodA = contact->GetFixtureA()->GetBody();
	auto bodB = contact->GetFixtureB()->GetBody();
}
void	TestLayer::EndContact(b2Contact *contact) {
	//CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
	auto bodA = contact->GetFixtureA()->GetBody();
	auto bodB = contact->GetFixtureB()->GetBody();

}

static float testOverlapWithZAxis(Component *cmpA, Component *cmpB, b2Contact *contact) {

	auto aZ = dynamic_cast<FakeZPhysicsComponent*>(cmpA);
	auto bZ = dynamic_cast<FakeZPhysicsComponent*>(cmpB);

	if (aZ == nullptr || bZ == nullptr) {
		return -1.0;
	}

	float posA = aZ->getWorldZPosition();
	float posB = bZ->getWorldZPosition();

	//contact->GetManifold()のいろんなの使えば衝突個所とかベクトルとか工夫できそうな気がする。
	if (posA < posB) {
		//ノードBの場所がノードAの身長分のどこに該当しそうか算出する。
		return aZ->convertToSlopeRatio(posB - posA);
	}
	else {
		//ノードAの場所がノードBの身長分のどこに該当しそうか逆算する
		return bZ->convertToSlopeRatio(posA - posB);
	}
}

void	TestLayer::PreSolve(b2Contact *contact, const b2Manifold* oldManifold) {
	//何とかしてコンポーネントにまでたどり着く。
	auto cmpA = static_cast<Component*>(contact->GetFixtureA()->GetBody()->GetUserData());
	auto cmpB = static_cast<Component*>(contact->GetFixtureB()->GetBody()->GetUserData());
	if (cmpA == nullptr || cmpB == nullptr) {
		CCASSERT(false, "STOOOP!!!");
		return;
	}
	auto nodeA = cmpA->getOwner();
	auto nodeB = cmpB->getOwner();
	//CCLOG("[%x]%s::%s, slopeRatio->%f", this, typeid(this).name(), __func__, testOverlapWithZAxis(nodeA->getComponent(FAKE_Z_COMPONENT), nodeB->getComponent(FAKE_Z_COMPONENT), contact));

	auto zmpA = dynamic_cast<FakeZPhysicsComponent*>(nodeA->getComponent(FAKE_Z_COMPONENT));
	auto zmpB = dynamic_cast<FakeZPhysicsComponent*>(nodeB->getComponent(FAKE_Z_COMPONENT));
	if (_pFakeZWorld->getLandingManager()->checkIfLandingBetween(zmpA, zmpB)) {
		contact->SetEnabled(false);
	}
	if (testOverlapWithZAxis(zmpA, zmpB, contact) >= 0.0) {
		contact->SetEnabled(false);
		return;
	}

}
void	TestLayer::PostSolve(b2Contact* contact, const b2ContactImpulse *impulse) {
	//CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
}

std::tuple<FakeZPhysicsComponent*, float> TestLayer::getNearestGround(FakeZPhysicsComponent *cmp) {
	//CCLOG("[%x]%s::%s, search for->%x", this, typeid(this).name(), __func__, cmp);
	/*
	auto zcmp = dynamic_cast<FakeZPhysicsComponent*>(_pFakeGround->getComponent(FAKE_Z_COMPONENT));
	CCLOG("[%x]%s::%s, found[%x]", this, typeid(this).name(), __func__, _pFakeGround);
	*/
	int priority = -1;
	float gheight;
	float ratio;
	FakeZPhysicsComponent *grCmp = nullptr;

	float myPos = cmp->getWorldZPosition();

	auto node = cmp->getOwner();
	auto b2Cmp = dynamic_cast<B2NodeComponent *>(node->getComponent("B2Component"));
	if (b2Cmp == nullptr) {
		return std::tuple<FakeZPhysicsComponent*, float>(nullptr, 0);
	}
	auto cList = b2Cmp->getBody()->GetContactList();
	while (cList != nullptr) {
//		CCLOG("CONTACT LIST->%x", cList->other);
		auto gBody = cList->other;
		auto gBCmp = static_cast<B2NodeComponent *>(gBody->GetUserData());
		auto gNode = gBCmp->getOwner();
		auto gZcmp = dynamic_cast<FakeZPhysicsComponent *>(gNode->getComponent(FAKE_Z_COMPONENT));
		if (gZcmp != nullptr) {
//			CCLOG("OPPONENT HAS Z COMPONENT");
			//Zの衝突がいる。ので、まず坂のどの辺に当たるかを出す
			float tmpRatio = testOverlapWithZAxis(gZcmp, cmp, cList->contact);
//			CCLOG("TMP SLOPE RATIO:%f, PRIORITY:%d", tmpRatio, gZcmp->getPriority());
//			CCLOG("POSZ:%f, WITHSTATURE:%f", gZcmp->getWorldZPosition(), gZcmp->getWorldZPosition() + gZcmp->getStature(tmpRatio));
			if (!cList->contact->IsEnabled() && gZcmp->getWorldZPosition() < myPos) {
//				CCLOG("OPPONENT IS GROUND");
				//んで、ちゃんと乗っかる候補に入っている。のでここから近い判定を出してあげる。
				if (grCmp == nullptr) {
//					CCLOG("NEWBIE GROUND");
					grCmp = gZcmp;
					priority = grCmp->getPriority();
					gheight = grCmp->getStature(tmpRatio) + grCmp->getWorldZPosition();
					ratio = tmpRatio;
				}
				else {
					float willHeight = gZcmp->getWorldZPosition() + gZcmp->getStature(tmpRatio);
//					CCLOG("nowPri:%d, nextPri:%d", priority, gZcmp->getPriority());
//					CCLOG("now:%f vs next:%f", gheight, willHeight);
					if (priority < gZcmp->getPriority()){
//						CCLOG("CHANGED WITH PRIORITY");

						//優先度から塗り替えにかかったやつ
						grCmp = gZcmp;
						priority = grCmp->getPriority();
						gheight = willHeight;
						ratio = tmpRatio;
					}
					else if (priority == gZcmp->getPriority() && gheight < willHeight) {
						//普通に高さが近いやつ。
//						CCLOG("CHANGED WITH HEIGHT");

						gheight = willHeight;
						grCmp = gZcmp;
						ratio = tmpRatio;
					}
				}
			}
		}
		cList = cList->next;
	}
	//とりあえずどの床なのか確定。
	//次に、今の衝突管理がどうなっているかを判断する。
	auto lManager = _pFakeZWorld->getLandingManager();
	auto gInfo = lManager->getLandedByUpper(cmp);
	if (gInfo != nullptr && gInfo->getGround() != grCmp) {
		//すでに誰かとつながっており、今回検出した地面とは全然違う場合。
		//frictionJointを解除する。んでコンポーネントのステータスも一時的にフロートにする。。
		cmp->downLandingFlag();
		auto gJoint = static_cast<b2FrictionJoint *>(gInfo->getUserData());
		lManager->releaseWithUpper(cmp);
		this->getB2World()->DestroyJoint(gJoint);
		
	}

	return std::tuple<FakeZPhysicsComponent*, float>(grCmp, ratio);
}

void	TestLayer::onFloated(FakeZPhysicsComponent *cmp) {
	//CCLOG("[%x]%s::%s, floated component->%x", this, typeid(this).name(), __func__, cmp);
	auto lManager = _pFakeZWorld->getLandingManager();
	auto gInfo = lManager->getLandedByUpper(cmp);
	if (gInfo != nullptr) {
		auto gJoint = static_cast<b2FrictionJoint *>(gInfo->getUserData());
		lManager->releaseWithUpper(cmp);
		this->getB2World()->DestroyJoint(gJoint);
	}
}

void	TestLayer::onLanded(FakeZPhysicsComponent *cmp, FakeZPhysicsComponent *grd, float impact) {
	//CCLOG("[%x]%s::%s, landed component->%x, ground->%x, impact->%f", this, typeid(this).name(), __func__, cmp, grd, impact);
	//新たにジョイントを作る。
	auto lManager = _pFakeZWorld->getLandingManager();
	auto nodA = cmp->getOwner();
	auto nodB = grd->getOwner();

	auto bodA = dynamic_cast<B2NodeComponent *>(nodA->getComponent("B2Component"));
	auto bodB = dynamic_cast<B2NodeComponent *>(nodB->getComponent("B2Component"));

	auto jDef = b2FrictionJointDef();
	jDef.bodyA = bodA->getBody();
	jDef.bodyB = bodB->getBody();
	jDef.collideConnected = true;
	jDef.maxForce = 165000000;
	jDef.maxTorque = 165000000;
	
	auto newJoint = this->getB2World()->CreateJoint(&jDef);
	lManager->putLandingData(cmp, grd, newJoint);

}

void	TestLayer::SayGoodbye(b2Joint *joint) {
	//CCLOG("[%x]%s::%s good bye (joint) %x", this, typeid(this).name(), __func__, joint);
	_pFakeZWorld->getLandingManager()->releaseWithUserData(joint);
}

void	TestLayer::SayGoodbye(b2Fixture *fixture) {
	//CCLOG("[%x]%s::%s good bye (fixture) %x", this, typeid(this).name(), __func__, fixture);
}