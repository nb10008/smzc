float4x4 	g_matWorldViewPrj;
float4x4 	g_matViewPrj;
float2      g_vUVTranslation;
float3 		g_eyePos;

float3		g_directDir;

float4		g_specularMtl;
float4		g_specularLightIntensity;
float		g_time;
float3x3 GetTangentSpaceBasis(float3 T, float3 N)
{
   float3x3 objToTangentSpace;
   
   objToTangentSpace[0]=T;           // tangent
   objToTangentSpace[1]=cross(T, N); // binormal
   objToTangentSpace[2]=N;           // normal  
   
   return objToTangentSpace;
}

struct VS_INPUT
{
	float3 pos 		: POSITION;
	float2 bumpMap 	: TEXCOORD0;	
	float3 normal 	: NORMAL0;
	float2 alphaMap : TEXCOORD1;
	float3 tpos	: POSITION1;
	float3 ttex 	: NORMAL1;	
};

struct VS_OUTPUT
{
	float4 pos 			: POSITION;
	float4 specular 	: COLOR0;
	float3 alphaMap 	: TEXCOORD0;
	float3 reflectMap   : TEXCOORD1;
	float3 refractMap   : TEXCOORD2;
	float4 Wave0		: TEXCOORD3; 
	float4 Wave1		: TEXCOORD4;
	float3 viewDirInTexSpace : TEXCOORD5;
	float3 halfDirInTexSpace : TEXCOORD6;
};

VS_OUTPUT main(VS_INPUT vert)
{
	VS_OUTPUT vsout = (VS_OUTPUT)0;
	float depth = vert.pos.y-vert.tpos.y;
	vsout.alphaMap.z =30*pow(0.5*sin(g_time/3+depth/1000)+0.5, 50)+depth;
	//--pos
	vert.pos += vsout.alphaMap.z-depth;
	float4 vPos = float4(vert.pos.xyz,1);
    vsout.pos = mul(vPos,g_matWorldViewPrj);

	//--specular
	vsout.specular = g_specularMtl * g_specularLightIntensity;

	//--uv
	vsout.alphaMap.xy = vert.alphaMap;
	float4 reflectMap = mul(float4(vert.pos,1), g_matViewPrj);
	vsout.reflectMap.xyz=reflectMap.xyw;
	
	float4 vHPos = vsout.pos;
	vHPos.y = -vHPos.y;
   	vsout.refractMap.xy = (vHPos.xy + vHPos.w)*0.5;    
    vsout.refractMap.z = vHPos.w;
	
	// Scale texture coordinates to get mix of low/high frequency details
    float2 vTexCoords=vert.bumpMap;
    vsout.Wave0.xy = vTexCoords.xy+g_vUVTranslation*2.0;
    vsout.Wave0.zw = vTexCoords.xy*2.0+g_vUVTranslation*4.0;
    vsout.Wave1.xy = vTexCoords.xy*4.0+g_vUVTranslation*2.0;
    vsout.Wave1.zw = vTexCoords.xy*8.0+g_vUVTranslation;

    // get tangent space basis
	float3 vTangent = float3(1.0f,0.0f,0.0f);
    
    float3x3 objToTangentSpace=GetTangentSpaceBasis(vTangent.xyz, vert.normal.xyz);
	            
    float3 vViewDir=g_eyePos.xyz-vPos;
    vsout.viewDirInTexSpace = normalize(mul(objToTangentSpace,vViewDir));
    float3 LightDirInTexSpace = normalize(mul(objToTangentSpace,g_directDir));
	vsout.halfDirInTexSpace = normalize( vsout.viewDirInTexSpace + LightDirInTexSpace );

	return vsout;
}