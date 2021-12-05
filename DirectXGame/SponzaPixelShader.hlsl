
Texture2D TextureColor: register(t0);
sampler TextureColorSampler: register(s0);

Texture2D NormalTexture: register(t1);
sampler NormalTextureSampler: register(s1);

Texture2D SpecularTexture: register(t2);
sampler SpecularTextureSampler: register(s2);


struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 tangent: TANGENT0;
	float3 binormal: BINORMAL0;
	float3 world_pos: TEXCOORD1;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_light_direction;
	float4 m_camera_position;
	float4 m_light_position;
	float m_light_radius;
	float m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 tex_color = TextureColor.Sample(TextureColorSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
	float spec_tex = SpecularTexture.Sample(SpecularTextureSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y)).r;

	if (tex_color.a == 0)
	{
		discard;
	}

	// AMBIENT LIGHT
	float ka = 0.1;
	float3 ia = float3(1,1,1);
	ia *= tex_color;
	float3 ambient_light = ka * ia;

	//DIFFUSE LIGHT
	float kd = 50.7;
	float3 light_dir = normalize(m_light_position.xyz - input.world_pos.xyz);
	float distance_light_object = length(m_light_position.xyz - input.world_pos.xyz);

	float fade_area = max(0, distance_light_object - m_light_radius);

	float constant_func = 1.0;
	float linear_func = 0.1;
	float quadratic_func = 0.1;

	float attenuation = constant_func + linear_func * fade_area + quadratic_func * fade_area * fade_area;

	float amount_diffuse_light = max(0, dot(light_dir.xyz, input.normal));
	float3 id = float3(1,1,1);
	id *= (tex_color.rgb);
	float3 diffuse_light = (kd * id * amount_diffuse_light) / attenuation;


	//SPECULAR LIGHT
	float ks = 0.2;
	float3 direction_to_camera = normalize(input.world_pos.xyz - m_camera_position.xyz);
	float3 is = float3(1.0, 1.0, 1.0);
	float3 reflected_light = reflect(light_dir.xyz, input.normal);
	reflected_light = normalize(reflected_light);
	float shininess = 100.0;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, direction_to_camera)), shininess);

	float3 specular_light = (ks * amount_specular_light * is) / attenuation;
	specular_light *= spec_tex;

	float3 final_light = ambient_light + diffuse_light + specular_light;


	//NORMAL MAP
	float4 bumpMap;
	float3 bumpNormal;

	// Calculate the amount of light on this pixel using the normal map.
	bumpMap = NormalTexture.Sample(NormalTextureSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
	bumpMap = (bumpMap * 2.0) - 1.0;
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal) ;
	bumpNormal = normalize(bumpNormal);
	float lightIntensity = clamp(dot(light_dir, bumpNormal),0,1);
	lightIntensity /= attenuation;

	float3 output = clamp(lightIntensity * diffuse_light,0,1);

	//float3 output = clamp(final_light * lightIntensity, 0, 1);
	//output = final_light;

	float3 test = float3(lightIntensity, lightIntensity, lightIntensity);

	//return float4(output * final_light, 1.0);
	return float4(diffuse_light, 1.0);
}