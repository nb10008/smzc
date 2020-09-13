float4x4 g_matWorldViewPrj;
float4x4 g_matWorld;
float3	 g_directDir;
float3	 g_eyePos;


struct VS_INPUT
{
	float3 pos 	: POSITION;
	float3 normal 	: NORMAL;
	float3 tangent 	: TEXCOORD1;
	float2 uv 	: TEXCOORD0;	
};

struct VS_OUTPUT
{
	float4 pos 	: POSITION;
	float2 uv 	: TEXCOORD0;
	float3 lightDirInTexSpace : TEXCOORD1;
	float3 halfDirInTexSpace  : TEXCOORD2;
};

VS_OUTPUT main(VS_INPUT vert)
{
	VS_OUTPUT vsout = (VS_OUTPUT)0;

	//--pos
  	vsout.pos = mul(float4(vert.pos.xyz,1.0f),g_matWorldViewPrj);
  	
  	//--uv
	vsout.uv = vert.uv;
	
	//--lightDirInTexSpace and halfDirInTexSpace
	float3 worldPos = mul(float4(vert.pos,1.0f),g_matWorld).xyz;
	float3 viewDir = g_eyePos - worldPos;
	
    	float3x3 worldToTangentSpace;
    	worldToTangentSpace[0] = mul(vert.tangent, g_matWorld);
    	worldToTangentSpace[1] = mul(cross(vert.normal, vert.tangent), g_matWorld);
    	worldToTangentSpace[2] = mul(vert.normal, g_matWorld);
    
    	vsout.lightDirInTexSpace = normalize(mul(worldToTangentSpace,g_directDir));
    	float3 viewDirInTexSpace = normalize(mul(worldToTangentSpace,viewDir));
     	vsout.halfDirInTexSpace = normalize(vsout.lightDirInTexSpace + viewDirInTexSpace);
    	
	return vsout;
}
