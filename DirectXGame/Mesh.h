#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexFullMesh.h"
#include <vector>

struct MaterialSlot
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
};

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* full_path);
	~Mesh();
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

	const MaterialSlot& getMaterialSlot(unsigned int slot);
	size_t getNumMaterialSlots();

private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	std::vector<MaterialSlot> m_material_slots;

private:
	void CalculateTangentBinormal(VertexMesh vertex1, VertexMesh vertex2, VertexMesh vertex3, Vector3D& tangent, Vector3D& binormal);
	Vector3D m_tangent;
	Vector3D m_binormal;

private:
	friend class DeviceContext;
};

