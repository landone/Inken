#include "Model.h"
#include "Assets.h"

Model::Model() {



}

void Model::setModel(MODELS type) {

	this->type = type;
	Assets::mesh(type, mesh);

}

void Model::setVisible(bool toggle) {

	visible = toggle;

}

void Model::onDrawGeometry(GBuffer& gbuf) {

	if (visible) {
		Transform& trans = getTransform();
		gbuf.setTransMat(trans.GetMatrix());
		gbuf.setRotMat(trans.GetRotMatrix());
		mesh.draw();
	}

}