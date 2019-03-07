#include <Components/MeshComponent.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <Loader.h>

#include <Core.h>

GLMesh::GLMesh()
   :_material{ new GL_Colour(GL_Colour::White) }
{
   _mesh = new FBXLoader();
}

GLMesh::GLMesh(const char * file)
{
   _mesh = new FBXLoader(file);
}

GLMesh::GLMesh(const GLMesh& m)
   : _mesh{ m._mesh }, _material{ m._material } {}

GLMesh::GLMesh(GLMesh * m)
   : _mesh{ m->_mesh }, _material{ m->_material } {}

void GLMesh::Tick()
{
   if(_mesh) _mesh->Render();
}



GLMeshComponent::GLMeshComponent()
{
}

void GLMeshComponent::Tick()
{
   if (_mesh) _mesh->Tick();
}

void GLMeshComponent::SetMesh(GLMesh* m)
{
   _mesh = m;
}

GLMesh* GLMeshComponent::GetMesh() const
{
   return _mesh;
}
