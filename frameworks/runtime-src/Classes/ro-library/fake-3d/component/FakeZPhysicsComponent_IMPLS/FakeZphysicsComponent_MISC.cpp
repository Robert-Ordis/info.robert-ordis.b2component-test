#include "../FakeZPhysicsComponent.h"

float	FakeZPhysicsComponent::convertToSlopeRatio(float localHeight) {
	float slopeMax = _maxStature - _minStature;
	float onSlope = localHeight - _minStature;
	if (slopeMax == 0) {
		if (onSlope == 0) {
			return 0;
		}
		else {
			return onSlope < 0 ? -1.0 : 1.0;
		}
	}
	if (onSlope < 0.0 && onSlope > -0.6) {
		return 0;
	}
	return onSlope / slopeMax;
}
