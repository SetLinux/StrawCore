#include "../Math/Math.hpp"
#include "../Utils/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION	// Use GL3 implementation.
#include "../Utils/nanovg_gl.h"
#include <GLFW/glfw3.h>
#include <string>
class GUISystem {
public:
	NVGcontext* vg;
	int winWidth, winHeight;
	int fbWidth, fbHeight;
	float pxRatio;
	float mousex, mousey;
	
	GUISystem(GLFWwindow* window) {
		vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_DEBUG | NVG_STENCIL_STROKES);
		//Just Getting Some Values for the NanoVG Rendering :)
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		// Calculate pixel ratio for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;
	}
	~GUISystem() {
		
	}
	// firstly you call begin() and End() around Your GUI Calls

	//Must Be Called before rendering
	void Begin() {
		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
	}
		 	
	//Don't forget to call this after finishing your GUI Calls
	void End() {
		nvgEndFrame(vg);
	}

	//Draw a rectangle 
	//I guess simple enough
	void DrawRect(XVector pos, XVector scale, XVector color, float rotation = 0) {
		nvgBeginPath(vg);
		nvgTranslate(vg, pos.x, pos.y);
		nvgRotate(vg, Radians(rotation));
		nvgRect(vg, -scale.x/2, -scale.y/2 ,scale.x, scale.y);
		nvgFillColor(vg, nvgRGBA(color.x, color.y, color.z, 255));
		nvgFill(vg);
		nvgTranslate(vg, -pos.x, -pos.y);
		nvgRotate(vg, -Radians(rotation));
	}
	//A Simple TextDrawing Function it looks a bit complicated but trust me itsn't at all 
	void DrawText(const XVector& pos, const XVector& color,const float& size ,const std::string& text, const std::string& fontname = "sans",const float& rotation =0) {
		nvgFontSize(vg, size);
		nvgFontFace(vg, fontname.c_str());
		nvgTextLetterSpacing(vg, 0.f);
		nvgTranslate(vg, pos.x, pos.y);
		nvgFillColor(vg, nvgRGBA(color.x, color.y, color.y, 255));
		nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
		nvgRotate(vg, Radians(rotation));
		nvgText(vg, 0, 0, text.c_str(), NULL);
		nvgTranslate(vg, -pos.x, -pos.y);
		nvgRotate(vg, -Radians(rotation));
	}
	bool DrawButton(const XVector& pos, const XVector& scale, const XVector& color ,float rotation = 0) {
		DrawRect(pos, scale, color, rotation);	
		XVector postotest = XVector(pos.x ,fbHeight -  (pos.y )); // the subtraction is because the orthographic  projection starts from bottom left while NanoVG Starts from top left
		return isPointInsideRect(XVector(mousex, mousey), postotest, scale, rotation);
	}
};
