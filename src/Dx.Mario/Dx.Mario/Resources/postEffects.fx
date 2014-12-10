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

technique Default
{
	pass P0
	{
		vertexShader = compile vs_2_0 TransformVS();
		pixelShader = compile ps_2_0 TransformPS();
        FillMode = Solid;
	}
}

float4 DarkerPS(float2 tex0 : TEXCOORD0) : COLOR
{
    return tex2D(gTextureSampler, tex0) / 4;
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

float4 InversePS(float2 tex0 : TEXCOORD0) : COLOR
{
    float4 color = tex2D(gTextureSampler, tex0);
    return float4(1 - color.r, 1 - color.g, 1 - color.b, color.a);
}

technique Inverse
{
    pass P0
    {
        vertexShader = compile vs_2_0 TransformVS();
        pixelShader = compile ps_2_0 InversePS();
        FillMode = Solid;
    }
};

float4 GrayscalePS(float2 tex0 : TEXCOORD0) : COLOR
{
    float4 color = tex2D(gTextureSampler, tex0);
    float s = (color.r + color.g + color.b) / 3;
    return float4(s, s, s, color.a);
}

technique Grayscale
{
    pass P0
    {
        vertexShader = compile vs_2_0 TransformVS();
        pixelShader = compile ps_2_0 GrayscalePS();
        FillMode = Solid;
    }
};

float4 EdgesPS(float2 tex0 : TEXCOORD0) : COLOR
{
    const int NUM = 9;
    const float threshold = 1;

    const float2 c[NUM] = {
        float2(-0.0078125, 0.0078125),
        float2(0.00 ,     0.0078125),
        float2(0.0078125, 0.0078125),
        float2(-0.0078125, 0.00),
        float2(0.0,       0.0),
        float2(0.0078125, 0.007),
        float2(-0.0078125,-0.0078125),
        float2(0.00 ,    -0.0078125),
        float2(0.0078125,-0.0078125),
    };

    float3 col[NUM];
    int i;

    for (i = 0; i < NUM; i++) {
        col[i] = tex2D(gTextureSampler, tex0 + c[i] / 8.0);
    }

    float3 rgb2lum = float3(0.2f, 0.2f, 0.2f);

    float lum[NUM];
    for (i = 0; i < NUM; i++) {
        lum[i] = dot(col[i].xyz, rgb2lum);
    }

    float x = lum[2] + lum[8] + 2 * lum[5] - lum[0] - 2 * lum[3] - lum[6];
    float y = lum[6] + 2 * lum[7] + lum[8] - lum[0] - 2 * lum[1] - lum[2];

    float edge = ((x*x + y*y) / threshold) * 4;
    //edge = edge > 0.5f ? edge : 0;

    return float4(col[4] - edge.xxx, 1.0);
}

technique Edges
{
    pass P0
    {
        vertexShader = compile vs_2_0 TransformVS();
        pixelShader = compile ps_2_0 EdgesPS();
        FillMode = Solid;
    }
};
