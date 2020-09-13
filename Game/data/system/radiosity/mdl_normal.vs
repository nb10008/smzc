struct tagInput
{
	float4 pos 	: POSITION0;
	float3 normal 	: NORMAL0;
	float2 uv1 	: TEXCOORD1;
};

struct tagOutput
{
	float4 pos 	: POSITION;
	float3 uv0 	: TEXCOORD0;
};

float4x4 g_matWorldInvTrans;

tagOutput main(tagInput myin)
{
	tagOutput myout=(tagOutput)0;
	
	myout.pos.x=myin.uv1.x*2-1;
	myout.pos.y=-(myin.uv1.y*2-1);
	myout.pos.z=1;
	myout.pos.w=1;
	
	myout.uv0.xyz=mul(myin.normal,g_matWorldInvTrans).xyz;
	//myout.uv0.xyz=myin.normal.xyz;
	
	return myout;
}

