#pragma once

#include "Texture.h"
#include "Evt_Display.h"
#include "Evt_Mouse.h"
#include "AssetEnums.h"

class Image : public DisplayListener, public MouseListener {
public:

	Image(TEXTURES = TEXTURE_NONE);

	void setRelPos(float x, float y);
	void setRelPos(glm::vec2);
	void setAbsPos(float x, float y);
	void setAbsPos(glm::vec2);
	glm::vec2 getRelPos() { return relPos; }
	glm::vec2 getAbsPos() { return absPos; }
	/* Screen position accounting for both relative and absolute */
	glm::vec2 getTruePos();

	void setRelSize(float x, float y);
	void setRelSize(glm::vec2);
	void setAbsSize(float x, float y);
	void setAbsSize(glm::vec2);
	glm::vec2 getRelSize() { return relSz; }
	glm::vec2 getAbsSize() { return absSz; }
	/* Screen size accounting for both relative and absolute */
	glm::vec2 getTrueSize();

	void setRelOffset(float x, float y);
	void setRelOffset(glm::vec2);
	
	void setTexture(TEXTURES);
	Texture getTexture() { return tex; }

	void setTint(float r, float g, float b);
	void setTint(glm::vec3);
	void setVisibility(bool);
	bool getVisibility() { return visible; }
	void setTiled(bool);
	bool getTiled() { return tiled; }
	glm::vec3 getTint() { return tint; }

protected:

	Texture tex;

private:

	void onDrawGUI(GBuffer&) override;
	void onResize(int, int) override;
	bool onMouseRelease(int, int, int) override;
	bool onMousePress(int, int, int) override;

	/* Calculate trans values */
	void calcTrans();
	/* Calculate texture coords */
	void calcTex();
	/* If trans values changed, calculate them */
	void checkChanged();
	bool transChanged = false;
	bool texChanged = false;
	bool visible = true;

	Mesh mesh;
	Transform trans;

	glm::vec2 relPos = glm::vec2(0,0);
	glm::vec2 absPos = glm::vec2(0,0);

	glm::vec2 relSz = glm::vec2(1, 1);
	glm::vec2 absSz = glm::vec2(0,0);

	glm::vec2 relOffs = glm::vec2(0, 0);

	glm::vec3 tint = glm::vec3(1, 1, 1);
	bool tinted = false;
	bool tiled = false;

};