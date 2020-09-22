#include "Evt_Mouse.h"

static MouseListener::MouseLayerNode* layerEnds[MouseLayer_Front][2] = { nullptr };

static bool buttons[3];

MouseListener::MouseListener() {
	myNode.obj = this;
	myNode.prev = nullptr;
	myNode.next = nullptr;
	layer = MouseLayer_None;
	setMouseLayer(MouseLayer_Back);
}

MouseListener::~MouseListener() {
	removeMouseLayer();
}

void MouseListener::MouseInFrontOf(MouseListener& target) {

	if (target.getMouseLayer() == MouseLayer_None) {
		return;
	}

	removeMouseLayer();
	MouseLayerNode& tNode = target.myNode;
	myNode.prev = tNode.prev;
	myNode.next = &tNode;
	if (tNode.prev) {
		tNode.prev->next = &myNode;
	}
	else {
		layerEnds[(int)target.getMouseLayer() - 1][0] = &myNode;
	}
	tNode.prev = &myNode;

}

void MouseListener::setMouseLayer(MouseLayer mLayer) {

	if (mLayer == layer) {
		return;
	}

	removeMouseLayer();
	layer = mLayer;

	/* Append to new list as head */
	if (layer != MouseLayer_None) {
		int index = (int)layer - 1;
		myNode.next = layerEnds[index][0];
		myNode.prev = nullptr;
		if (layerEnds[index][0] != nullptr) {
			layerEnds[index][0]->prev = &myNode;
		}
		else {
			/* Set tail if previously empty list */
			layerEnds[index][1] = &myNode;
		}
		layerEnds[index][0] = &myNode;
	}

}

void MouseListener::removeMouseLayer() {

	/* Remove from previous list */
	if (layer != MouseLayer_None) {
		if (myNode.prev) {
			myNode.prev->next = myNode.next;
		}
		else {
			/* If no prev, we must be head */
			layerEnds[(int)layer - 1][0] = myNode.next;
		}
		if (myNode.next) {
			myNode.next->prev = myNode.prev;
		}
		else {
			/* If no next, we must be tail */
			layerEnds[(int)layer - 1][1] = myNode.prev;
		}
	}

	layer = MouseLayer_None;

}

void Evt_Mouse::sendMouseMotion(double xrel, double yrel) {
	for (int i = (int)MouseLayer_Front - 1; i >= 0; i--) {
		MouseListener::MouseLayerNode* node = layerEnds[i][0];
		bool blocked = false;
		while (node && !blocked) {
			blocked = node->obj->onMouseMotion(xrel, yrel);
			node = node->next;
		}
		if (blocked) {
			break;
		}
	}
}

void Evt_Mouse::sendMousePress(int button, int x, int y) {
	buttons[button] = true;
	for (int i = (int)MouseLayer_Front - 1; i >= 0; i--) {
		MouseListener::MouseLayerNode* node = layerEnds[i][0];
		bool blocked = false;
		while (node && !blocked) {
			blocked = node->obj->onMousePress(button, x, y);
			node = node->next;
		}
		if (blocked) {
			break;
		}
	}
}

void Evt_Mouse::sendMouseRelease(int button, int x, int y) {
	buttons[button] = false;
	for (int i = (int)MouseLayer_Front - 1; i >= 0; i--) {
		MouseListener::MouseLayerNode* node = layerEnds[i][0];
		bool blocked = false;
		while (node && !blocked) {
			blocked = node->obj->onMouseRelease(button, x, y);
			node = node->next;
		}
		if (blocked) {
			break;
		}
	}
}

void Evt_Mouse::sendMouseWheel(double amount) {
	for (int i = (int)MouseLayer_Front - 1; i >= 0; i--) {
		MouseListener::MouseLayerNode* node = layerEnds[i][0];
		bool blocked = false;
		while (node && !blocked) {
			blocked = node->obj->onMouseWheel(amount);
			node = node->next;
		}
		if (blocked) {
			break;
		}
	}
}

bool Evt_Mouse::isButtonHeld(int button) {
	return buttons[button % 3];
}