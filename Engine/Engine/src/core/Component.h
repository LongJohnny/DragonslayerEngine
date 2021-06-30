#ifndef COMPONENT_H
#define COMPONENT_H

#include "../gui/GUI.h"
#include <string>

namespace core {

	class GameObject; // Forward declaration of Component due to cross referencing between Component and GameObject

	/* Base class for all components. A component can be added to a GameObject.
	* Examples of components include: RigidBody, MeshRenderer etc..
	*/
	class Component {

	public:
		virtual bool getType() const = 0;
		std::string getName() const { return name; }

		/*! @brief Associates GameObject with this components
		* if the component already has a GameObject associated
		* then return false else return true
		*/
		bool setGameObject(GameObject* gameObject);

		GameObject* getGameObject();

		/* Called by the GUI to display the component */
		virtual void onGUI() = 0;

	protected:
		Component(const std::string& name);

		GameObject* gameObject = nullptr;

	private:
		std::string name;

		/* Called by the scene graph to update the component every frame */
		virtual void update() = 0;

		friend class Hierarchy;
		friend class GUI;

	};

};

#include "GameObject.h"

#endif 


