//==================================================================
//Phong shading
//==================================================================

// Par�metros de transforma��o
uniform extern float4x4 gWorld;
uniform extern float4x4 gView;
uniform extern float4x4 gProjection;

//Componente ambiente
uniform extern float4 gAmbientColor;
uniform extern float4 gAmbientMaterial;

//Componente difuso
uniform extern float4 gDiffuseColor;
uniform extern float4 gDiffuseMaterial;
uniform extern float4 gLightDir;

//Componente especular
uniform extern float4 gSpecularColor;
uniform extern float4 gSpecularMaterial;
uniform extern float gSpecularPower;

uniform extern float4 gCameraPos;

//Texturas
uniform extern texture gTexture;

// Estrutura
struct OutputVS
{
	float4 posH : POSITION0;
	float2 tex0 : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 V : TEXCOORD2;
};

// Sampler
sampler TesS = sampler_state
{
	Texture = <gTexture>;
	MinFilter = Anisotropic;
	MaxAnisotropy = 8;	
	//MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// Vertex shader
OutputVS TransformVS(float3 posL : POSITION0, float3 normal : NORMAL0, float2 tex0 : TEXCOORD0)
{
	// Zeramos a estrutura
	OutputVS outVS = (OutputVS)0;

	// Transforma no espa�o de coordenadas homog�neo
	float4 posW = mul(float4(posL, 1.0f), gWorld);
	float4 posWV = mul(posW, gView);
	float4 posWVP = mul(posWV, gProjection);      
	outVS.posH = posWVP;
	  	  
	outVS.N = mul(float4(normal,0), gWorld).xyz;
	outVS.V = (gCameraPos - posW).xyz;

	//Repassamos a textura para processa-la na rasteriza��o (pixel shader)
	outVS.tex0 = tex0;
	return outVS;
}

// Pixel shader
float4 TransformPS(float3 tex0 : TEXCOORD0, float3 N : TEXCOORD1, float3 V : TEXCOORD2) : COLOR
{	
	float3 normal = normalize(N);
	float3 toCamera = normalize(V);
	//C�lculo do componente ambiente
	float3 ambient = (gAmbientColor * gAmbientMaterial).rgb;

	//C�lculo do componente difuso
	float3 lightDir = gLightDir.xyz;
	float diffuseIntensity = max(dot(-lightDir, normal), 0.0f);
	float3 diffuse = (gDiffuseColor * gDiffuseMaterial * diffuseIntensity).rgb;

	//C�lculo do componente especular
	float3 reflex = reflect(lightDir, normal);	
	float specularIntensity = pow(saturate(dot(reflex, toCamera)), gSpecularPower);
	float3 specular = (gSpecularColor * gSpecularMaterial * specularIntensity).rgb;
	
	//Soma das luzes
	float3 lighting = (ambient.rgb + diffuse.rgb) * tex2D(TesS, tex0).rgb;
	return saturate(float4(lighting + specular.rgb, gAmbientMaterial.a));
}

technique PhongTech
{
	pass P0
	{
		// Especifica o vertex e pixel shader associado a essa passada.            
		vertexShader = compile vs_2_0 TransformVS();
		pixelShader  = compile ps_2_0 TransformPS();

		//Especifica o device state associado a essa passada. 
		FillMode = Solid;
	}
}