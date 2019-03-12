#pragma once

#include <Object.h>

class GLMesh
{
public:
	GLMesh();
	GLMesh(const char*);
	GLMesh(const GLMesh&);
	GLMesh(GLMesh*);

	void Tick();

	void SetMaterial(const struct GL_Colour&);

private:
	GL_Colour* _material;
	class FBXLoader* _mesh;
};

class GLMeshComponent : public Object
{
public:
	GLMeshComponent();
	~GLMeshComponent() { delete _mesh; }

	void Tick();

	void SetMesh(GLMesh*);

	GLMesh* GetMesh() const;
	
	void SetMaterial(const struct GL_Colour&);

  private:
	GLMesh* _mesh;
};
