#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <assert.h>
#include <list>
#include "../math/Mat4.h"

namespace core {

	class GameObject;

	class Hierarchy {

	private:
		std::list<GameObject*> rootGameObjects;
		static Hierarchy* instance;

		bool addRoot(GameObject* gameObject);
		bool removeRoot(GameObject* gameObject);

		void updateSceneRecursively(GameObject* gameObject, const Mat4& parentModel = Mat4::IDENTITY);

		friend class GameObject; // Transform is a friend class to have access to addRoot and removeRoot

	public:
		static Hierarchy& createHierarchy();
		static Hierarchy& getHierarchy();

		std::list<GameObject*> getRootGameObjects() const;

		void updateScene();
	};

}

#include "GameObject.h"

#endif