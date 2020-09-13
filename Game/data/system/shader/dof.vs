float4x4 g_matWorldViewPrj;
float4x4 g_matWorldView;
float2 g_depth;
struct VS_OUTPUT
{
    float4 pos : POSITION;
    float2 uv 	: TEXCOORD0;
    float diffuse: TEXCOORD1;
};

VS_OUTPUT main(float3 pos: POSITION, float2 uv: TEXCOORD0)
{
    VS_OUTPUT vsout = (VS_OUTPUT)0;
    vsout.pos = mul(float4(pos,1.0f), g_matWorldViewPrj);
    float4 view = mul(float4(pos, 1.0f), g_matWorldView);
    float dist = (view.z - g_depth.x)/g_depth.y;
    vsout.diffuse = saturate(dist);
    vsout.uv = uv;
    return vsout;
}