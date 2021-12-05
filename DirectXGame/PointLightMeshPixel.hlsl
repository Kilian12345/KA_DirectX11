struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 direction_to_camera: TEXCOORD2;
};


float4 psmain(PS_INPUT input) : SV_TARGET
{
	return float4(1,1,1,1);
}