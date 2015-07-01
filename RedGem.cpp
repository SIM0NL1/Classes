#include "Gem.h"

RedGem::RedGem()
{
	_state = 0;
	_type = red;
	_particleColor1 = kColor1;
}

bool RedGem::createGemSprite(Node *layer, Point point,bool visible)
{
	Gem::createGemSpriteByName(layer, point, visible, "red.png");
	return true;
}
