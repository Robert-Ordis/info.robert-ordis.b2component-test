#ifndef __B2_DEBUG_DRAW_LAYER_H__
#define __B2_DEBUG_DRAW_LAYER_H__

#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include "GLES-Render.h"

USING_NS_CC;

class B2DebugDrawLayer : public Layer
{
	b2World *mB2World;
	GLESDebugDraw *mB2DebugDraw;
	const float mPtmRatio;

public:
	B2DebugDrawLayer(b2World *pB2World, float pPtmRatio);

	static B2DebugDrawLayer *create(b2World *pB2World, float pPtmRatio);

	virtual bool init();

	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);

protected:
	Mat4 _modelViewMV;

	void onDraw();

	CustomCommand _customCommand;

};

#endif // __B2_DEBUG_DRAW_LAYER_H__