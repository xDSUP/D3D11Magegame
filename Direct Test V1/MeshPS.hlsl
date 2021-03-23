struct PixelInputType
{
	float4 pos: SV_POSITION;
	float2 tex: TEXCOORD;
};

Texture2D objTexture;
SamplerState sampleType;

float4 main(PixelInputType input) : SV_TARGET
{
	return objTexture.Sample(sampleType, input.tex);
}