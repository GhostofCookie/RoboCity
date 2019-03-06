#pragma once

#ifdef _WIN32
namespace fbxsdk
{
	class FbxScene;
	class FbxNode;
	class FbxAMatrix;
}
using namespace fbxsdk;

class FBXLoader
{
public:
	FBXLoader() {};
	FBXLoader(const char* file);
	FBXLoader(FBXLoader&);
	FBXLoader(FBXLoader*);

	void LoadFBX(const char* file);
	void Render();

private:
	void DrawMesh(FbxNode* node, FbxAMatrix& parentGlobalPos);
	void DrawMeshRecursive(FbxNode* node, FbxAMatrix& parentGlobalPos);
	FbxAMatrix GetGeometryOffset(FbxNode* node);

	FbxScene* mesh_scene;

};

#endif
