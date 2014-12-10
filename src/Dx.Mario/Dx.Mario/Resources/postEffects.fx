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
}

float4 DarkerPS(float2 tex0 : TEXCOORD0) : COLOR
{
    return tex2D(gTextureSampler, tex0) / 4;
}

float4 InversePS(float2 tex0 : TEXCOORD0) : COLOR
{
    float4 color = tex2D(gTextureSampler, tex0);
    return float4(1-color.r, 1-color.g, 1-color.b, color.a);
}

technique Default
{
	pass P0
	{
		vertexShader = compile vs_2_0 TransformVS();
		pixelShader = compile ps_2_0 TransformPS();
        FillMode = Solid;
	}
}

technique Darker
{
    pass P0
    {
        vertexShader = compile vs_2_0 TransformVS();
        pixelShader = compile ps_2_0 DarkerPS();
        FillMode = Solid;
    }
};

technique Inverse
{
    pass P0
    {
        vertexShader = compile vs_2_0 TransformVS();
        pixelShader = compile ps_2_0 InversePS();
        FillMode = Solid;
    }
};
