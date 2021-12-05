#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path)
{

	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	std::string mtldir = inputfile.substr(0, inputfile.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str(), mtldir.c_str());


	if (!err.empty()) throw std::exception("Mesh not created successfully");
	if (!res) throw std::exception("Mesh not created successfully");

	std::vector<VertexFullMesh> list_vertices;
	std::vector<VertexMesh> list_base_vertices;
	std::vector<unsigned int> list_indices;

	size_t size_vertex_index_lists = 0;


	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_vertex_index_lists += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(size_vertex_index_lists);
	list_indices.reserve(size_vertex_index_lists);

	m_material_slots.resize(materials.size());

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{


		m_material_slots[m].start_index = index_global_offset;
		m_material_slots[m].material_id = m;


		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				if (shapes[s].mesh.material_ids[f] != m) continue;

				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));

					list_base_vertices.push_back(vertex);
					list_indices.push_back(index_global_offset + v);


				}

				CalculateTangentBinormal(list_base_vertices[0], list_base_vertices[1], list_base_vertices[2], m_tangent, m_binormal);

				for (unsigned char fv = 0; fv < list_base_vertices.size(); fv++)
				{
					VertexFullMesh full_vertex(list_base_vertices[fv].m_position, list_base_vertices[fv].m_texcoord, list_base_vertices[fv].m_normal, m_tangent, m_binormal);

					list_vertices.push_back(full_vertex);
				}


				list_base_vertices.clear();

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}

		m_material_slots[m].num_indices = index_global_offset - m_material_slots[m].start_index;
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCode(&shader_byte_code, &size_shader);


	 m_vertex_buffer =  GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexFullMesh), 
		 (UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);
	
	 m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size() );
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

const MaterialSlot& Mesh::getMaterialSlot(unsigned int slot)
{
	if (slot >= m_material_slots.size()) return MaterialSlot();

	return m_material_slots[slot];
}

size_t Mesh::getNumMaterialSlots()
{
	return m_material_slots.size();
}

void Mesh::CalculateTangentBinormal(VertexMesh vertex1, VertexMesh vertex2, VertexMesh vertex3, Vector3D& tangent, Vector3D& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.m_position.m_x - vertex1.m_position.m_x;
	vector1[1] = vertex2.m_position.m_y - vertex1.m_position.m_y;
	vector1[2] = vertex2.m_position.m_z - vertex1.m_position.m_z;

	vector2[0] = vertex3.m_position.m_x - vertex1.m_position.m_x;
	vector2[1] = vertex3.m_position.m_y - vertex1.m_position.m_y;
	vector2[2] = vertex3.m_position.m_z - vertex1.m_position.m_z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.m_texcoord.m_x - vertex1.m_texcoord.m_x ;
	tvVector[0] = vertex2.m_texcoord.m_y - vertex1.m_texcoord.m_y ;

	tuVector[1] = vertex3.m_texcoord.m_x - vertex1.m_texcoord.m_x;
	tvVector[1] = vertex3.m_texcoord.m_y - vertex1.m_texcoord.m_y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.m_x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.m_y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.m_z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.m_x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.m_y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.m_z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of the tangent.
	length = (float)sqrt((tangent.m_x * tangent.m_x) + (tangent.m_y * tangent.m_y) + (tangent.m_z * tangent.m_z));

	// Normalize the tangent and then store it.
	tangent.m_x = tangent.m_x / length;
	tangent.m_y = tangent.m_y / length;
	tangent.m_z = tangent.m_z / length;

	// Calculate the length of the binormal.
	length = (float)sqrt((binormal.m_x * binormal.m_x) + (binormal.m_y * binormal.m_y) + (binormal.m_z * binormal.m_z));

	// Normalize the binormal and then store it.
	binormal.m_x = binormal.m_x / length;
	binormal.m_y = binormal.m_y / length;
	binormal.m_z = binormal.m_z / length;

	return;
}
