Texture2D shaderTexture;
SamplerState sampleType;

struct PixelInputType
{
	
};

float4 main(PixelInputType input) : SV_TARGET
{
	//return shaderTexture.Sample(sampleType, input.Tex);// *input.Color;
	//return input.Color;
	return float4(1,1,1,1);
}