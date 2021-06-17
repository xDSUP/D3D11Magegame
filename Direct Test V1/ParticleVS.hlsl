
struct VertexInputType
{
	float4 pos:POSITION;
	float2 tex:TEXCOORD;
};

struct PixelInputType
{
	float4 pos:SV_POSITION;
	float2 tex:TEXCOORD;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	output.pos = mul(input.pos, ortho);
	output.tex = input.tex;

	return output;
}