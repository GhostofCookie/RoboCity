#include <Building.h>
#include <Core.h>

#include <cmath>

Building::Building()
	: _material{ new GL_Colour(GL_Colour::White) },
	_max_health{ 100.f },
	_health{ _max_health }
{
}

Building::~Building()
{
	if(_mesh) delete _mesh;
	if(_material) delete _material;
}

bool Building::CanTakeDamage() const
{
	return _health > 0.f || _max_health > -1;
}

void Building::TakeDamage(float Delta)
{
	if (CanTakeDamage())
		_health = std::fmax(_health - Delta, 0.f);
}
