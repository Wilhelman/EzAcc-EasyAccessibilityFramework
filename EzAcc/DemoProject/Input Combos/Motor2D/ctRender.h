#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "ctPoint.h"
#include "ctModule.h"
#include <string>
#include <vector>
#include "ImGui/imgui.h"
#include "EzAcc/include/EzAcc.h"

class ctRender : public ctModule
{
public:

	ctRender();

	// Destructor
	virtual ~ctRender();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	void ModulateTextureColor(SDL_Texture* texture, SDL_Color color);
	void ProcessTextureWithSurface(SDL_Texture* texture, SDL_Surface* surface, SDL_Color keyColor1 = SDL_Color(), SDL_Color newColor1 = SDL_Color(), SDL_Color keyColor2 = SDL_Color(), SDL_Color newColor2 = SDL_Color());
	bool Blit(SDL_BlendMode blendMode, SDL_Surface* surface, SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Utils
	iPoint ScreenToWorld(int x, int y) const;
	void DrawVision();
	void DrawHearing();
	void DrawInput();
	void DrawAbout();
	void DrawCognitive();
	void DrawLanguage();
	void DrawConsole();

	void ShowHelpMarker(const char* desc);

	void Log(const char* new_log);
	void AddLogToConsole(const char* log);

public:
	bool debug = false;
	SDL_Renderer*	renderer = nullptr;
	SDL_GLContext context;
	SDL_Rect		camera = { 0, 0, 0, 0 };
	SDL_Rect		last_camera = { 0, 0, 0, 0 };
	SDL_Rect		viewport = { 0, 0, 0, 0 };
	SDL_Color		background = { 0, 0, 0, 0 };
	bool			vsync_state = false;

	bool console_available = false;

private:
	bool show_demo_window = false;
	bool show_about = false;
	bool show_vision = false;
	bool show_hearing = false;
	bool show_input = false;
	bool show_cognitive = false;
	bool show_language = false;
	bool show_console = false;
	bool change_debug = false;

	char tmp_name[100];
	std::string name;


	bool binding = false;
	int lastBindedKey = -1;

	SDL_Color modColor;
	EzAcc_Macro macroTest;
	int macroTestIndex = -1;
	bool bindingMacroKey = false;
	bool bindingMacroKeyValue1 = false;
	bool bindingMacroKeyValue2 = false;

	

	std::vector<std::string> init_logs;
	ImGuiTextBuffer text_buffer;
	bool have_to_scroll = false;
};

#endif // __j1RENDER_H__