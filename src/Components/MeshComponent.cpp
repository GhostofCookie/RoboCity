#include <Components/MeshComponent.h>
#include <Core.h>
#include <Loader.h>

GLMesh::GLMesh()
   :_material{ new GL_Colour(GL_Colour::White) }
{
   _mesh = new FBXLoader();
}

GLMesh::GLMesh(const char * file)
   :_material{ new GL_Colour(GL_Colour::White) }
{
   _mesh = new FBXLoader(file);
}

GLMesh::GLMesh(const GLMesh& m)
   : _mesh{ m._mesh }, _material{ m._material } {}

GLMesh::GLMesh(GLMesh * m)
   : _mesh{ m->_mesh }, _material{ m->_material } {}

GLMesh::~GLMesh() { delete _mesh; delete _material;}

void GLMesh::Tick()
{
   if(_material) _material->UseColour();
   if(_mesh) _mesh->Render();
}

void GLMesh::SetMaterial(const GL_Colour& m)
{
   if(_material)
   {
      delete _material;
      _material = nullptr;
   }
   _material = new GL_Colour(m);
}



GLMeshComponent::GLMeshComponent()
{
}

void GLMeshComponent::Tick()
{
   if (_mesh)
   {
      StartRender();
      _mesh->Tick();
      EndRender();
   }
}

void GLMeshComponent::SetMesh(GLMesh* m)
{
   _mesh = m;
}

GLMesh* GLMeshComponent::GetMesh() const
{
   return _mesh;
}

void GLMeshComponent::SetMaterial(const GL_Colour& m)
{
   if(_mesh) _mesh->SetMaterial(m);
}
