float4x4 	g_matWorldViewPrj;
float       g_fStart;
float       g_fEnd;
float3      g_Color;

struct VS_INPUT
{
	float3 pos 		: POSITION;
};

struct VS_OUTPUT
{
	float4 pos 		: POSITION;
	float4 diffuse	: COLOR0;
};

VS_OUTPUT main(VS_INPUT vert)
{
	VS_OUTPUT vsout = (VS_OUTPUT)0;
	
	vsout.pos = mul(float4(vert.pos.xyz,1),g_matWorldViewPrj);
	
	float f = ( g_fEnd - vert.pos.y ) / ( g_fEnd - g_fStart );
	vsout.diffuse = float4( g_Color, 1.0f - f );
	
  	
	return vsout;
}