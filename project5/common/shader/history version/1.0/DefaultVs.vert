#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 ViewMat;
uniform mat4 ProjectionMat;
uniform mat4 RotateMat;
uniform mat4 TranslateMat;

out vec2 TexCoord;
out vec3 NNormal;
out vec3 FragPos;
void main()
{
   mat4 modleMat=TranslateMat*RotateMat;
   gl_Position =ProjectionMat*ViewMat*modleMat*vec4(aPos, 1.0);
   NNormal=normalize((RotateMat*vec4(aNormal,1.0)).xyz);
	TexCoord=vec2(aTexCoord.x,aTexCoord.y);
   FragPos=vec3((modleMat*vec4(aPos,1.0)).xyz);
}