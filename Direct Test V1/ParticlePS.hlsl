Texture2D shaderTexture;
SamplerState sampleType;

struct PixelInputType
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

float4 main(PixelInputType input) : SV_TARGET
{
    return shaderTexture.Sample(sampleType, input.tex);
}