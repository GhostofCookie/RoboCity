#pragma once

#include <Object.h>

class Building : public Object
{
public:
	Building();
	~Building();

	bool CanTakeDamage() const;

	void TakeDamage(float Delta);

private:
	float _health, _max_health;
	class GLMeshComponent* _mesh;
	struct GL_Colour* _material;

};