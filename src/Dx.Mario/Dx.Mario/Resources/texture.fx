//==================================================================
//Phong shading
//==================================================================

// Parâmetros de transformação
uniform extern float4x4 gWorld;
uniform extern float4x4 gInverseWorld;
uniform extern float4x4 gView;
uniform extern float4x4 gProjection;

// ambiente
uniform extern float4 gCameraPos;
uniform extern float4 gLightDir;
uniform extern float4 gAmbientColor;
uniform extern float4 gDiffuseColor;
uniform extern float4 gSpecularColor;

// material
uniform extern float4 gAmbientMaterial;
uniform extern float4 gDiffuseMaterial;
uniform extern float4 gSpecularMaterial;
uniform extern float gSpecularPower;
uniform extern texture gTexture;
uniform extern texture gDiffuserTexture;

// Sampler
sampler TesS = sampler_state
{
    Texture = < gTexture >;
    MinFilter = Anisotropic;
    MaxAnisotropy = 8;
    //MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler DiffuserSample = sampler_state
{
	Texture = < gDiffuserTexture >;
	MinFilter = Anisotropic;
	MaxAnisotropy = 8;
	//MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

// Estrutura
struct OutputVS
{
	float4 posH : POSITION0;
	float2 tex0 : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 V : TEXCOORD2;
};

// Vertex shader
OutputVS TransformVS(float3 posL : POSITION0, float3 normal : NORMAL0, float2 tex0 : TEXCOORD0)
{
    // Zeramos a estrutura
    OutputVS outVS = (OutputVS)0;

    // Transforma no espaço de coordenadas homogêneo
    float4 posW = mul(float4(posL, 1.0f), gWorld);
    float4 posWV = mul(posW, gView);
    float4 posWVP = mul(posWV, gProjection);
    outVS.posH = posWVP;

    outVS.N = mul(float4(normal, 0), gWorld).xyz;
    outVS.V = (gCameraPos - posW).xyz;

    //Repassamos a textura para processa-la na rasterização (pixel shader)
    outVS.tex0 = tex0;
    return outVS;
}

// Pixel shader
float4 TransformPS(float2 tex0 : TEXCOORD0, float3 N : TEXCOORD1, float3 V : TEXCOORD2) : COLOR
{
    float3 normal = normalize(N);
    float3 toCamera = normalize(V);
    //Cálculo do componente ambiente
    float3 ambient = (gAmbientColor * gAmbientMaterial).rgb;

    //Cálculo do componente difuso
    float3 lightDir = gLightDir.xyz;
    float diffuseIntensity = max(dot(-lightDir, normal), 0.0f);
    float3 diffuse = (gDiffuseColor * gDiffuseMaterial * diffuseIntensity).rgb;

    //Cálculo do componente especular
    float3 reflex = reflect(lightDir, normal);
    float specularIntensity = pow(saturate(dot(reflex, toCamera)), gSpecularPower);
    float3 specular = (gSpecularColor * gSpecularMaterial * specularIntensity).rgb;

    //Soma das luzes
    float3 lighting = (ambient.rgb + diffuse.rgb) * tex2D(TesS, tex0).rgb;
    return saturate(float4(lighting + specular.rgb, gAmbientMaterial.a));
}

/////////////////////////
// Toon

struct OutputToonVS
{
	float4 Pos : POSITION0;
	float2 Tex : TEXCOORD0;
	float3 L : TEXCOORD1;
	float3 N : TEXCOORD2;
};

OutputToonVS ToonVS(float3 posL : POSITION0, float3 normal : NORMAL0, float2 tex0 : TEXCOORD0)
{
	// Transforma no espaço de coordenadas homogêneo
	float4 posW = mul(float4(posL, 1.0f), gWorld);
	float4 posWV = mul(posW, gView);
	float4 posWVP = mul(posWV, gProjection);

	OutputToonVS Out = (OutputToonVS)0;
	Out.Pos = posWVP;
	Out.Tex = tex0;
	Out.L = normalize(gCameraPos - gLightDir);
	Out.N = normalize(mul(gInverseWorld, normal));

	return Out;
}

float4 ToonPS(float2 Tex: TEXCOORD0, float3 L : TEXCOORD1, float3 N : TEXCOORD2) : COLOR
{
	float4 Color = TransformPS(Tex, N, L);

	float Ai = 0.7f;
	float4 Ac = float4(1.0, 1.0, 1.0, 1.0);
	float Di = 0.35f;
	float4 Dc = float4(1.0, 1.0, 1.0, 1.0);

	Tex.y = 0.0f;
	Tex.x = saturate(dot(L, N));

	float4 CelColor = tex2D(DiffuserSample, Tex);

	return (Ai*Ac*Color) + (Color*Di*Dc*CelColor);
}

technique PhongTech
{
    pass P0
    {
        // Especifica o vertex e pixel shader associado a essa passada.
        vertexShader = compile vs_2_0 TransformVS();
        pixelShader = compile ps_2_0 TransformPS();

        //Especifica o device state associado a essa passada.
        FillMode = Solid;
    }
}

technique Toon
{
	pass P0
	{
		// Especifica o vertex e pixel shader associado a essa passada.
		vertexShader = compile vs_2_0 ToonVS();
		pixelShader = compile ps_2_0 ToonPS();

		//Especifica o device state associado a essa passada.
		FillMode = Solid;
	}
}
