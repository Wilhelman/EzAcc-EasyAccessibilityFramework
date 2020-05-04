#include "ctApp.h"
#include "ctRender.h"
#include "ctEntities.h"
#include "ctTextures.h"
#include "Entity.h"
#include "ctAudio.h"
#include "ctWindow.h"
#include "ctLog.h"
#include "ctFadeToBlack.h"

#include "Player.h"


ctEntities::ctEntities()
{
	name = "entities";
}

// Destructor
ctEntities::~ctEntities()
{
	LOG("Unloading entities spritesheet");
	App->tex->UnLoad(entity_sprites);
}

bool ctEntities::Awake(pugi::xml_node& config)
{
	LOG("Loading Entities from config file");
	bool ret = true;

	spritesheetName = config.child("spritesheetSource").attribute("name").as_string();
	
	return ret;
}

bool ctEntities::Start()
{
	bool ret = true;

	entity_sprites = App->tex->Load(spritesheetName.data());

	if (entity_sprites == NULL) {
		LOG("Error loading entities spritesheet!!");
		ret = false;
	}

	if (!ret)
		return false;

	return ret;
}

bool ctEntities::PreUpdate()
{
	for (int i = 0; i < entities.capacity(); i++) {
		if (entities[i]->to_destroy) {
			delete(entities[i]);
			entities[i] = nullptr;
			entities.erase(entities.cbegin() + i);
			entities.shrink_to_fit();
		}
	}
	return true;
}

// Called before render is available
bool ctEntities::Update(float dt)
{

	for (int i = 0; i < entities.capacity(); i++)
		if (entities.at(i) != nullptr) entities[i]->Update(dt);

	for (int i = 0; i < entities.capacity(); i++)
		if (entities.at(i) != nullptr) entities[i]->Draw(entity_sprites);

	return true;
}

// Called before quitting
bool ctEntities::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(entity_sprites);



	return true;
}

bool ctEntities::SpawnEntity(int x, int y, EntityType type)
{
	// find room for the new entity
	bool ret = false;

	switch (type)
	{
	case EntityType::PLAYER: {
		Player* player = new Player(x, y, PLAYER);
		entities.push_back(player);
		ret = true;
		break;
	}
	default:
		break;
	}


	return ret;
}

Player* ctEntities::GetPlayer() const {

	for (uint i = 0; i < entities.capacity(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == PLAYER)
				return (Player*)entities[i];
		}
	}

	return nullptr;

}