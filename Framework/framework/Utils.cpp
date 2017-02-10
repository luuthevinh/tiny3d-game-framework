#include "Utils.h"

USING_NS_TINY;

D3DMATERIAL9 Utils::WHITE_MATERIAL = {
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  // set diffuse color to white
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  // set ambient color to white
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  // set ambient color to white
	D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),	// Emissive
	2.0f								// power
};

Utils::Utils()
{
}
