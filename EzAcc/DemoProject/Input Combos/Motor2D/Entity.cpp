#include "ctApp.h"
#include "Entity.h"
#include "ctRender.h"

Entity::Entity(int x, int y, EntityType type) : position(x, y), type(type)
{
}

Entity::~Entity()
{
}

void Entity::Draw(SDL_Texture* sprites, SDL_Surface* surface)
{
	SDL_Rect r = animation->GetCurrentFrame();
	
	if (animation != nullptr)
		App->render->Blit(SDL_BLENDMODE_BLEND, surface,  sprites, position.x, position.y - r.h, &(animation->GetCurrentFrame()));
}