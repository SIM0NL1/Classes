#include "Gem.h"

OrangeGem::OrangeGem()
{
	_state = 0;
//	_type = orange;
	_particleColor1 = kColor7;
}

bool OrangeGem::createGemSprite(Node *layer, Point point,bool visible)
{
	Gem::createGemSpriteByName(layer, point, visible, "orange.png");
	return true;
}