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


	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.ChildRounding = 3.f;
		style.GrabRounding = 0.f;
		style.WindowRounding = 4.f;
		style.ScrollbarRounding = 3.f;
		style.FrameRounding = 3.f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style.Colors[ImGuiCol_Text] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	}
	debug = true;
	show_about = true;

	macroTest.key = -1;

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
		//ImGui::Text("Hello, world %d", 123);
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save Settings"))
					return true; //App->Save();
				if (ImGui::MenuItem("Load Settings"))
					return true; //App->Load();
				if (ImGui::MenuItem("Quit", "Alt+F4"))
					return false;

				ImGui::EndMenu();
			}
		
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Motor (Mobility/Control)", "1"))
					show_input = !show_input;

				if (ImGui::MenuItem("Cognitive (Memory/Information Processing)", "2"))
					show_cognitive = !show_cognitive;

				if (ImGui::MenuItem("Vision", "3"))
					show_vision = !show_vision;

				if (ImGui::MenuItem("Hearing", "4"))
					show_hearing = !show_hearing;
			
				if (ImGui::MenuItem("Show/Hide UI Configuration", "F1"))
					change_debug = true;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Gui Demo"))
					show_demo_window = true;

				if (ImGui::MenuItem("Documentation"))
					App->RequestBrowser("https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework/");

				if (ImGui::MenuItem("Download latest"))
					App->RequestBrowser("https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework/releases");
			
				if (ImGui::MenuItem("Report a bug"))
					App->RequestBrowser("https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework/issues");

				if (ImGui::MenuItem("About"))
					show_about = !show_about;

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (show_input) {
			DrawInput();
		}
		if (show_cognitive) {
			DrawCognitive();
		}
		if (show_vision) {
			DrawVision();
		}
		if (show_hearing) {
			DrawHearing();
		}
		if (show_about) {
			DrawAbout();
		}
	} // end debug

	

	return true;
}

void ctRender::DrawHearing() // TODOG
{
	ImGui::Begin("Hearing Settings", &show_hearing, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::End();
}

void ctRender::DrawVision() // TODOG
{
	ImGui::Begin("Vision Settings", &show_vision, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Read EzAcc_Vision settings in the readme to learn more about all the function calls");
	ImGui::SameLine();
	if (ImGui::Button("Readme##READMEInput3"))
		App->RequestBrowser("https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework/blob/master/README.md");

	
	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "EzAcc color picker settings");
	static ImVec4 colorTarget = ImColor(250, 0, 0, 255);
	static ImVec4 colorReplacement = ImColor(0, 0, 250, 255);
	static ImVec4 colorModulation = ImColor(114, 255, 50, 255);

	static bool alpha_preview = true;
	static bool alpha_half_preview = false;
	static bool drag_and_drop = true;
	static bool options_menu = true;
	ImGui::Checkbox("With Alpha Preview", &alpha_preview);
	ImGui::Checkbox("With Half Alpha Preview", &alpha_half_preview);
	ImGui::Checkbox("With Drag and Drop", &drag_and_drop);
	ImGui::Checkbox("With Options Menu", &options_menu);
	int misc_flags = (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
	ImGui::Separator();

	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "EzAcc pixel color replacement");
	ImGui::Text("Color target 01:");
	ImGui::SameLine(); ShowHelpMarker("Click on the colored square to open a color picker.\n");
	ImGui::ColorEdit4("MyColor##1", (float*)&colorTarget, ImGuiColorEditFlags_RGB| misc_flags);

	ImGui::Text("Color replacement 01:");
	ImGui::SameLine(); ShowHelpMarker("Click on the colored square to open a color picker.\n");
	ImGui::ColorEdit4("MyColor##2", (float*)&colorReplacement, ImGuiColorEditFlags_RGB | misc_flags);

	ImGui::Separator();

	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "EzAcc modulate texture color");
	ImGui::Text("Modulation color:");
	ImGui::SameLine(); ShowHelpMarker("Click on the colored square to open a color picker.\n");
	ImGui::ColorEdit4("MyColor##3", (float*)&colorModulation, ImGuiColorEditFlags_RGB | misc_flags);
	ImGui::Text("Time:");
	ImGui::SameLine();
	static int time_between_inputs = 0;
	if (ImGui::SliderInt("##TIMEBETWEENINPUTS2", &time_between_inputs, 0, 5000))
		EzAcc_SetTimeBetweenInputs(time_between_inputs);
	ImGui::SameLine();
	ImGui::Text("ms");

	ImGui::Separator();

	ImGui::End();
}

void ctRender::ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void ctRender::DrawCognitive() // TODOG
{
	ImGui::Begin("Cognitive (Memory/Information Processing) Settings", &show_cognitive, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Read EzAcc_Core settings in the readme to learn more about all the function calls");
	ImGui::SameLine();
	if (ImGui::Button("Readme##READMEInput"))
		App->RequestBrowser("https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework/blob/master/README.md");
	
	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "EzAcc framework clocks");
	ImGui::Text("Current DT value: %.2f", App->dt);
	ImGui::Separator();
	ImGui::Text("Real time clock: %.2f sec", EzAcc_GetRealtime());
	ImGui::Text("Real time clock (hour format): %02d : %02d : %02d",
		EzAcc_GetRealtimeHourFormat().hours,
		EzAcc_GetRealtimeHourFormat().min,
		EzAcc_GetRealtimeHourFormat().sec);

	ImGui::Separator();

	ImGui::Text("Game clock: %.2f sec", EzAcc_GetGameTime());
	ImGui::Text("Game clock (hour format): %02d : %02d : %02d",
		EzAcc_GetGameTimeHourFormat().hours,
		EzAcc_GetGameTimeHourFormat().min,
		EzAcc_GetGameTimeHourFormat().sec);

	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "Set time scale for game clock DT");
	ImGui::Text("Time scale:");
	ImGui::SameLine();
	static float dt_scale = 1.0f;
	if (ImGui::SliderFloat("##TIMEBETWEENINPUTS", &dt_scale, 0.0f, 2.0f))
		EzAcc_SetDTTimeScale(dt_scale);

	ImGui::End();
}

void ctRender::DrawInput() // TODOG
{
	ImGui::Begin("Motor (Mobility/Control) Settings", &show_input, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Read EzAcc_InputModule settings in the readme to learn more about all the function calls");
	ImGui::SameLine();
	if (ImGui::Button("Readme##READMEInput"))
		App->RequestBrowser("https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework/blob/master/README.md");
	ImGui::Separator();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("TODOG : GET KEY INFO WHAT RETURNS ETC ETC");
	ImGui::Separator();

	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "Basic input info");
	if (ImGui::IsMousePosValid())
		ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
	else
		ImGui::Text("Mouse pos: <INVALID>");
	ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
	ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
	ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
	ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
	ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
	ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

	ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (%.02f secs)", i, io.KeysDownDuration[i]); }


	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "Set time between inputs");
	ImGui::Text("Time between inputs:");
	ImGui::SameLine();
	static int time_between_inputs = 0;
	if (ImGui::SliderInt("##TIMEBETWEENINPUTS", &time_between_inputs, 0, 5000))
		EzAcc_SetTimeBetweenInputs(time_between_inputs);
	ImGui::SameLine();
	ImGui::Text("ms");

	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "Bind new key");
	if (!binding) {
		if (lastBindedKey != -1) {
			ImGui::Text("Last binded key ID was: %i", lastBindedKey);
			ImGui::SameLine();
		}
		if (ImGui::Button("Press to bind new key##Bind")) {
			binding = true;
		}
	}
	else {
		ImGui::Text("Press any key to return the binding code...");
		lastBindedKey = EzAcc_BindKey();
		binding = false;
	}

	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "Generate Macros");
	if (!bindingMacroKey) {
		if (macroTest.key == -1) {
			ImGui::Text("Choose the key for the new macro");
			ImGui::SameLine();
			if (ImGui::Button("New Key##Bw")) {
				bindingMacroKey = true;
			}
		}
		else {
			ImGui::Text("Key ID choosed: %i",macroTest.key);
			ImGui::SameLine();
			if (ImGui::Button("Choose another##Be")) {
				bindingMacroKey = true;
			}
		}
		
	}
	else {
		ImGui::Text("Choose the key for the new macro");
		ImGui::SameLine();
		ImGui::Text("Press any key to return the binding code...");
		macroTest.key = EzAcc_BindKey();
		bindingMacroKey = false;
	}

	if (macroTest.key != -1 && macroTest.key_effects.size() == 0) {
		if (!bindingMacroKeyValue1) {
			ImGui::Text("Choose first key VALUE for the new macro");
			ImGui::SameLine();
			if (ImGui::Button("New Key##B2w")) {
				bindingMacroKeyValue1 = true;
			}
		}
		else {
			ImGui::Text("Choose the key for the first value");
			ImGui::SameLine();
			ImGui::Text("Press any key to return the binding code...");
			macroTest.key_effects.push_back(EzAcc_BindKey());
			bindingMacroKeyValue1 = false;
		}
	}
	else if (macroTest.key != -1) {
		ImGui::Text("Effect key ID choosed: %i", macroTest.key_effects[0]);
	}

	if (macroTest.key != -1 && macroTest.key_effects.size() == 1) {
		if (!bindingMacroKeyValue2) {
			ImGui::Text("Choose second key VALUE for the new macro");
			ImGui::SameLine();
			if (ImGui::Button("New Key##B32w")) {
				bindingMacroKeyValue2 = true;
			}
		}
		else {
			ImGui::Text("Choose the key for the second value");
			ImGui::SameLine();
			ImGui::Text("Press any key to return the binding code...");
			macroTest.key_effects.push_back(EzAcc_BindKey());
			bindingMacroKeyValue2 = false;
		}
	}
	else if (macroTest.key != -1 && macroTest.key_effects.size() > 1){
		ImGui::Text("Effect key ID choosed: %i", macroTest.key_effects[1]);
	}
	if (ImGui::Button("Reset macro##qweqwe")) {
		macroTest.key = -1;
		macroTest.key_effects.clear();
	}

	ImGui::End();
}

void ctRender::DrawAbout() // TODOG
{
	ImGui::Begin("About Easy Accessibility Framework - EzAcc", &show_about, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Version 1.0");
	ImGui::SameLine();
	if (ImGui::Button("Github##EZACC"))
		App->RequestBrowser("https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework/");
	ImGui::Separator();
	ImGui::Text("By Guillermo Garcia Subirana");

	if (ImGui::CollapsingHeader("Tools and libraries"))
	{
		ImGui::Text("Programming language: C++");

		ImGui::Text("IDE: Microsoft Visual Studio 2017");
		ImGui::SameLine();
		if (ImGui::Button("Web page"))
			App->RequestBrowser("https://visualstudio.microsoft.com/?rr=https%3A%2F%2Fwww.google.es%2F");

		ImGui::Text("Input and audio: SDL v2.0");
		ImGui::SameLine();
		if (ImGui::Button("Web page##Second"))
			App->RequestBrowser("https://www.libsdl.org/index.php");

		ImGui::Text("Graphics: OpenGL version supported: %s", glGetString(GL_VERSION));
		ImGui::SameLine();
		if (ImGui::Button("Web page##Third"))
			App->RequestBrowser("https://www.opengl.org/");

		ImGui::Text("Extension loading library: Glew v%s", (char*)glewGetString(GLEW_VERSION));
		ImGui::SameLine();
		if (ImGui::Button("Web page##Fourth"))
			App->RequestBrowser("http://glew.sourceforge.net/");

		ImGui::Text("GUI: Dear ImGui v%s", ImGui::GetVersion());
		ImGui::SameLine();
		if (ImGui::Button("Github##Second"))
			App->RequestBrowser("https://github.com/ocornut/imgui");

		ImGui::Text("Image loader: Developer's Image Library (DevIL) v%i.%i.%i", 1, 8, 0);
		ImGui::SameLine();
		if (ImGui::Button("Web page##Seventh"))
			App->RequestBrowser("http://openil.sourceforge.net/");

		ImGui::Text("JSON parser: Parson v2017");
		ImGui::SameLine();
		if (ImGui::Button("Github##Third"))
			App->RequestBrowser("https://github.com/kgabis/parson");

		ImGui::Text("Profiler: Brofiler v1.1.2");
		ImGui::SameLine();
		if (ImGui::Button("Web page##Eigth"))
			App->RequestBrowser("http://www.brofiler.com/");

	}

	if (ImGui::CollapsingHeader("License"))
	{
		ImGui::Text("MIT License");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
		ImGui::Text("of this software and associated documentation files(the ""Software""), to deal");
		ImGui::Text("in the Software without restriction, including without limitation the rights");
		ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
		ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
		ImGui::Text("furnished to do so, subject to the following conditions :\n\n");

		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "The above copyright notice and this permission notice shall be included in all");
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "copies or substantial portions of the Software.\n\n");

		ImGui::Text("THE SOFTWARE IS PROVIDED ""AS IS"", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
		ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, ");
		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
		ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
		ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
		ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
		ImGui::Text("SOFTWARE.");
	}

	ImGui::End();
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

	
	if (EzAcc_GetKey(SDL_SCANCODE_F1) == EzAcc_KeyState::EZACC_KEY_DOWN || change_debug) {
		change_debug = false;
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
bool ctRender::Blit(SDL_BlendMode blendMode, SDL_Surface* surface, SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	uint scale = App->win->GetScale();
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	//SDL_SetTextureColorMod(texture, 500, 500, 200); //TODOG OJO
	if (blendMode != SDL_BLENDMODE_INVALID) {
		SDL_SetTextureBlendMode(texture, blendMode);
		SDL_SetSurfaceBlendMode(surface, blendMode);
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

	SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
	//Lock texture for manipulation
	//Texture is already locked
	/*
	if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
	{
		LOG("Unable to lock texture! %s\n", SDL_GetError());
	}
		

	//Copy loaded/formatted surface pixels
	memcpy(mPixels, surface->pixels, surface->pitch * surface->h);
	
	//Allocate format from window
	Uint32 format = SDL_PIXELFORMAT_ARGB8888; // SDL_GetWindowPixelFormat(App->win->window);
	SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

	//Get pixel data
	Uint32* pixels = (Uint32*)mPixels;
	int pixelCount = (mPitch / 4) * surface->h; //TODOG puede que App->ken_stage_scene->backgroundSurface->h

	Uint32 colorKey = SDL_MapRGBA(mappingFormat,248, 0, 0,255);
	Uint32 colorKey2 = SDL_MapRGBA(mappingFormat, 184, 0, 0, 255);
	Uint32 transparent = SDL_MapRGBA(mappingFormat, 0, 0, 248, 255);
	Uint32 transparent2 = SDL_MapRGBA(mappingFormat, 0, 0, 184, 255);

	//Color key pixels
	for (int i = 0; i < pixelCount; ++i)
	{
		Uint32 pixel = pixels[i];
		SDL_Color rgb;
		SDL_GetRGBA(pixels[i], mappingFormat, &rgb.r, &rgb.g, &rgb.b, &rgb.a);
			
		if (pixels[i] == colorKey)
		{
			pixels[i] = transparent;
		}
		if (pixels[i] == colorKey2)
		{
			pixels[i] = transparent2;
		}

	}
	
	//Unlock texture to update
	SDL_UnlockTexture(texture);
	mPixels = NULL;
	SDL_FreeFormat(mappingFormat);
	*/


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

