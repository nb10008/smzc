struct tagInput
{
	float4 pos 	: POSITION0;
	float3 normal 	: NORMAL0;
	float4 diffuse	: COLOR0;
	float2 uv0 	: TEXCOORD0;
};

struct tagOutput
{
	float4 pos 	: POSITION;
	float4 diffuse	: COLOR0;
	float2 uv0 	: TEXCOORD0;
};

float4x4 g_matWorld;

tagOutput main(tagInput myin)
{
	tagOutput myout=(tagOutput)0;
	
	myout.pos.x=myin.uv0.x*2-1;
	myout.pos.y=-(myin.uv0.y*2-1);
	myout.pos.z=1;
	myout.pos.w=1;
	
	myout.diffuse=myin.diffuse;
	myout.uv0=myin.uv0;
	
	return myout;
}

