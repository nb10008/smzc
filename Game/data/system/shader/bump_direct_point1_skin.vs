float4x4 g_matWorldViewPrj;
float4x4 g_matWorld;
float3	 g_directDir;
float3	 g_eyePos;
float	 g_pointRange;
float3	 g_pointPos;
float	 g_pointAtten;

static const int MAX_MATRICES = 80;
float4x3 g_boneMatrix[MAX_MATRICES] : WORLDMATRIXARRAY;

struct VS_INPUT
{
	float4 pos 	: POSITION;
	float3 normal 	: NORMAL;
	float3 tangent 	: TANGENT;
   	float4 blendWeights  	: BLENDWEIGHT;
    	float4 blendIndices  	: BLENDINDICES;		
    	float2 uv 	: TEXCOORD0;	
};

struct VS_OUTPUT
{
	float4 pos 	: POSITION;
	float2 uv 	: TEXCOORD0;
	float3 directDirInTexSpace 	: TEXCOORD1;
	float3 directHalfDirInTexSpace 	: TEXCOORD2;
	float3 pointHalfDirInTexSpace   : TEXCOORD3;
	float  pointAtten	  	: TEXCOORD4;
	float3 pointDirInTexSpace		: TEXCOORD5;
};

VS_OUTPUT main(VS_INPUT vert)
{
	VS_OUTPUT vsout = (VS_OUTPUT)0;
	
	//--skin pos, normal and tangent in model space
  	float3 pos = 0.0f;
    	float3 normal = 0.0f;    
    	float3 tangent = 0.0f;
	
  	int4 indexVector = D3DCOLORtoUBYTE4(vert.blendIndices);

    	float blendWeightsArray[4] = (float[4])vert.blendWeights;
    	int   indexArray[4]        = (int[4])indexVector;
    
    	pos += mul(vert.pos, g_boneMatrix[indexArray[0]]) * blendWeightsArray[0];
   	 	normal += mul(vert.normal, g_boneMatrix[indexArray[0]]) * blendWeightsArray[0];
   	 	tangent += mul(vert.tangent, g_boneMatrix[indexArray[0]]) * blendWeightsArray[0];
    	pos += mul(vert.pos, g_boneMatrix[indexArray[1]]) * blendWeightsArray[1];
   	 	normal += mul(vert.normal, g_boneMatrix[indexArray[1]]) * blendWeightsArray[1];
   	 	tangent += mul(vert.tangent, g_boneMatrix[indexArray[1]]) * blendWeightsArray[1];
    	pos += mul(vert.pos, g_boneMatrix[indexArray[2]]) * blendWeightsArray[2];
   	 	normal += mul(vert.normal, g_boneMatrix[indexArray[2]]) * blendWeightsArray[2];
   	 	tangent += mul(vert.tangent, g_boneMatrix[indexArray[2]]) * blendWeightsArray[2];
    	pos += mul(vert.pos, g_boneMatrix[indexArray[3]]) * blendWeightsArray[3];
   	 	normal += mul(vert.normal, g_boneMatrix[indexArray[3]]) * blendWeightsArray[3];
   	 	tangent += mul(vert.tangent, g_boneMatrix[indexArray[3]]) * blendWeightsArray[3];
      
  	//--pos
	vsout.pos = mul(float4(pos.xyz,1),g_matWorldViewPrj);	
	
	//--uv
	vsout.uv = vert.uv;
	
	//--directDirInTexSpace and directHalfDirInTexSpace
	float3 worldPos = mul(float4(pos.xyz,1.0f),g_matWorld);
	float3 viewDir = g_eyePos - worldPos;
	
    	float3x3 worldToTangentSpace;
    	worldToTangentSpace[0] = mul(tangent, g_matWorld);
    	worldToTangentSpace[1] = mul(cross(normal, tangent), g_matWorld);
    	worldToTangentSpace[2] = mul(normal, g_matWorld);
    
    	vsout.directDirInTexSpace = normalize(mul(worldToTangentSpace,g_directDir));
    	float3 viewDirInTexSpace = normalize(mul(worldToTangentSpace,viewDir));
    	vsout.directHalfDirInTexSpace = normalize(vsout.directDirInTexSpace + viewDirInTexSpace);
    	
    	//--point atten and half vector
     	float pointDist = distance(worldPos,g_pointPos);
    	if(pointDist < g_pointRange)
    	{
    		vsout.pointAtten = 1 / (pointDist * g_pointAtten);
    		
    		float3 pointDir = g_pointPos - worldPos;
    		vsout.pointDirInTexSpace = normalize(mul(worldToTangentSpace,pointDir));
    		vsout.pointHalfDirInTexSpace = normalize(vsout.pointDirInTexSpace + viewDirInTexSpace);
    	}
    	
	return vsout;
}



