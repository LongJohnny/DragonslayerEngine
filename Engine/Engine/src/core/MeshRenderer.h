#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Renderer.h"

namespace core {

	class MeshRenderer : public Renderer {
	
	public:
		MeshRenderer();

		void onGUI() override;
	};

};

#endif
