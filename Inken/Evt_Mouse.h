#pragma once

enum MouseLayer {
	MouseLayer_None = 0,
	MouseLayer_Back,
	MouseLayer_Middle,
	MouseLayer_Front
};

class MouseListener {
public:

	MouseListener();
	~MouseListener();

	void setMouseLayer(MouseLayer);
	MouseLayer getMouseLayer() { return layer; }
	void MouseInFrontOf(MouseListener&);

	struct MouseLayerNode {
		MouseListener* obj;
		MouseLayerNode* prev;
		MouseLayerNode* next;
	};

protected:

	virtual bool onMouseMotion(double x, double y) { return false; }
	virtual bool onMousePress(int button, int x, int y) { return false; }
	virtual bool onMouseRelease(int button, int x, int y) { return false; }
	virtual bool onMouseWheel(double amount) { return false; }

	friend class Evt_Mouse;

private:

	MouseLayerNode myNode;
	MouseLayer layer;
	/* Remove self from layer */
	void removeMouseLayer();

};

class Evt_Mouse {
public:

	static void sendMouseMotion(double xrel, double yrel);
	static void sendMousePress(int button, int x, int y);
	static void sendMouseRelease(int button, int x, int y);
	static void sendMouseWheel(double amount);

	static bool isButtonHeld(int button);
};