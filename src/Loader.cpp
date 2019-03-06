#include <Loader.h>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fbxsdk.h>
#include <App.h>
#include <Core.h>
#include <string>

FBXLoader::FBXLoader(const char * file)
{
	LoadFBX(file);
}

FBXLoader::FBXLoader(FBXLoader & fbxl)
	: mesh_scene{ fbxl.mesh_scene }
{
}

FBXLoader::FBXLoader(FBXLoader * fbxl)
	: mesh_scene{ fbxl->mesh_scene }
{
}

void FBXLoader::LoadFBX(const char * file)
{
	FbxManager* manager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string tmp(buff);
	tmp.append("/../../");
	tmp.append(file);
	FbxImporter* importer = FbxImporter::Create(manager, "");
	bool bIsInit = importer->Initialize(tmp.c_str(), -1, manager->GetIOSettings());

	if (!bIsInit)
	{
		std::string str = "Failed to import \"";
		str.append(tmp);
		str.append("\" from current directory");
		App::PrintToScreen(str.c_str(), -App::Width(), App::Height()-30, GL_Colour::Red);
		return;
	}

	FbxScene* scene = FbxScene::Create(manager, "temp_scene");
	if (importer->Import(scene))
	{
		mesh_scene = scene;
		importer->Destroy();
	}
}

void FBXLoader::Render()
{
	FbxAMatrix dummy_pos;
	if(mesh_scene) 
		if(mesh_scene->GetRootNode())
			DrawMeshRecursive(mesh_scene->GetRootNode(), dummy_pos);
}

void FBXLoader::DrawMesh(FbxNode * node, FbxAMatrix & parentGlobalPos)
{
	FbxMesh* mesh = node->GetMesh();
	const int vertex_count = mesh->GetControlPointsCount();

	if (vertex_count == 0) return;

	FbxVector4* vertices = NULL;
	vertices = new FbxVector4[vertex_count];
	memcpy(vertices, mesh->GetControlPoints(), vertex_count * sizeof(FbxVector4));

	glPushMatrix();
	glMultMatrixd((const double*)parentGlobalPos);

	const int poly_count = mesh->GetPolygonCount();
	for (int i = 0; i < poly_count; i++)
	{
		const int vert_count = mesh->GetPolygonSize(i);
		glBegin(GL_POLYGON);

		for (int j = 0; j < vert_count; j++)
		{
			GLdouble* vertex = (GLdouble*)vertices[mesh->GetPolygonVertex(i, j)];
			glNormal3dv(vertex);
			glVertex3dv(vertex);
		}

		glEnd();
	}
	glPopMatrix();
	delete[] vertices;
}

void FBXLoader::DrawMeshRecursive(FbxNode * node, FbxAMatrix & parentGlobalPos)
{
	FbxAMatrix temp_pos = node->EvaluateGlobalTransform(0);

	FbxNodeAttribute* node_attr = node->GetNodeAttribute();

	if (node_attr)
		if (node_attr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxAMatrix offset = GetGeometryOffset(node);
			FbxAMatrix offset_pos = temp_pos * offset;

			DrawMesh(node, offset_pos);
		}

	const int child_count = node->GetChildCount();
	for (int i = 0; i < child_count; i++)
		DrawMeshRecursive(node->GetChild(i), temp_pos);
}

FbxAMatrix FBXLoader::GetGeometryOffset(FbxNode * node)
{
	const FbxVector4 T = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 R = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 S = node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(T, R, S);
}


#endif