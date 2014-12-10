//==================================================================
//Phong shading
//==================================================================

uniform extern texture gTexture;

// Estrutura
struct OutputVS
{
	float4 pos : POSITION0;
	float2 tex0 : TEXCOORD0;
};

// Sampler
sampler gTextureSampler = sampler_state
{
	Texture = <gTexture>;
};

// Vertex shader
OutputVS TransformVS(float2 posL : POSITION0, float2 tex0 : TEXCOORD0)
{
	OutputVS outVS = (OutputVS)0;
	outVS.pos = float4(posL, 0.0f, 1.0f);
	outVS.tex0 = tex0;
	return outVS;
}

// Pixel shader
float4 TransformPS(float2 tex0 : TEXCOORD0) : COLOR
{
	return tex2D(gTextureSampler, tex0);
	//return float4(1, 1, 0, 1);
}

technique Default
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
