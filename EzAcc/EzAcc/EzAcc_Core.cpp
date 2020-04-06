#include "pch.h"
#include "EzAcc_Core.h"

#include <iostream> 

#include "EzAcc_InputModule.h"
#include "EzAcc_Log.h"

// Constructor
EzAcc_Core::EzAcc_Core()
{
	LOG("EzAcc: Generating EzAcc_InputModule ...");
	input = new EzAcc_InputModule();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	/*AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(input_combo);
	AddModule(ken_stage_scene);
	AddModule(entities);
	AddModule(gui);
	AddModule(fonts);
	AddModule(fadeToBlack);*/

	// render last to swap buffer
	//AddModule(render);

}

// Destructor
EzAcc_Core::~EzAcc_Core()
{
	// release modules

	for (std::list<EzAcc_Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		RELEASE(*it);
	}

	modules.clear();
}

void EzAcc_Core::AddModule(EzAcc_Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool EzAcc_Core::Awake()
{
	bool ret = false;

	// Read config file
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	config = LoadConfig(config_file);


	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("ezacc");
	}

	if (ret == true)
	{

		std::list<EzAcc_Module*>::iterator it = modules.begin();

		while (it != modules.end() && ret == true)
		{
			pugi::xml_node mod_node = config.child((*it)->name.data());
			ret = (*it)->Awake(mod_node);

			it++;
		}
	}


	return ret;
}

// Called before the first frame
bool EzAcc_Core::Start()
{
	bool ret = true;

	std::list<EzAcc_Module*>::iterator it = modules.begin();

	while (it != modules.end() && ret == true)
	{
		if ((*it)->active == false) {
			it++;
			continue;
		}
		ret = (*it)->Start();
		it++;
	}

	return ret;
}

// Called each loop iteration
bool EzAcc_Core::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void EzAcc_Core::PrepareUpdate()
{

}

// ---------------------------------------------
void EzAcc_Core::FinishUpdate()
{
	if (!all_modules_loaded)
		all_modules_loaded = true;
}

// Call modules before each loop iteration
bool EzAcc_Core::PreUpdate()
{
	bool ret = true;

	EzAcc_Module* pModule = NULL;

	for (std::list<EzAcc_Module*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		ret = (*it)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool EzAcc_Core::DoUpdate()
{
	bool ret = true;

	EzAcc_Module* pModule = NULL;

	for (std::list<EzAcc_Module*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}
		// TODOG : Do dt thing
		//(all_modules_loaded) ? ret = (*it)->Update(dt) : ret = (*it)->Update(0);
	}

	return ret;
}

// Call modules after each loop iteration
bool EzAcc_Core::PostUpdate()
{
	bool ret = true;

	EzAcc_Module* pModule = NULL;

	for (std::list<EzAcc_Module*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		ret = (*it)->PostUpdate();
	}

	//PERF_PEEK(ptimer);
	return ret;
}

// Called before quitting
bool EzAcc_Core::CleanUp()
{
	bool ret = true;

	std::list<EzAcc_Module*>::reverse_iterator it = modules.rbegin();

	while (it != modules.rend() && ret == true)
	{
		ret = (*it)->CleanUp();
		it++;
	}

	return ret;
}

pugi::xml_node EzAcc_Core::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL)
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");
	return ret;
}
