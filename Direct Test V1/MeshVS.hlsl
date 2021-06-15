#include "Basic.hlsli"

struct VertexInputType
{
	float4 pos: POSITION;
	float3 normal: NORMAL;
	float2 tex: TEXCOORD;
};


VertexPosHWNormalTex main(VertexInputType input)
{
	VertexPosHWNormalTex vOut;
	//matrix viewProj = mul(g_View, g_Proj);
	//float4 posW = mul(float4(input.pos, 1.0f), g_World);

	vOut.PosH = mul(input.pos, WVP);
	vOut.PosW = mul(input.pos, g_World);
	vOut.NormalW = mul(input.normal, (float3x3) g_WorldInvTranspose);
	vOut.Tex = input.tex;
	return vOut;
}