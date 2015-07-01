#include "Gem.h"

WhiteGem::WhiteGem()
{
	_state = 0;
	_type = white;
	_particleColor1 = kColor5;
}

bool WhiteGem::createGemSprite(Node *layer, Point point,bool visible)
{
	Gem::createGemSpriteByName(layer, point, visible, "white.png");
	return true;
}