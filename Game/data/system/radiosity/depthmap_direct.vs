float4x4 	g_matWorldViewPrj;

struct VS_Input
{
	float3 pos 		: POSITION0;
	float3 normal	: NORMAL0;
	float2 tex		: TEXCOORD0;
};

struct VS_Output
{
	float4 pos 		: POSITION;
	float2 depth 	: TEXCOORD0;
	float2 tex		: TEXCOORD1;
};

VS_Output main(VS_Input myin)
{
	VS_Output myout=(VS_Output)0;
	
	myout.pos 	= mul(float4(myin.pos.xyz,1.0f),g_matWorldViewPrj);	
	myout.depth.xy = myout.pos.zw;
	myout.tex	= myin.tex;
	
	return myout;
}
