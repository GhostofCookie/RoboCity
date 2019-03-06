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

private:
	struct GL_Colour* _material;
	class FBXLoader* _mesh;
};

class GLMeshComponent
{
public:
	GLMeshComponent();
	~GLMeshComponent() { delete _mesh; }

	void Tick();

	void SetMesh(GLMesh*);

	GLMesh* GetMesh() const;

private:
	GLMesh* _mesh;
};