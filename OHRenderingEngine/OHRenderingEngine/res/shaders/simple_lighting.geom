#version 400 core
layout (triangles) in;
layout (triangle_strip, max_vertices  = 3) out;

in VS_OUT
{
	vec3 v_NormalDir;
	vec2 v_TexCoords;
	vec3 fragPos;
	vec3 fragPosWorld;
	mat3 TBN;
} gs_in[];

out GS_OUT
{
	vec3 v_NormalDir;
	vec2 v_TexCoords;
	vec3 fragPos;
	vec3 fragPosWorld;
	mat3 TBN;
} gs_out;

uniform float explodePower;
uniform mat4 projection;

vec3 GetNormal()
{
	vec3 va = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
	vec3 vb = vec3(gl_in[0].gl_Position - gl_in[2].gl_Position);
	return normalize(vec3(cross(va,vb)));
}

void main()
{
	for(int index = 0; index < 3; index++)
	{
		gs_out.v_NormalDir = gs_in[index].v_NormalDir;
		gs_out.v_TexCoords = gs_in[index].v_TexCoords;
		gs_out.fragPos = gs_in[index].fragPos;
		gs_out.fragPosWorld = gs_in[index].fragPosWorld;
		gs_out.TBN = gs_in[index].TBN;
		gl_Position = projection * ( gl_in[index].gl_Position + vec4(GetNormal()*explodePower,0.0f) );
		EmitVertex();
	}
	EndPrimitive();
}