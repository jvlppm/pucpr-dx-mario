//==================================================================
//Phong shading
//==================================================================

uniform extern texture gTexture;
uniform extern texture gToonLightTexture;

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

// Toon color sampler
sampler gToonSampler = sampler_state
{
    Texture = < gToonLightTexture >;
    MinFilter = Anisotropic;
    MaxAnisotropy = 8;
    MagFilter = None;
    MipFilter = None;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

/////////////////////////
// Helper functions
/////////////////////////

float3 HUEtoRGB(in float H)
{
    float R = abs(H * 6 - 3) - 1;
    float G = 2 - abs(H * 6 - 2);
    float B = 2 - abs(H * 6 - 4);
    return saturate(float3(R, G, B));
}

float3 HSLtoRGB(in float3 HSL)
{
    float3 RGB = HUEtoRGB(HSL.x);
        float C = (1 - abs(2 * HSL.z - 1)) * HSL.y;
    return (RGB - 0.5) * C + HSL.z;
}

float Epsilon = 1e-10;

float3 RGBtoHCV(in float3 RGB)
{
    // Based on work by Sam Hocevar and Emil Persson
    float4 P = (RGB.g < RGB.b) ? float4(RGB.bg, -1.0, 2.0 / 3.0) : float4(RGB.gb, 0.0, -1.0 / 3.0);
    float4 Q = (RGB.r < P.x) ? float4(P.xyw, RGB.r) : float4(RGB.r, P.yzx);
    float C = Q.x - min(Q.w, Q.y);
    float H = abs((Q.w - Q.y) / (6 * C + Epsilon) + Q.z);
    return float3(H, C, Q.x);
}

float3 RGBtoHSL(in float3 RGB)
{
    float3 HCV = RGBtoHCV(RGB);
    float L = HCV.z - HCV.y * 0.5;
    float S = HCV.y / (1 - abs(L * 2 - 1) + Epsilon);
    return float3(HCV.x, S, L);
}

/////////////////////////
// Default
/////////////////////////

OutputVS TransformVS(float2 posL : POSITION0, float2 tex0 : TEXCOORD0)
{
    OutputVS outVS = (OutputVS)0;
    outVS.pos = float4(posL, 0.0f, 1.0f);
    outVS.tex0 = tex0;
    return outVS;
}

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

/////////////////////////
// Darker
/////////////////////////

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

/////////////////////////
// Inverse
/////////////////////////

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

/////////////////////////
// Grayscale
/////////////////////////

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

/////////////////////////
// Toon
/////////////////////////

float4 ToonColor(float4 color) : COLOR
{
    const float ToonMinThreshold = 0.1f;
    const float ToonEffect = 0.75;
    const float Ac = (1 - ToonEffect);

    float3 hsl = RGBtoHSL(color.xyz);
    if (hsl.z > ToonMinThreshold && hsl.z < 1 - ToonMinThreshold)
        hsl.z = tex2D(gToonSampler, float2(hsl.z, 0.0f)).x;
    float3 newColor = HSLtoRGB(hsl);

    return color * Ac + float4(newColor, color.w) * ToonEffect;
}

float4 SobelPS(float2 tex0: TEXCOORD0) : COLOR
{
    const int NUM = 9;
    const float threshold = 2;

    const float2 c[NUM] = {
        float2(-0.00001, 0.00001),
        float2(0.0,     0.00001),
        float2(0.00001, 0.00001),
        float2(-0.00001, 0.00),
        float2(0.0,       0.0),
        float2(0.00001, 0.00001),
        float2(-0.00001,-0.00001),
        float2(0.0,    -0.00001),
        float2(0.00001,-0.00001),
    };

    float3 col[NUM];
    int i;

    [unroll(NUM)]for (i = 0; i < NUM; i++) {
        col[i] = tex2D(gTextureSampler, tex0 + c[i]);
    }

    float lum[NUM];
    [unroll(NUM)]for (i = 0; i < NUM; i++) {
        lum[i] = (col[i].x + col[i].y + col[i].z) / 3;
    }

    float x = lum[1] + 2 * lum[2] - lum[3] + lum[5] - 2 * lum[5] - lum[6];
    float y = -2 * lum[0] - lum[1] - lum[3] + lum[5] + lum[7] + 2 * lum[8];

    float edge = ((x*x + y*y) / threshold) * 4;
    //edge = edge > 0.5f ? edge : 0;

    return float4(edge.xxx, 1.0);
}

float4 ToonPS(float2 tex0: TEXCOORD0) : COLOR
{
    float4 original = tex2D(gTextureSampler, tex0);
    float4 toon = ToonColor(original);
    float4 border = SobelPS(tex0);

    return (toon - border);
}

technique Toon
{
    pass P0
    {
        vertexShader = compile vs_2_0 TransformVS();

        // ps_2_0 limita número de instruções, impedindo que conversões de cores sejam aplicadas facilmente
        pixelShader = compile ps_3_0 ToonPS();
        FillMode = Solid;
    }
};
