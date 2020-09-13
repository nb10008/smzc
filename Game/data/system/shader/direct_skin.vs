float3 g_directDir = {-1.0f, 1.0f, -1.0f};    		
float4 g_directDiffuse = {0.6f, 0.6f, 0.6f, 1.0f}; 
float4 g_directAmbient = {0.1f, 0.1f, 0.1f, 1.0f};

static const int MAX_MATRICES = 80;
float4x3  g_boneMatrix[MAX_MATRICES];
float4x4  g_matWorldViewPrj;
float4x4  g_matWorld;

struct VS_INPUT
{
    float4  pos             : POSITION;
    float3  normal          : NORMAL; 
    float4  blendWeights    : BLENDWEIGHT;
    float4  blendIndices    : BLENDINDICES;
    float3  uv0             : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  pos     : POSITION;
    float4  diffuse : COLOR;
    float2  uv0     : TEXCOORD0;
    float2  uv1     : TEXCOORD1;
};


float3 Diffuse(float3 normal)
{
    float cosTheta = max(0.0f, dot(normal, g_directDir.xyz));
    return (cosTheta);
}


VS_OUTPUT main(VS_INPUT i)
{
    VS_OUTPUT o = (VS_OUTPUT)0;
   
    //--pos and normal in model space
    float3  pos = 0.0f;
    float3  normal = 0.0f;    
 
    int4 indexVector = D3DCOLORtoUBYTE4(i.blendIndices);

    float blendWeightsArray[4] = (float[4])i.blendWeights;
    int   indexArray[4]        = (int[4])indexVector;
    
    pos += mul(i.pos, g_boneMatrix[indexArray[0]]) * blendWeightsArray[0];
    normal += mul(i.normal, g_boneMatrix[indexArray[0]]) * blendWeightsArray[0];
    pos += mul(i.pos, g_boneMatrix[indexArray[1]]) * blendWeightsArray[1];
    normal += mul(i.normal, g_boneMatrix[indexArray[1]]) * blendWeightsArray[1];
    pos += mul(i.pos, g_boneMatrix[indexArray[2]]) * blendWeightsArray[2];
    normal += mul(i.normal, g_boneMatrix[indexArray[2]]) * blendWeightsArray[2];
    pos += mul(i.pos, g_boneMatrix[indexArray[3]]) * blendWeightsArray[3];
    normal += mul(i.normal, g_boneMatrix[indexArray[3]]) * blendWeightsArray[3];
      
    //--pos
    o.pos = mul(float4(pos.xyz,1.0f), g_matWorldViewPrj);

    //--diffuse
    normal = normalize(mul(normal,g_matWorld));
    o.diffuse.xyz = g_directAmbient.xyz + Diffuse(normal) * g_directDiffuse.xyz;
    o.diffuse.w = g_directDiffuse.w;
   
    //--uv0
    o.uv0 = i.uv0;
    o.uv1 = i.uv0;
    
    //--曝光效果预先除以2
    o.diffuse.rgb*=0.5f;    

    return o;
}