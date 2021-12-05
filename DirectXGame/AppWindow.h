#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"

class AppWindow: public Window,public InputListener
{
public:
	AppWindow();

	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;	
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

public:
	void render();
	void update();
	void updateModel(Vector3D position, Vector3D scale, Vector3D rotation, const std::vector<MaterialPtr>& list_materials);
	void updateCamera();
	void updateSkyBox();
	void updateLight();

	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr> &list_materials);


private:
	SwapChainPtr m_swap_chain;
	//VertexBufferPtr m_vb;
	VertexShaderPtr m_vs;

	PixelShaderPtr m_ps;
	PixelShaderPtr m_sky_ps;

	ConstantBufferPtr m_cb;
	ConstantBufferPtr m_sky_cb;

	//IndexBufferPtr m_ib;

	//----TEXTURES
	TexturePtr m_wall_tex;
	TexturePtr m_bricks_tex;
	TexturePtr m_earth_color_tex;
	TexturePtr m_earth_spec_tex;
	TexturePtr m_clouds_tex;
	TexturePtr m_earth_night_tex;
	TexturePtr m_sand_tex;
	TexturePtr m_sky_tex;

	TexturePtr m_barrel_tex;
	TexturePtr m_windows_tex;
	TexturePtr m_wood_tex;
	TexturePtr m_effigy_tex;
	TexturePtr m_effigy_normal_tex;

#pragma region NANO_TEXTURE
	//Nano
	TexturePtr m_nano_arm_diff_tex;
	TexturePtr m_nano_arm_spec_tex;
	TexturePtr m_nano_body_diff_tex;
	TexturePtr m_nano_body_spec_tex;
	TexturePtr m_nano_glasses_tex;
	TexturePtr m_nano_hand_diff_tex;
	TexturePtr m_nano_hand_spec_tex;
	TexturePtr m_nano_helmet_diff_tex;
	TexturePtr m_nano_helmet_spec_tex;
	TexturePtr m_nano_leg_diff_tex;
	TexturePtr m_nano_leg_spec_tex;
#pragma endregion

#pragma region SPONZA_TEXTURE

	//Sponza
	TexturePtr m_sponza_background_diff_tex;
	TexturePtr m_sponza_background_norm_tex;
	TexturePtr m_sponza_background_spec_tex;

	TexturePtr m_sponza_chain_diff_tex;
	TexturePtr m_sponza_chain_norm_tex;
	TexturePtr m_sponza_chain_spec_tex;

	TexturePtr m_sponza_lion_diff_tex;
	TexturePtr m_sponza_lion_norm_tex;
	TexturePtr m_sponza_lion_spec_tex;

	TexturePtr m_sponza_bricks_diff_tex;
	TexturePtr m_sponza_bricks_norm_tex;
	TexturePtr m_sponza_bricks_spec_tex;

	TexturePtr m_sponza_arch_diff_tex;
	TexturePtr m_sponza_arch_norm_tex;
	TexturePtr m_sponza_arch_spec_tex;

	TexturePtr m_sponza_ceiling_diff_tex;
	TexturePtr m_sponza_ceiling_norm_tex;
	TexturePtr m_sponza_ceiling_spec_tex;

	TexturePtr m_sponza_columnA_diff_tex;
	TexturePtr m_sponza_columnA_norm_tex;
	TexturePtr m_sponza_columnA_spec_tex;

	TexturePtr m_sponza_columnB_diff_tex;
	TexturePtr m_sponza_columnB_norm_tex;
	TexturePtr m_sponza_columnB_spec_tex;

	TexturePtr m_sponza_columnC_diff_tex;
	TexturePtr m_sponza_columnC_norm_tex;
	TexturePtr m_sponza_columnC_spec_tex;

	TexturePtr m_sponza_curtain_blue_diff_tex;
	TexturePtr m_sponza_curtain_red_diff_tex;
	TexturePtr m_sponza_curtain_green_diff_tex;
	TexturePtr m_sponza_curtain_norm_tex;
	TexturePtr m_sponza_curtain_spec_tex;

	TexturePtr m_sponza_details_diff_tex;
	TexturePtr m_sponza_details_norm_tex;
	TexturePtr m_sponza_details_spec_tex;

	TexturePtr m_sponza_fabric_blue_diff_tex;
	TexturePtr m_sponza_fabric_red_diff_tex;
	TexturePtr m_sponza_fabric_green_diff_tex;
	TexturePtr m_sponza_fabric_norm_tex;
	TexturePtr m_sponza_fabric_spec_tex;

	TexturePtr m_sponza_flagpole_diff_tex;
	TexturePtr m_sponza_flagpole_norm_tex;
	TexturePtr m_sponza_flagpole_spec_tex;

	TexturePtr m_sponza_floor_diff_tex;
	TexturePtr m_sponza_floor_norm_tex;
	TexturePtr m_sponza_floor_spec_tex;

	TexturePtr m_sponza_roof_diff_tex;
	TexturePtr m_sponza_roof_norm_tex;
	TexturePtr m_sponza_roof_spec_tex;

	TexturePtr m_sponza_thorn_diff_tex;
	TexturePtr m_sponza_thorn_norm_tex;
	TexturePtr m_sponza_thorn_spec_tex;

	TexturePtr m_sponza_vase_diff_tex;
	TexturePtr m_sponza_vase_norm_tex;
	TexturePtr m_sponza_vase_spec_tex;

	TexturePtr m_sponza_hanging_diff_tex;
	TexturePtr m_sponza_hanging_norm_tex;
	TexturePtr m_sponza_hanging_spec_tex;

	TexturePtr m_sponza_plant_diff_tex;
	TexturePtr m_sponza_plant_norm_tex;
	TexturePtr m_sponza_plant_spec_tex;

	TexturePtr m_sponza_round_diff_tex;
	TexturePtr m_sponza_round_norm_tex;
	TexturePtr m_sponza_round_spec_tex;

#pragma endregion



	//----MESHES
	MeshPtr m_mesh;
	MeshPtr m_torus_mesh;
	MeshPtr m_suzanne_mesh;
	MeshPtr m_plane_mesh;
	MeshPtr m_sky_mesh;
	MeshPtr m_terrain_mesh;
	MeshPtr m_house_mesh;

	MeshPtr m_effigy_mesh;
	MeshPtr m_point_light_mesh;

	//Nano
	MeshPtr m_nano_mesh;

	//Sponza
	MeshPtr m_sponza_mesh;
	



	//----MATERIAL
	MaterialPtr m_mat;
	MaterialPtr m_brick_mat;
	MaterialPtr m_earth_mat;
	MaterialPtr m_sky_mat;
	MaterialPtr m_terrain_mat;

	MaterialPtr m_barrel_mat;
	MaterialPtr m_windows_mat;
	MaterialPtr m_wood_mat;

	MaterialPtr m_effigy_mat;
	MaterialPtr m_point_light_mat;

#pragma region NANO_MATERIAL
	//Nano
	MaterialPtr m_nano_arm_mat;
	MaterialPtr m_nano_body_mat;
	MaterialPtr m_nano_glasses_mat;
	MaterialPtr m_nano_hand_mat;
	MaterialPtr m_nano_helmet_mat;
	MaterialPtr m_nano_leg_mat;
#pragma endregion

#pragma region SPONZA_MATERIAL
	//Sponza
	MaterialPtr m_sponza_background;
	MaterialPtr m_sponza_chain;
	MaterialPtr m_sponza_lion;
	MaterialPtr m_sponza_bricks;
	MaterialPtr m_sponza_arch;
	MaterialPtr m_sponza_ceiling;
	MaterialPtr m_sponza_columnA;
	MaterialPtr m_sponza_columnB;
	MaterialPtr m_sponza_columnC;
	MaterialPtr m_sponza_curtain_blue;
	MaterialPtr m_sponza_curtain_red;
	MaterialPtr m_sponza_curtain_green;
	MaterialPtr m_sponza_details;
	MaterialPtr m_sponza_fabric_blue;
	MaterialPtr m_sponza_fabric_red;
	MaterialPtr m_sponza_fabric_green;
	MaterialPtr m_sponza_flagpole;
	MaterialPtr m_sponza_floor;
	MaterialPtr m_sponza_roof;
	MaterialPtr m_sponza_thorn;
	MaterialPtr m_sponza_vase;
	MaterialPtr m_sponza_hanging;
	MaterialPtr m_sponza_plant;
	MaterialPtr m_sponza_round;
#pragma endregion


private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_rot_x=0.0f;
	float m_rot_y = 0.0f;

	float m_light_rot_y = 0.0f;
	float m_light_y = 5.0f;

	float m_scale_cube = 1;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	float m_time = 0.0f;
	float m_light_radius = 10.0f;

	bool m_play_state = false;
	bool m_fullscreen_state = false;

	std::vector<MaterialPtr> m_list_materials;

	Vector4D m_light_position;
};

