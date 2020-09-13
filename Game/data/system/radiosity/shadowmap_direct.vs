struct VS_Input
{
	float4 pos : POSITION0;
};

struct VS_Output
{
	float4 pos : POSITION;
	float2 uv0 : TEXCOORD0;
};

VS_Output main(VS_Input myin)
{
	VS_Output myout=(VS_Output)0;
	
	myout.pos.x=myin.pos.x*2-1;
	myout.pos.y=-(myin.pos.y*2-1);
	myout.pos.z=1;
	myout.pos.w=1;
	
	myout.uv0.xy=myin.pos.xy;
	
	return myout;
}