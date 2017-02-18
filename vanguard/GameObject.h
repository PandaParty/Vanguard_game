#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Component;
class GameObject
{
protected:
	std::vector<Component*> components;
public:
	glm::vec2 position;
	bool enabled;
	std::string name;

	virtual ~GameObject();

	virtual void Create();

	virtual void Init();
	virtual void Update(float elapsed);
	virtual void Destroy();
};
