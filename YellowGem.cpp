#include "Gem.h"

YellowGem::YellowGem()
{
	_state = 0;
	_type = yellow;
	_particleColor1 = kColor2;
}

bool YellowGem::createGemSprite(Node *layer, Point point,bool visible)
{
	Gem::createGemSpriteByName(layer, point, visible, "yellow.png");
	return true;
}