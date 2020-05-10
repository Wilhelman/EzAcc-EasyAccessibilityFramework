#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctWindow.h"
#include "ctRender.h"
#include "Player.h"
#include "ctEntities.h"
#include "ctInput.h"
#include "ctKenStageScene.h"

#include "Glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>
#include "ImGui/imgui.h"
//#include "ImGui/imgui_impl_opengl3.h"
#include "SDL/include/SDL.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"

#include "EzAcc/include/EzAcc.h"

#define VSYNC true

ctRender::ctRender() : ctModule()
{ 
	name = "renderer";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
ctRender::~ctRender()
{}

// Called before render is available
bool ctRender::Awake(pugi::xml_node& config)
{
	debug = false;
	GLenum err = glewInit();
	LOG("Create SDL rendering context");

	bool ret = true;
	context = SDL_GL_CreateContext(App->win->window);
	if (context == NULL)
	{
		ret = false;
	}
	vsync_state = false;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		vsync_state = true;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = -1173;
		camera.y = -21;
	}

	// OpenGL
	if (ret == true)
	{

		// Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			ret = false;
		}

		// Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			ret = false;
		}

		glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

		glClearDepth(1.0f);

		// Initialize clear color
		glClearColor(0.12f, 0.12f, 0.12f, 1.f);

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			ret = false;
		}

		glDisable(GL_CULL_FACE);
	}
	
	return ret;
}

// Called before the first frame
bool ctRender::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->win->window, App->render->context);
	ImGui_ImplOpenGL3_Init();
	GLenum err = glewInit();
	return true;
}

// Called each loop iteration
bool ctRender::PreUpdate()
{
	if(!debug)
		SDL_RenderClear(renderer);
	if (debug) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(App->win->window);
		ImGui::NewFrame();
	}
	
	return true;
}

bool ctRender::Update(float dt)
{

	uint winWidth, winHeight;

	App->win->GetWindowSize(winWidth, winHeight);

	/*
	int speed = 3;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		camera.y += speed;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		camera.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		camera.x += speed;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		camera.x -= speed;*/

	//LOG("Camera pos x: %i pos y: %i", camera.x, camera.y);

	if (debug) {
		ImGui::Text("Hello, world %d", 123);
	}
	

	return true;
}

bool ctRender::PostUpdate()
{
	if (!debug) {
		//SDL_SetRenderDrawColor(renderer, 255, background.g, background.g, background.a);
		SDL_RenderPresent(renderer);
	}
	
	if (debug) {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//SWAP BUFFERS
		SDL_GL_SwapWindow(App->win->window);
	}

	
	if (EzAcc_GetKey(SDL_SCANCODE_F1) == EzAcc_KeyState::EZACC_KEY_DOWN) {
		debug = !debug;
	}
	return true;
}

// Called before quitting
bool ctRender::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool ctRender::Load(pugi::xml_node& data)
{
	return true;
}

// Save Game State
bool ctRender::Save(pugi::xml_node& data) const
{

	return true;
}

void ctRender::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void ctRender::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void ctRender::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool ctRender::Blit(SDL_BlendMode blendMode, SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	uint scale = App->win->GetScale();
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	//SDL_SetTextureColorMod(texture, 255, 64, 64); //TODOG OJO
	if (blendMode != SDL_BLENDMODE_INVALID) {
		SDL_SetTextureBlendMode(texture, blendMode);
		SDL_SetSurfaceBlendMode(App->ken_stage_scene->backgroundSurface, blendMode);
		SDL_SetRenderDrawBlendMode(renderer, blendMode);
	}
	/*SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetSurfaceBlendMode(App->ken_stage_scene->backgroundSurface, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);*/
	//SDL_SetTextureAlphaMod(texture, 255);
	/*SDL_Rect blur;
	blur.x = 600;
	blur.y = 0;
	blur.w = 100;
	blur.h = 100;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 80);
	SDL_RenderFillRect(renderer, &blur);*/

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;
	
	void* mPixels;
	int mPitch;
	if (App->ken_stage_scene->atlas_tex == texture) {
		//Lock texture for manipulation
		//Texture is already locked
	
			if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
			{
				LOG("Unable to lock texture! %s\n", SDL_GetError());
			}
		

		//Copy loaded/formatted surface pixels
		memcpy(mPixels, App->ken_stage_scene->backgroundSurface->pixels, App->ken_stage_scene->backgroundSurface->pitch * App->ken_stage_scene->backgroundSurface->h);

		//Allocate format from window
		Uint32 format = SDL_PIXELFORMAT_RGBA8888; // SDL_GetWindowPixelFormat(App->win->window);
		SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

		//Get pixel data
		Uint32* pixels = (Uint32*)mPixels;
		int pixelCount = (mPitch / 4) *  rect.h; //TODOG puede que App->ken_stage_scene->backgroundSurface->h

		Uint32 colorKey = SDL_MapRGBA(mappingFormat,0, 0, 0,255);
		Uint32 transparent = SDL_MapRGBA(mappingFormat, 0, 0, 255, 50);

		//Color key pixels
		/*for (int i = 0; i < pixelCount; ++i)
		{
			if (pixels[i] == colorKey)
			{
				pixels[i] = transparent;
			}

		}*/


		//Unlock texture to update
		SDL_UnlockTexture(texture);
		mPixels = NULL;
		SDL_FreeFormat(mappingFormat);
	}

	

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}
	//if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ctRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ctRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();
	scale = 1;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ctRender::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

iPoint ctRender::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}