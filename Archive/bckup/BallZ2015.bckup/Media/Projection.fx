texture BaseTexture : TEXTURE0;
sampler BaseSampler = sampler_state 
{ 
	Texture = (BaseTexture); 
	MinFilter = Point;
	MagFilter = Point;
};

struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
};
struct VS_OUTPUT
{
	float4 ProjectedTC : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 View : TEXCOORD2;
};
#define PS_INPUT VS_OUTPUT

float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
float4x4 World : WORLD;

float3 ViewPosition : VIEWPOSITION;

float FresnelPower;
float MinReflection, MaxReflection;

VS_OUTPUT VS(VS_INPUT In, out float4 OutPosition : POSITION)
{
	OutPosition = mul(In.Position, WorldViewProjection);

	VS_OUTPUT Out;

	Out.ProjectedTC = (OutPosition + OutPosition.w) * 0.5;
	Out.ProjectedTC.xy *= -1;

	Out.Normal = In.Normal;
	Out.View = ViewPosition - mul(In.Position, World).xyz;

	return Out;
}

float Fresnel(float3 i, float3 n) 
{
	return pow(1 - dot(i, n), FresnelPower);
}

float4 PS_Darken(PS_INPUT In) : COLOR 
{
	float3 view = normalize(In.View);
	return float4(MinReflection + 1 - Fresnel(view, In.Normal).xxx, 1);
}

float4 PS_Add(PS_INPUT In) : COLOR 
{
	float3 sample = tex2Dproj(BaseSampler, In.ProjectedTC).rgb;

	float3 view = normalize(In.View);
	float fresnel = Fresnel(view, In.Normal);
	sample *= fresnel * (MaxReflection - MinReflection) + MinReflection;

	return float4(sample, 1);
}

technique Main
{
	pass Darken
	{
		AlphaBlendEnable = true;
		ZWriteEnable = false;
		SrcBlend = DestColor;
		DestBlend = Zero;

		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS_Darken();    
	}
	pass Add 
	{
		SrcBlend = One;
		DestBlend = One;

		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS_Add();    
	}
}
