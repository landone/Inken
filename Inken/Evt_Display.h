#pragma once

#include "GBuffer.h"

enum GUILayer {
	GUILayer_None = 0,
	GUILayer_Back,
	GUILayer_Middle,
	GUILayer_Front
};

class DisplayListener {
public:

	DisplayListener();
	DisplayListener(const DisplayListener&);
	~DisplayListener();

	void setGUILayer(GUILayer);
	GUILayer getGUILayer() { return guiLayer; }
	void GUIInFrontOf(DisplayListener&);

	struct GUILayerNode {
		DisplayListener* obj;
		GUILayerNode* prev;
		GUILayerNode* next;
	};

protected:

	virtual void onFrame(double delta) {}
	virtual void onDrawGUI(GBuffer&) {}
	virtual void onDraw3DGUI(GBuffer&) {}
	virtual void onDrawGeometry(GBuffer&) {}
	virtual void onResize(int, int) {}

	friend class Evt_Display;

private:

	GUILayerNode myNode;
	GUILayer guiLayer = GUILayer_None;
	/* Remove self from layer */
	void removeGUILayer();

};

class Evt_Display {
public:

	static void sendFrame();
	static void sendDrawGUI(GBuffer&);
	static void sendDrawGeometry(GBuffer&);
	static void sendDraw3DGUI(GBuffer&);
	static void sendResize(int x, int y);

private:

	static long long lastFrame;
	static long long thisFrame;

};