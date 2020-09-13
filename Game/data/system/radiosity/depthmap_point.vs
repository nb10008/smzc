float4x4	g_matWorld;
float4x4	g_matCubeMapViewPrj;
float3		g_vecLightPosWorld;

struct VS_Input
{
	float3 pos		: POSITION0;
	float3 normal	: NORMAL0;
	float2 tex		: TEXCOORD0;
};

struct VS_Output
{
	float4 pos		: POSITION;
	float3 toLight	: TEXCOORD0;
	float2 tex		: TEXCOORD1;
};

VS_Output main(VS_Input myin)
{
	VS_Output myout=(VS_Output)0;
	
	float4 posWorld=mul( float4(myin.pos.xyz,1.0f), g_matWorld );
	
	myout.toLight = posWorld.xyz - g_vecLightPosWorld;
	myout.pos	  = mul( posWorld, g_matCubeMapViewPrj );
	myout.tex	  = myin.tex;
	
	return myout;
}