#pragma once
class GameObject;
class Component
{
protected:
	GameObject * gameObject;

public:
	virtual ~Component() {}

	virtual void Create(GameObject * parent);

	virtual void Init() {}
	virtual void Update(float dt) = 0;
	virtual void Destroy() {}
};