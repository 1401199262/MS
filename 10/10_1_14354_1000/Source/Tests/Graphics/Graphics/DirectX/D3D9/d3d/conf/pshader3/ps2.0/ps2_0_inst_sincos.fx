// Tests for Instruction: sincos
#include "include.fx"

#define PS_SETUP \
	ps_2_0


// sets green, then removes green if either red or blue channels are set:
#define PS_END \
	mov r2.ga, c0.x \
	add_sat r2.g, r2.g, -r2.r \
	add_sat r2.g, r2.g, -r2.b \
	mov oC0, r2

#define SET_VS \
	VertexShaderConstant[0] = <mFinal>; \
	VertexShader = <NormalVS>;

#define SET_MAIN_PS_CONST \
	PixelShaderConstant[0] = {1.0, 0.0, 0.01, 0.05};

// Customized verify result macro, does not scale eps: outputs to r1 0-right and 1-wrong, per component:
#define VERIFY_RESULT \
	add r1, r0, -c1 \
	cmp r1, r1, r1, -r1 \
	add r1, r2, -r1 \
	cmp r1, r1, c0.y, c0.x    \
	mov r2, c0.y

// The following can be used to verify each component individually, since some
// instructions or macros may write to extra components and you do not want to verify those.
#define V_X add_sat r2.r, r2.r, r1.x
#define V_Y add_sat r2.r, r2.r, r1.y
#define V_Z add_sat r2.r, r2.r, r1.z
#define V_W add_sat r2.r, r2.r, r1.w

#define TESTCASE \
Technique \
< \
	String Name =
#define PASSBEGIN \
	; \
	String Shape = "TinyQuad"; \
> \
{ \
	Pass P0 \
	{
#define TESTBEGIN \
		PixelShader = asm { \
			PS_SETUP
#define TESTEND \
			PS_END \
		}; \
		SET_VS \
		SET_MAIN_PS_CONST \
		SET_PS_CONSTANTS \
	} \
}



#define SET_PS_CONSTANTS \
	PixelShaderConstant[8] = { -1.5500992e-006f, -2.1701389e-005f,  0.0026041667f, 0.00026041668f }; \
	PixelShaderConstant[9] = { -0.020833334f, -0.12500000f, 1.0f, 0.50000000f }; \
	PixelShaderConstantF[4] = {0.314159265359, -1.25663706144, 2.04203522483, -2.98451302091};

VertexShader NormalVS =
	asm
	{
		vs_2_0
		dcl_position	v0
		def c5, 0.314159265359, -1.25663706144, 2.04203522483, -2.98451302091
		def c6, 0.1, 0.4, 0.65, 0.95
		m4x4 oPos, v0, c0
		mov oT0, c5
		mov oD0, c6
	};


// Tests:

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// source registers * modifiers * swizzles
TESTCASE "sincos - Standard (0) sincos r0.xy, r4.x"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.951056516295, 0.309016994375, 0.0, 0.0
	mov r4, c4
	mov r0, c0.y
	mov r2, c0.z
	sincos r0.xy, r4.x, c8, c9
	mov r0.zw, c0.y
	VERIFY_RESULT
	V_X V_Y
	TESTEND

TESTCASE "sincos - Standard (1) sincos_pp r0.x, c4.y"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.309016994375, 0.0, 0.0, 0.0
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.x, c4.y, c8, c9
	mov r0.yzw, c0.y
	VERIFY_RESULT
	V_X
	TESTEND

TESTCASE "sincos - Standard (2) sincos r0.y, -v0.z"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.0, -0.605186405736, 0.0, 0.0
	dcl v0
	mov r0, c0.y
	mov r2, c0.w
	sincos r0.y, -v0.z, c8, c9
	mov r0.xzw, c0.y
	VERIFY_RESULT
	V_Y
	TESTEND

TESTCASE "sincos - Standard (3) sincos_pp r0.xy, -t0.w"
	PASSBEGIN
	TESTBEGIN
	def c1, -0.987688340595, 0.15643446504, 0.0, 0.0
	dcl t0
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.xy, -t0.w, c8, c9
	mov r0.zw, c0.y
	VERIFY_RESULT
	V_X V_Y
	TESTEND

TESTCASE "sincos - Standard (4) sincos_pp r0.y, -c4.x"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.0, -0.309016994375, 0.0, 0.0
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.y, -c4.x, c8, c9
	mov r0.xzw, c0.y
	VERIFY_RESULT
	V_Y
	TESTEND

TESTCASE "sincos - Standard (5) sincos_pp r0.y, -r4.y"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.0, 0.951056516295, 0.0, 0.0
	mov r4, c4
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.y, -r4.y, c8, c9
	mov r0.xzw, c0.y
	VERIFY_RESULT
	V_Y
	TESTEND

TESTCASE "sincos - Standard (6) sincos r0.x, t0.z"
	PASSBEGIN
	TESTBEGIN
	def c1, -0.45399049974, 0.0, 0.0, 0.0
	dcl t0
	mov r0, c0.y
	mov r2, c0.z
	sincos r0.x, t0.z, c8, c9
	mov r0.yzw, c0.y
	VERIFY_RESULT
	V_X
	TESTEND

TESTCASE "sincos - Standard (7) sincos r0.x, -v0.w"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.581683089464, 0.0, 0.0, 0.0
	dcl v0
	mov r0, c0.y
	mov r2, c0.w
	sincos r0.x, -v0.w, c8, c9
	mov r0.yzw, c0.y
	VERIFY_RESULT
	V_X
	TESTEND

TESTCASE "sincos - Standard (8) sincos r0.xy, v0.y"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.921060994003, 0.389418342309, 0.0, 0.0
	dcl v0
	mov r0, c0.y
	mov r2, c0.w
	sincos r0.xy, v0.y, c8, c9
	mov r0.zw, c0.y
	VERIFY_RESULT
	V_X V_Y
	TESTEND

TESTCASE "sincos - Standard (9) sincos r0.xy, c4.z"
	PASSBEGIN
	TESTBEGIN
	def c1, -0.45399049974, 0.891006524188, 0.0, 0.0
	mov r0, c0.y
	mov r2, c0.z
	sincos r0.xy, c4.z, c8, c9
	mov r0.zw, c0.y
	VERIFY_RESULT
	V_X V_Y
	TESTEND

TESTCASE "sincos - Standard (10) sincos_pp r0.y, r4.w"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.0, -0.15643446504, 0.0, 0.0
	mov r4, c4
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.y, r4.w, c8, c9
	mov r0.xzw, c0.y
	VERIFY_RESULT
	V_Y
	TESTEND

TESTCASE "sincos - Standard (11) sincos_pp r0.x, v0.x"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.995004165278, 0.0, 0.0, 0.0
	dcl v0
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.x, v0.x, c8, c9
	mov r0.yzw, c0.y
	VERIFY_RESULT
	V_X
	TESTEND

TESTCASE "sincos - Standard (12) sincos_pp r0.x, r4.z"
	PASSBEGIN
	TESTBEGIN
	def c1, -0.45399049974, 0.0, 0.0, 0.0
	mov r4, c4
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.x, r4.z, c8, c9
	mov r0.yzw, c0.y
	VERIFY_RESULT
	V_X
	TESTEND

TESTCASE "sincos - Standard (13) sincos_pp r0.y, -t0.x"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.0, -0.309016994375, 0.0, 0.0
	dcl t0
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.y, -t0.x, c8, c9
	mov r0.xzw, c0.y
	VERIFY_RESULT
	V_Y
	TESTEND

TESTCASE "sincos - Standard (14) sincos_pp r0.xy, -t0.y"
	PASSBEGIN
	TESTBEGIN
	def c1, 0.309016994375, 0.951056516295, 0.0, 0.0
	dcl t0
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.xy, -t0.y, c8, c9
	mov r0.zw, c0.y
	VERIFY_RESULT
	V_X V_Y
	TESTEND

TESTCASE "sincos - Standard (15) sincos_pp r0.xy, -c4.w"
	PASSBEGIN
	TESTBEGIN
	def c1, -0.987688340595, 0.15643446504, 0.0, 0.0
	mov r0, c0.y
	mov r2, c0.w
	sincos_pp r0.xy, -c4.w, c8, c9
	mov r0.zw, c0.y
	VERIFY_RESULT
	V_X V_Y
	TESTEND



////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// masks & predication



// End of file.  16 tests generated.