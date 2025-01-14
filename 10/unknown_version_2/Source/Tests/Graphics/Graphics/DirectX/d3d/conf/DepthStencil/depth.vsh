// Transform and pass through vertex shader

struct VSIn
{
	float3 pos	: pos;
};

struct VSOut
{
	float4 pos		: SV_Position;
	float  odepth   : odepth;
};

cbuffer cb0 : register( b0 )
{	
	float4		inv_vp;		    // inverse viewport
	float		default_depth;  // used when odepth is enabled
	uint		rt_index;		// render target array index
};

VSOut main( VSIn input )
{
	VSOut output;
	
	output.pos = float4( 0.f, 0.f, input.pos.z, 1.f );
	output.pos.xy = input.pos.xy * inv_vp.zw + inv_vp.xy;
	output.odepth = 0.f;	// not used
	
	return output;
}