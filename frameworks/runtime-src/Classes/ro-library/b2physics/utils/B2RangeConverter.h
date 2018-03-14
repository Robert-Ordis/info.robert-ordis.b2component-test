#ifndef _B2_RANGE_CONVERTER_H
#define	_B2_RANGE_CONVERTER_H
#include <cocos2d.h>
#include <Box2D/Box2D.h>
/**
 *	@file	B2RangeConverter.h
 *	@brief	Cocos2dx単位系（ピクセル）とBox2D単位系（メートル）が違うので変換ユーティリティ用意…。
 *	@author	Robert_Ordis
 */

class B2RangeConverter {
public:
	static cocos2d::Vec2	convertVec2ToCC(b2Vec2 vector, float ptmRatio) {
		cocos2d::Vec2	retVal;
		retVal.x = vector.x * ptmRatio;
		retVal.y = vector.y * ptmRatio;
		return retVal;
	};
	static b2Vec2			convertVec2ToB2(cocos2d::Vec2 vector, float ptmRatio) {
		b2Vec2 retVal;
		retVal.x = vector.x / ptmRatio;
		retVal.y = vector.y / ptmRatio;
		return retVal;
	};
};

#endif // !_B2_RANGE_CONVERTER_H
