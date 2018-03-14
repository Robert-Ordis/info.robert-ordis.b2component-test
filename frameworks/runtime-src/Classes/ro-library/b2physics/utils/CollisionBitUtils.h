/**
 *	@file	CollisionBitUtils.h
 *	@brief	あたり判定のフィルタビットのユーティリティ
 *	@author	Robert_Ordis
 */

#ifndef _COLLISION_BIT_UTILS_H
#define	_COLLISION_BIT_UTILS_H
#include <Box2D/Box2D.h>

///@brief	ビットマスクをわかりやすくしたくてこういうのやってます。
typedef union CollisionBits {
	//@brief	ベース（32ビット。Box2Dの場合は改造してね）
	uint32	baseNum;
	bool operator==(const CollisionBits &a) { return baseNum == a.baseNum; }
	bool operator!=(const CollisionBits &a) { return baseNum != a.baseNum; }
	bool operator==(const uint32 a) { return baseNum == a; }
	bool operator!=(const uint32 a) { return baseNum != a; }
	CollisionBits operator&(const CollisionBits &a) {
		CollisionBits retVal;
		retVal.baseNum = baseNum & a.baseNum;
		return retVal;
	}
	CollisionBits operator|(const CollisionBits &a) {
		CollisionBits retVal;
		retVal.baseNum = baseNum | a.baseNum;
		return retVal;
	}
	CollisionBits operator^(const CollisionBits &a) {
		CollisionBits retVal;
		retVal.baseNum = baseNum ^ a.baseNum;
		return retVal;
	}
	CollisionBits operator~() {
		CollisionBits retVal;
		retVal.baseNum = ~baseNum;
		return retVal;
	}
	CollisionBits operator=(const uint32 a) {
		CollisionBits retVal;
		retVal.baseNum = a;
		return retVal;
	}
	//@brief	物理的な判定フラグの記述
	struct {
		//@brief	普通のモノとぶつかるかどうか
		int	obj0 : 1;
		//@brief	幽霊的な、特殊なモノに作用するかどうか
		int	obj1 : 1;
		//@brief	武器に作用するかどうか
		int	weapon0 : 1;
		//@brief	カベに作用するかどうか。
		int wall0 : 1;
		//@brief	特殊な壁に作用するかどうか
		int wall1 : 1;
		//@brief	見えてアクション取らせるかどうか（EyeSight反応用）
		//ただの壁や幽霊なんかは基本的にここは0になる。
		int do_act0 : 1;
		//@brief	do_actの特殊版（EyeSight反応用）
		//幽霊系とかステルスやっても見通されるものとかはこれが立つ。
		int do_act1 : 1;
		//@brief	そのまま後ろのものへ透過するかどうか（EyeSightComponent要）
		int see_through : 1;
		//@brief	床判定専用。上に乗れるかどうかってやつ（乗れないならはじかれる）
		int floor0 : 1;
		//@brief	特殊な床判定専用。
		int floor1 : 1;
		//@brief	水中。
		int liquid0 : 1;
		//@brief	音。（VisualSoundへのとっかかり。音は大体これだけON）
		int sound0 : 1;
	} as_physics;

	//@brief	音声系のビット定義（現在超適当）
	struct {
		int voice_bass : 1;
		int voice_tenor : 1;
		int voice_alto : 1;
		int voice_soprano : 1;
		int voice_monster : 1;
		int voice_animal : 1;
		int effect_explode : 1;
		int effect_metal : 1;
		int effect_wood : 1;
		int effect_thunder : 1;
		int effect_fire : 1;
		int effect_liquid : 1;
		int effect_wind : 1;
	} as_sound;
}collision_bit_t;


#endif // !_COLLISION_BIT_UTILS_H
