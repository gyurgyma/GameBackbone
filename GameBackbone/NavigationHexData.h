#pragma once
#include "DllUtil.h"

#define BLOCKED_HEX_WEIGHT 10000

/// <summary> Information stored in each navigation hex. </summary>
libGameBackbone struct NavigationHexData {
	int weight;
	unsigned int blockerDist;
};

libGameBackbone bool operator<(const NavigationHexData& lhs, const NavigationHexData& rhs) {
	return lhs.weight < rhs.weight;
}
libGameBackbone bool operator>(const NavigationHexData& lhs, const NavigationHexData& rhs) {
	return lhs.weight > rhs.weight;
}
