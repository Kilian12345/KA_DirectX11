#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
	Vector4D m_light_position = Vector4D(0, 0, 0, 0);
	float m_light_radius = 0.0f;
	float m_time = 0.0f;
};


AppWindow::AppWindow()
{
}

void AppWindow::render()
{

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0.5f, 0.0f, 0.2f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);




	update();

	//// NANO SUIT MESH
	m_list_materials.clear();
	m_list_materials.push_back(m_nano_arm_mat);
	m_list_materials.push_back(m_nano_body_mat);
	m_list_materials.push_back(m_nano_glasses_mat);
	m_list_materials.push_back(m_nano_hand_mat);
	m_list_materials.push_back(m_nano_helmet_mat);
	m_list_materials.push_back(m_nano_leg_mat);

	for (int i = 0; i < 3; i++)
	{

		updateModel(Vector3D(-6.0f + 6.0f * i, 0, 2), Vector3D(0.3f, 0.3f, 0.3f), Vector3D(0, 90, 0), m_list_materials);
		drawMesh(m_nano_mesh, m_list_materials);
	}


	//// SPONZA MESH
	m_list_materials.clear();
	m_list_materials.push_back(m_sponza_thorn);
	m_list_materials.push_back(m_sponza_round);
	m_list_materials.push_back(m_sponza_plant);
	m_list_materials.push_back(m_sponza_plant);
	m_list_materials.push_back(m_sponza_background);
	m_list_materials.push_back(m_sponza_fabric_red);
	m_list_materials.push_back(m_sponza_bricks);
	m_list_materials.push_back(m_sponza_arch);
	m_list_materials.push_back(m_sponza_ceiling);
	m_list_materials.push_back(m_sponza_columnA);
	m_list_materials.push_back(m_sponza_floor);
	m_list_materials.push_back(m_sponza_columnC);
	m_list_materials.push_back(m_sponza_details);
	m_list_materials.push_back(m_sponza_columnB);
	m_list_materials.push_back(m_sponza_hanging);
	m_list_materials.push_back(m_sponza_flagpole);
	m_list_materials.push_back(m_sponza_fabric_green);
	m_list_materials.push_back(m_sponza_fabric_blue);
	m_list_materials.push_back(m_sponza_fabric_red);
	m_list_materials.push_back(m_sponza_curtain_blue);
	m_list_materials.push_back(m_sponza_curtain_red);
	m_list_materials.push_back(m_sponza_curtain_green);
	m_list_materials.push_back(m_sponza_chain);
	m_list_materials.push_back(m_sponza_hanging);
	m_list_materials.push_back(m_sponza_vase);
	m_list_materials.push_back(m_sponza_lion);
	m_list_materials.push_back(m_sponza_roof);
	

	updateModel(Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.03f, 0.03f, 0.03f), Vector3D(0, 0, 0), m_list_materials);
	drawMesh(m_sponza_mesh, m_list_materials);
	


	//// EFFIGY SUIT MESH
	/*m_list_materials.clear();
	m_list_materials.push_back(m_effigy_mat);

	for (int i = 0; i < 1; i++)
	{

		updateModel(Vector3D(-6.0f + 6.0f *i, 0, -2), Vector3D(3, 3, 3), Vector3D(0, 90, 0), m_list_materials);
		drawMesh(m_effigy_mesh, m_list_materials);
		
	}*/



	m_list_materials.clear();
	m_list_materials.push_back(m_terrain_mat);
	updateModel(Vector3D(0, 0, 0), Vector3D(1, 1, 1), Vector3D(0,0,0) ,m_list_materials);
	drawMesh(m_terrain_mesh, m_list_materials);


	m_list_materials.clear();
	m_list_materials.push_back(m_point_light_mat);
	updateModel(Vector3D(m_light_position.m_x, m_light_position.m_y, m_light_position.m_z), Vector3D(0.1f, 0.1f, 0.1f), Vector3D(0, 0, 0), m_list_materials);
	drawMesh(m_point_light_mesh, m_list_materials);


	m_list_materials.clear();
	m_list_materials.push_back(m_sky_mat);


	// RENDER SKYBOX/SPHERE
	//updateModel(Vector3D(0, 0, 0), m_sky_mat);
	drawMesh(m_sky_mesh, m_list_materials);

	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_time += m_delta_time;

}

void AppWindow::update()
{
	updateCamera();
	updateLight();
	updateSkyBox();
}

void AppWindow::updateModel(Vector3D position, Vector3D scale, Vector3D rotation, const std::vector<MaterialPtr>& list_materials)
{
	constant cc;

	Matrix4x4 m_light_rot_matrix,temp;
	m_light_rot_matrix.setIdentity();
	m_light_rot_matrix.setRotationY(m_light_rot_y);

	cc.m_world.setIdentity();
	cc.m_world.setScale(scale);

	temp.setIdentity();
	temp.setRotationX(rotation.m_x);
	cc.m_world *= temp;
	temp.setIdentity();
	temp.setRotationY(rotation.m_y);
	cc.m_world *= temp;
	temp.setIdentity();
	temp.setRotationZ(rotation.m_z);
	cc.m_world *= temp;

	cc.m_world.setTranslation(position);
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	cc.m_light_position = m_light_position;
	cc.m_light_radius = m_light_radius;
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;

	for (size_t m = 0; m < list_materials.size(); m++)
	{

		list_materials[m]->setData(&cc, sizeof(constant));
	}
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;


	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.1f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.1f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;


	world_cam.inverse();




	m_view_cam = world_cam;
	/*cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left)/300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top)/300.0f,
		-4.0f,
		4.0f
	);*/

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);


	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.01f, 100.0f);
}

void AppWindow::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;


	m_sky_mat->setData(&cc, sizeof(constant));
}

void AppWindow::updateLight()
{
	m_light_rot_y += 0.5f * m_delta_time;
	float dist_from_origin = 5.0f;

	m_light_position = Vector4D(cos(m_light_rot_y) * dist_from_origin, m_light_y, sin(m_light_rot_y) * dist_from_origin, 1.0f);

}

void AppWindow::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
{

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());


	for (size_t m = 0; m < mesh->getNumMaterialSlots(); m++)
	{

		if (m >= list_materials.size()) break;

		MaterialSlot mat = mesh->getMaterialSlot(m);
		GraphicsEngine::get()->setMaterial(list_materials[m]);
		// FINALLY DRAW THE TRIANGLE
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
	}
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);

	m_play_state = true;
	InputSystem::get()->showCursor(false);

	/*m_wall_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wall.jpg");
	m_bricks_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_earth_color_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_color.jpg");*/

	//m_earth_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_spec.jpg");
	//m_clouds_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\clouds.jpg");
	//m_earth_night_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_night.jpg");


	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");
	m_sand_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sand.jpg");

	//m_barrel_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\barrel.jpg");
	//m_windows_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_windows.jpg");
	//m_bricks_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_brick.jpg");
	//m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_wood.jpg");
	//m_effigy_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\effigy.png");
	//m_effigy_normal_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\effigy_normal.png");

#pragma region NANO_TEX
	m_nano_arm_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\arm_dif.png");
	m_nano_arm_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\arm_showroom_spec.png");
	m_nano_body_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\body_dif.png");
	m_nano_body_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\body_showroom_spec.png");
	m_nano_glasses_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\glass_dif.png");
	m_nano_hand_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\hand_dif.png");
	m_nano_hand_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\hand_showroom_spec.png");
	m_nano_helmet_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\helmet_diff.png");
	m_nano_helmet_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\helmet_showroom_spec.png");
	m_nano_leg_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\leg_dif.png");
	m_nano_leg_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Nano\\leg_showroom_spec.png");
#pragma endregion

#pragma region SPONZA_TEX
	
	m_sponza_background_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\background.png");
	m_sponza_background_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\background_ddn.png");
	m_sponza_background_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\backgroundBGR.png");

	m_sponza_chain_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\chain_texture.png");
	m_sponza_chain_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\chain_texture_ddn.png");
	m_sponza_chain_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\chain_texture_spec.png");

	m_sponza_lion_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\lion.png");
	m_sponza_lion_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\lion_ddn.png");
	m_sponza_lion_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\black.png");

	m_sponza_bricks_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\spnza_bricks_a_diff.png");
	m_sponza_bricks_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\spnza_bricks_a_ddn.png");
	m_sponza_bricks_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\spnza_bricks_a_spec.png");

	m_sponza_arch_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_arch_diff.png");
	m_sponza_arch_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_arch_ddn.png");
	m_sponza_arch_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_arch_spec.png");

	m_sponza_ceiling_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_ceiling_a_diff.png");
	m_sponza_ceiling_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_ceiling_ddn.jpg");
	m_sponza_ceiling_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_ceiling_a_spec.png");

	m_sponza_columnA_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_a_diff.png");
	m_sponza_columnA_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_a_ddn.png");
	m_sponza_columnA_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_a_spec.png");

	m_sponza_columnB_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_b_diff.png");
	m_sponza_columnB_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_b_ddn.png");
	m_sponza_columnB_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_b_spec.png");

	m_sponza_columnC_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_c_diff.png");
	m_sponza_columnC_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_c_ddn.png");
	m_sponza_columnC_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_column_c_spec.png");







	m_sponza_curtain_blue_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_curtain_blue_diff.png");
	m_sponza_curtain_red_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_curtain_diff.png");
	m_sponza_curtain_green_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_curtain_green_diff.png");
	m_sponza_curtain_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_curtain_ddn.jpg");
	m_sponza_curtain_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_curtain_blue_diff_spec.png");

	m_sponza_details_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_details_diff.png");
	m_sponza_details_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_details_ddn.jpg");
	m_sponza_details_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_details_spec.png");





	m_sponza_fabric_blue_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_fabric_blue_diff.png");
	m_sponza_fabric_red_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_fabric_diff.png");
	m_sponza_fabric_green_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_fabric_green_diff.png");
	m_sponza_fabric_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_fabric_ddn.jpg");
	m_sponza_fabric_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_fabric_spec.png");

	m_sponza_flagpole_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_flagpole_diff.png");
	m_sponza_flagpole_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_flagpole_diff_NRM.png");
	m_sponza_flagpole_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_flagpole_spec.png");

	m_sponza_floor_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_floor_a_diff.png");
	m_sponza_floor_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_floor_ddn.jpg");
	m_sponza_floor_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_floor_a_diff_spec.png");

	m_sponza_roof_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_roof_diff.png");
	m_sponza_roof_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_roof_diff_NRM.png");
	m_sponza_roof_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\black.png");

	m_sponza_thorn_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_thorn_diff.png");
	m_sponza_thorn_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_thorn_ddn.png");
	m_sponza_thorn_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\sponza_thorn_spec.png");

	m_sponza_vase_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_dif.png");
	m_sponza_vase_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_ddn.png");
	m_sponza_vase_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\black.png");

	m_sponza_hanging_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_hanging.png");
	m_sponza_hanging_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_hanging_NRM.png");
	m_sponza_hanging_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\black.png");

	m_sponza_plant_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_plant.png");
	m_sponza_plant_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_plant_ddn.jpg");
	m_sponza_plant_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_plant_spec.png");

	m_sponza_round_diff_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_round.png");
	m_sponza_round_norm_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_round_ddn.png");
	m_sponza_round_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sponza\\vase_round_spec.png");

#pragma endregion

	/*m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_torus_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\torus.obj");
	m_suzanne_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	m_plane_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\plane.obj");*/

	//m_effigy_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\effigy.obj");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_terrain_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\terrain.obj");
	//m_house_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\house.obj");
	m_point_light_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

	m_nano_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\nanosuit.obj");
	m_sponza_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sponza.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);




	m_world_cam.setTranslation(Vector3D(0, 0, -1));




	m_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"PointLightPixelShader.hlsl");
	m_mat->addTexture(m_wall_tex);
	m_mat->setCullMode(CULL_MODE_BACK);

	/*m_earth_mat = GraphicsEngine::get()->createMaterial(m_mat);
	m_earth_mat->addTexture(m_earth_color_tex);
	m_earth_mat->setCullMode(CULL_MODE_BACK);

	m_brick_mat = GraphicsEngine::get()->createMaterial(m_mat);
	m_brick_mat->addTexture(m_bricks_tex);
	m_brick_mat->setCullMode(CULL_MODE_BACK);*/

	m_terrain_mat = GraphicsEngine::get()->createMaterial(m_mat);
	m_terrain_mat->addTexture(m_sand_tex);
	m_terrain_mat->setCullMode(CULL_MODE_BACK);





	m_barrel_mat = GraphicsEngine::get()->createMaterial(m_mat);
	m_barrel_mat->addTexture(m_barrel_tex);
	m_barrel_mat->setCullMode(CULL_MODE_BACK);

	m_brick_mat = GraphicsEngine::get()->createMaterial(m_mat);
	m_brick_mat->addTexture(m_bricks_tex);
	m_brick_mat->setCullMode(CULL_MODE_BACK);

	m_windows_mat = GraphicsEngine::get()->createMaterial(m_mat);
	m_windows_mat->addTexture(m_windows_tex);
	m_windows_mat->setCullMode(CULL_MODE_BACK);

	m_wood_mat = GraphicsEngine::get()->createMaterial(m_mat);
	m_wood_mat->addTexture(m_wood_tex);
	m_wood_mat->setCullMode(CULL_MODE_BACK);


	

	/*m_effigy_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"StatuePixelShader.hlsl");
	m_effigy_mat->addTexture(m_effigy_tex);
	m_effigy_mat->addTexture(m_effigy_normal_tex);
	m_effigy_mat->setCullMode(CULL_MODE_BACK);*/

	
	

	m_point_light_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"PointLightMeshPixel.hlsl");
	m_point_light_mat->addTexture(m_sky_tex);
	m_point_light_mat->setCullMode(CULL_MODE_BACK);


	m_sky_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SkyBoxShader.hlsl");
	m_sky_mat->addTexture(m_sky_tex);
	m_sky_mat->setCullMode(CULL_MODE_FRONT);


#pragma region NANO_MATERIAL
	//------------Nano

	m_nano_arm_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"NanoPixelShader.hlsl");
	m_nano_arm_mat->addTexture(m_nano_arm_diff_tex);
	m_nano_arm_mat->addTexture(m_nano_arm_spec_tex);
	m_nano_arm_mat->setCullMode(CULL_MODE_BACK);

	m_nano_body_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"NanoPixelShader.hlsl");
	m_nano_body_mat->addTexture(m_nano_body_diff_tex);
	m_nano_body_mat->addTexture(m_nano_body_spec_tex);
	m_nano_body_mat->setCullMode(CULL_MODE_BACK);

	m_nano_hand_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"NanoPixelShader.hlsl");
	m_nano_hand_mat->addTexture(m_nano_hand_diff_tex);
	m_nano_hand_mat->addTexture(m_nano_hand_spec_tex);
	m_nano_hand_mat->setCullMode(CULL_MODE_BACK);

	m_nano_helmet_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"NanoPixelShader.hlsl");
	m_nano_helmet_mat->addTexture(m_nano_helmet_diff_tex);
	m_nano_helmet_mat->addTexture(m_nano_helmet_spec_tex);
	m_nano_helmet_mat->setCullMode(CULL_MODE_BACK);

	m_nano_leg_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"NanoPixelShader.hlsl");
	m_nano_leg_mat->addTexture(m_nano_leg_diff_tex);
	m_nano_leg_mat->addTexture(m_nano_leg_spec_tex);
	m_nano_leg_mat->setCullMode(CULL_MODE_BACK);

	m_nano_glasses_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"NanoGlassesPixelShader.hlsl");
	m_nano_glasses_mat->addTexture(m_nano_glasses_tex);
	m_nano_glasses_mat->setCullMode(CULL_MODE_BACK);
#pragma endregion

	
#pragma region SPONZA_MATERIAL

	m_sponza_background = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_background->addTexture(m_sponza_background_diff_tex);
	m_sponza_background->addTexture(m_sponza_background_norm_tex);
	m_sponza_background->addTexture(m_sponza_background_spec_tex);
	m_sponza_background->setCullMode(CULL_MODE_BACK);

	m_sponza_chain = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_chain->addTexture(m_sponza_chain_diff_tex);
	m_sponza_chain->addTexture(m_sponza_chain_norm_tex);
	m_sponza_chain->addTexture(m_sponza_chain_spec_tex);
	m_sponza_chain->setCullMode(CULL_MODE_BACK);

	m_sponza_lion = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_lion->addTexture(m_sponza_lion_diff_tex);
	m_sponza_lion->addTexture(m_sponza_lion_norm_tex);
	m_sponza_lion->addTexture(m_sponza_lion_spec_tex);
	m_sponza_lion->setCullMode(CULL_MODE_BACK);

	m_sponza_bricks = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_bricks->addTexture(m_sponza_bricks_diff_tex);
	m_sponza_bricks->addTexture(m_sponza_bricks_norm_tex);
	m_sponza_bricks->addTexture(m_sponza_bricks_spec_tex);
	m_sponza_bricks->setCullMode(CULL_MODE_BACK);

	m_sponza_arch = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_arch->addTexture(m_sponza_arch_diff_tex);
	m_sponza_arch->addTexture(m_sponza_arch_norm_tex);
	m_sponza_arch->addTexture(m_sponza_arch_spec_tex);
	m_sponza_arch->setCullMode(CULL_MODE_BACK);

	m_sponza_ceiling = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_ceiling->addTexture(m_sponza_ceiling_diff_tex);
	m_sponza_ceiling->addTexture(m_sponza_ceiling_norm_tex);
	m_sponza_ceiling->addTexture(m_sponza_ceiling_spec_tex);
	m_sponza_ceiling->setCullMode(CULL_MODE_BACK);

	m_sponza_columnA = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_columnA->addTexture(m_sponza_columnA_diff_tex);
	m_sponza_columnA->addTexture(m_sponza_columnA_norm_tex);
	m_sponza_columnA->addTexture(m_sponza_columnA_spec_tex);
	m_sponza_columnA->setCullMode(CULL_MODE_BACK);

	m_sponza_columnB = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_columnB->addTexture(m_sponza_columnB_diff_tex);
	m_sponza_columnB->addTexture(m_sponza_columnB_norm_tex);
	m_sponza_columnB->addTexture(m_sponza_columnB_spec_tex);
	m_sponza_columnB->setCullMode(CULL_MODE_BACK);

	m_sponza_columnC = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_columnC->addTexture(m_sponza_columnC_diff_tex);
	m_sponza_columnC->addTexture(m_sponza_columnC_norm_tex);
	m_sponza_columnC->addTexture(m_sponza_columnC_spec_tex);
	m_sponza_columnC->setCullMode(CULL_MODE_BACK);

	m_sponza_curtain_blue = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_curtain_blue->addTexture(m_sponza_curtain_blue_diff_tex);
	m_sponza_curtain_blue->addTexture(m_sponza_curtain_norm_tex);
	m_sponza_curtain_blue->addTexture(m_sponza_curtain_spec_tex);
	m_sponza_curtain_blue->setCullMode(CULL_MODE_BACK);

	m_sponza_curtain_red = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_curtain_red->addTexture(m_sponza_curtain_red_diff_tex);
	m_sponza_curtain_red->addTexture(m_sponza_curtain_norm_tex);
	m_sponza_curtain_red->addTexture(m_sponza_curtain_spec_tex);
	m_sponza_curtain_red->setCullMode(CULL_MODE_BACK);

	m_sponza_curtain_green = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_curtain_green->addTexture(m_sponza_curtain_green_diff_tex);
	m_sponza_curtain_green->addTexture(m_sponza_curtain_norm_tex);
	m_sponza_curtain_green->addTexture(m_sponza_curtain_spec_tex);
	m_sponza_curtain_green->setCullMode(CULL_MODE_BACK);

	m_sponza_details = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_details->addTexture(m_sponza_details_diff_tex);
	m_sponza_details->addTexture(m_sponza_details_norm_tex);
	m_sponza_details->addTexture(m_sponza_details_spec_tex);
	m_sponza_details->setCullMode(CULL_MODE_BACK);

	m_sponza_fabric_blue = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_fabric_blue->addTexture(m_sponza_fabric_blue_diff_tex);
	m_sponza_fabric_blue->addTexture(m_sponza_fabric_norm_tex);
	m_sponza_fabric_blue->addTexture(m_sponza_fabric_spec_tex);
	m_sponza_fabric_blue->setCullMode(CULL_MODE_BACK);

	m_sponza_fabric_red = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_fabric_red->addTexture(m_sponza_fabric_red_diff_tex);
	m_sponza_fabric_red->addTexture(m_sponza_fabric_norm_tex);
	m_sponza_fabric_red->addTexture(m_sponza_fabric_spec_tex);
	m_sponza_fabric_red->setCullMode(CULL_MODE_BACK);

	m_sponza_fabric_green = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_fabric_green->addTexture(m_sponza_fabric_green_diff_tex);
	m_sponza_fabric_green->addTexture(m_sponza_fabric_norm_tex);
	m_sponza_fabric_green->addTexture(m_sponza_fabric_spec_tex);
	m_sponza_fabric_green->setCullMode(CULL_MODE_BACK);

	m_sponza_flagpole = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_flagpole->addTexture(m_sponza_flagpole_diff_tex);
	m_sponza_flagpole->addTexture(m_sponza_flagpole_norm_tex);
	m_sponza_flagpole->addTexture(m_sponza_flagpole_spec_tex);
	m_sponza_flagpole->setCullMode(CULL_MODE_BACK);

	m_sponza_floor = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_floor->addTexture(m_sponza_floor_diff_tex);
	m_sponza_floor->addTexture(m_sponza_floor_norm_tex);
	m_sponza_floor->addTexture(m_sponza_floor_spec_tex);
	m_sponza_floor->setCullMode(CULL_MODE_BACK);

	m_sponza_roof = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_roof->addTexture(m_sponza_roof_diff_tex);
	m_sponza_roof->addTexture(m_sponza_roof_norm_tex);
	m_sponza_roof->addTexture(m_sponza_roof_spec_tex);
	m_sponza_roof->setCullMode(CULL_MODE_BACK);

	m_sponza_thorn = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_thorn->addTexture(m_sponza_thorn_diff_tex);
	m_sponza_thorn->addTexture(m_sponza_thorn_norm_tex);
	m_sponza_thorn->addTexture(m_sponza_thorn_spec_tex);
	m_sponza_thorn->setCullMode(CULL_MODE_BACK);

	m_sponza_vase = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_vase->addTexture(m_sponza_vase_diff_tex);
	m_sponza_vase->addTexture(m_sponza_vase_norm_tex);
	m_sponza_vase->addTexture(m_sponza_vase_spec_tex);
	m_sponza_vase->setCullMode(CULL_MODE_BACK); 

	m_sponza_hanging = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_hanging->addTexture(m_sponza_hanging_diff_tex);
	m_sponza_hanging->addTexture(m_sponza_hanging_norm_tex);
	m_sponza_hanging->addTexture(m_sponza_hanging_spec_tex);
	m_sponza_hanging->setCullMode(CULL_MODE_BACK); 

	m_sponza_plant = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_plant->addTexture(m_sponza_plant_diff_tex);
	m_sponza_plant->addTexture(m_sponza_plant_norm_tex);
	m_sponza_plant->addTexture(m_sponza_plant_spec_tex);
	m_sponza_plant->setCullMode(CULL_MODE_BACK);

	m_sponza_round = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SponzaPixelShader.hlsl");
	m_sponza_round->addTexture(m_sponza_round_diff_tex);
	m_sponza_round->addTexture(m_sponza_round_norm_tex);
	m_sponza_round->addTexture(m_sponza_round_spec_tex);
	m_sponza_round->setCullMode(CULL_MODE_BACK);

#pragma endregion


	m_world_cam.setTranslation(Vector3D(0, 2, -2));

	m_list_materials.reserve(256);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();

	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right, rc.bottom);
	this->render();
}

void AppWindow::onKeyDown(int key)
{
	if (!m_play_state) return;

	if (key == 'Z')
	{
		//m_rot_x += 3.14f*m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 3.14f*m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'Q')
	{
		//m_rot_y += 3.14f*m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 3.14f*m_delta_time;
		m_rightward = +1.0f;
	}
	else if (key == 'W')
	{
		//m_rot_y += 3.14f*m_delta_time;
		m_light_y -= 1.0f;
	}
	else if (key == 'X')
	{
		//m_rot_y -= 3.14f*m_delta_time;
		m_light_y += 1.0f;
	}
	else if (key == 'O')
	{
		m_light_radius -= 100.0f * m_delta_time;
	}
	else if (key == 'P')
	{
		m_light_radius += 100.0f * m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'G')
	{
		m_play_state = (m_play_state) ? false : true;
		InputSystem::get()->showCursor(!m_play_state);
	}
	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;
		RECT size_screen = this->getSizeScreen();

		m_swap_chain->setFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;

	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
