#pragma once
#include "Vector3D.h"
#include "Vector2D.h"
#include "VertexMesh.h"

class VertexFullMesh
{
public:
	VertexFullMesh() :m_position(), m_texcoord(), m_normal(), m_tangent(), m_binormal()
	{
	}
	VertexFullMesh(Vector3D position, Vector2D texcoord, Vector3D normal, Vector3D tangent, Vector3D binormal)
		:
		m_position(position),
		m_texcoord(texcoord),
		m_normal(normal),
		m_tangent(tangent),
		m_binormal(binormal)
	{
	}
	VertexFullMesh(const VertexFullMesh& vertex)
		:
		m_position(vertex.m_position),
		m_texcoord(vertex.m_texcoord),
		m_normal(vertex.m_normal),
		m_tangent(vertex.m_tangent),
		m_binormal(vertex.m_binormal)
	{
	}

	VertexFullMesh(const VertexMesh& vertex, Vector3D tangent, Vector3D binormal)
		:
		m_position(vertex.m_position),
		m_texcoord(vertex.m_texcoord),
		m_normal(vertex.m_normal),
		m_tangent(tangent),
		m_binormal(binormal)

	{
	}

	~VertexFullMesh()
	{
	}

public:
	Vector3D m_position;
	Vector2D m_texcoord;
	Vector3D m_normal;
	Vector3D m_tangent;
	Vector3D m_binormal;
};