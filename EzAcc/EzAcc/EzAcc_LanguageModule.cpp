#include "pch.h"
#include "EzAcc_LanguageModule.h"

#include "EzAcc_Log.h"


EzAcc_LanguageModule::EzAcc_LanguageModule()
{
	EZACC_LOG("EzAcc: Generating EzAcc_LanguageModule ...");
}

// Destructor
EzAcc_LanguageModule::~EzAcc_LanguageModule()
{

}

// Called before render is available
bool EzAcc_LanguageModule::Awake(pugi::xml_node& config)
{
	EZACC_LOG("Loading Languages");
	bool ret = true;

	pugi::xml_document	language_file;
	pugi::xml_node* node = &this->LoadLanguages(language_file);
	current_language.create(config.attribute("current").as_string());

	for (pugi::xml_node languages = node->first_child(); languages && ret; languages = languages.next_sibling())
	{
		p2SString tmp_language;
		tmp_language.create(languages.name());
		posible_languages.PushBack(tmp_language);
	}

	node = &node->child(current_language.GetString());

	dictionary.press_space_btn.create(node->child("press_space").attribute("string").as_string());
	dictionary.new_game_btn.create(node->child("new_game_btn").attribute("string").as_string());
	dictionary.continue_btn.create(node->child("continue_btn").attribute("string").as_string());
	dictionary.music_volume.create(node->child("music_volume").attribute("string").as_string());
	dictionary.fx_volume.create(node->child("fx_volume").attribute("string").as_string());
	dictionary.cap_to.create(node->child("cap_to").attribute("string").as_string());
	dictionary.language_option.create(node->child("language_option").attribute("string").as_string());
	dictionary.quit_lan.create(node->child("quit_lan").attribute("string").as_string());
	dictionary.save_lan.create(node->child("save_lan").attribute("string").as_string());
	dictionary.load_lan.create(node->child("load_lan").attribute("string").as_string());
	dictionary.main_menu_lan.create(node->child("main_menu_lan").attribute("string").as_string());
	dictionary.credits_btn.create(node->child("credits_btn").attribute("string").as_string());

	return ret;
}

bool EzAcc_LanguageModule::Start()
{
	return true;
}

bool EzAcc_LanguageModule::PreUpdate()
{

	return true;
}

bool EzAcc_LanguageModule::PostUpdate()
{

	return true;
}

bool EzAcc_LanguageModule::CleanUp()
{
	return true;
}

pugi::xml_node EzAcc_LanguageModule::LoadLanguages(pugi::xml_document& language_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = language_file.load_file("ezacc_languages");

	if (result == NULL)
		EZACC_LOG("Could not load xml file config.xml. pugi error: %s", result.description());
	else
		ret = language_file.child("languages");
	return ret;
}

bool EzAcc_LanguageModule::Load(pugi::xml_node&)
{
	return false;
}

bool EzAcc_LanguageModule::Save(pugi::xml_node&) const
{
	return false;
}

void EzAcc_LanguageModule::ChangeCurrentLanguage(p2SString new_language)
{
	current_language = new_language;

	bool ret = true;

	pugi::xml_document	language_file;
	pugi::xml_node* node = &this->LoadLanguages(language_file);

	for (pugi::xml_node languages = node->first_child(); languages && ret; languages = languages.next_sibling())
	{
		p2SString tmp_language;
		tmp_language.create(languages.name());
		posible_languages.PushBack(tmp_language);
	}

	node = &node->child(current_language.GetString());

	dictionary.press_space_btn.create(node->child("press_space").attribute("string").as_string());
	dictionary.new_game_btn.create(node->child("new_game_btn").attribute("string").as_string());
	dictionary.continue_btn.create(node->child("continue_btn").attribute("string").as_string());
	dictionary.music_volume.create(node->child("music_volume").attribute("string").as_string());
	dictionary.fx_volume.create(node->child("fx_volume").attribute("string").as_string());
	dictionary.cap_to.create(node->child("cap_to").attribute("string").as_string());
	dictionary.language_option.create(node->child("language_option").attribute("string").as_string());
	dictionary.quit_lan.create(node->child("quit_lan").attribute("string").as_string());
	dictionary.save_lan.create(node->child("save_lan").attribute("string").as_string());
	dictionary.load_lan.create(node->child("load_lan").attribute("string").as_string());
	dictionary.main_menu_lan.create(node->child("main_menu_lan").attribute("string").as_string());
	dictionary.credits_btn.create(node->child("credits_btn").attribute("string").as_string());
}

p2SString EzAcc_LanguageModule::GetWordFromKey(p2SString key)
{
	bool ret = true;

	pugi::xml_document	language_file;
	pugi::xml_node* node = &this->LoadLanguages(language_file);

	for (pugi::xml_node languages = node->first_child(); languages && ret; languages = languages.next_sibling())
	{
		p2SString tmp_language;
		tmp_language.create(languages.name());
		posible_languages.PushBack(tmp_language);
	}

	node = &node->child(current_language.GetString());

	p2SString wordToReturn;
	wordToReturn.create(node->child(key.GetString()).attribute("string").as_string());
	return wordToReturn;
}

EzAcc_Dictionary EzAcc_LanguageModule::GetDictionary() const
{
	return dictionary;
}
