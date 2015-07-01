#include "Gem.h"

PurpleGem::PurpleGem()
{
	_state = 0;
	_type = purple;
	_particleColor1 = kColor7;
}

bool PurpleGem::createGemSprite(Node *layer, Point point,bool visible)
{
	Gem::createGemSpriteByName(layer, point, visible, "purple.png");
	return true;
}