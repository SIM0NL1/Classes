#include "Gem.h"

GreenGem::GreenGem()
{
	_state = 0;
	_type = green;
	_particleColor1 = kColor3;
}

bool GreenGem::createGemSprite(Node *layer, Point point,bool visible)
{
	Gem::createGemSpriteByName(layer, point, visible, "green.png");
	return true;
}