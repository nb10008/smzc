float4x4 g_matWorldViewPrj;
float4x4 g_matWorld;
float3	 g_directDir;
float3	 g_eyePos;
float	 g_pointRange;
float3	 g_pointPos;
float	 g_pointAtten;

struct VS_INPUT
{
	float4 pos 	: POSITION;
	float2 uv 	: TEXCOORD0;
	float3 normal 	: NORMAL;
	float3 tangent 	: TEXCOORD1;
};

struct VS_OUTPUT
{
	float4 pos 	: POSITION;
	float2 uv 	: TEXCOORD0;
	float3 directDirInTexSpace 	: TEXCOORD1;
	float3 directHalfDirInTexSpace 	: TEXCOORD2;
	float3 pointHalfDirInTexSpace   : TEXCOORD3;
	float  pointAtten	  	: TEXCOORD4;
};

VS_OUTPUT main(VS_INPUT vert)
{
	VS_OUTPUT vsout = (VS_OUTPUT)0;
  	
  	//--pos 
	vsout.pos = mul(float4(vert.pos.xyz,1.0f),g_matWorldViewPrj);	
	
	//--uv
	vsout.uv = vert.uv;
	
	//--directDirInTexSpace and  directHalfDirInTexSpace
	float3 worldPos = mul(float4(vert.pos.xyz,1.0f),g_matWorld).xyz;
	float3 viewDir = g_eyePos - worldPos;
	
    	float3x3 worldToTangentSpace;
    	worldToTangentSpace[0] = mul(vert.tangent, g_matWorld);
    	worldToTangentSpace[1] = mul(cross(vert.normal, vert.tangent), g_matWorld);
    	worldToTangentSpace[2] = mul(vert.normal, g_matWorld);
    
    	vsout.directDirInTexSpace = normalize(mul(worldToTangentSpace,g_directDir));
    	float3 viewDirInTexSpace = normalize(mul(worldToTangentSpace,viewDir));
    	vsout.directHalfDirInTexSpace = normalize(vsout.directDirInTexSpace + viewDirInTexSpace);
    	
    	//--point atten and half vector
     	float pointDist = distance(worldPos,g_pointPos);
    	if(pointDist < g_pointRange)
    	{
    		vsout.pointAtten = 1 / (pointDist * g_pointAtten);
    		
    		float3 pointDir = g_pointPos - worldPos;
    		float3 pointDirInTexSpace = normalize(mul(worldToTangentSpace,pointDir));
    		vsout.pointHalfDirInTexSpace = normalize(pointDirInTexSpace + viewDirInTexSpace);
    	}
    	
	return vsout;
}
