struct CameraBuffer
{
	float4x4 camView;
	float4x4 camProjection;
	float4 camLookAt;
};

cbuffer Camera
{
	CameraBuffer cameraBuffer;
};

struct ConstantBufferSkyLight
{
	float4 direction;
	float4 position;
	float4 ambient;
	float4 diffuse;
	float difIntensity;
};
cbuffer SkyLight
{
	ConstantBufferSkyLight SkyLight;
};

cbuffer TransformConstantBuffer::Transforms
{
	float4x4 WorldViewProjection;
	float4x4 World;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;
TextureCube SkyMap;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct SKYMAP_VS_OUTPUT	//output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;

	output.Pos = mul(inPos, WorldViewProjection);

	output.normal = mul(normal, World);

	output.TexCoord = inTexCoord;

	return output;
}

SKYMAP_VS_OUTPUT SKYMAP_VS(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;
	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Pos = mul(float4(inPos, 1.0f), WorldViewProjection).xyww;

	output.texCoord = inPos;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float4 __diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);

	float3 __finalColor;

	__finalColor = __diffuse * SkyLight.ambient;
	__finalColor += saturate(dot(SkyLight.direction, input.normal) * SkyLight.diffuse * __diffuse);

	return float4(__finalColor, __diffuse.a);
}

float4 SKYMAP_PS(SKYMAP_VS_OUTPUT input) : SV_Target
{
	return SkyMap.Sample(ObjSamplerState, input.texCoord);
}

float4 D2D_PS(VS_OUTPUT input) : SV_TARGET
{
	return ObjTexture.Sample(ObjSamplerState, input.TexCoord);
}
