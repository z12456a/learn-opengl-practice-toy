#version 450 core                                        //offset
layout (location = 0) in vec3 aPos;                      //3
layout (location = 1) in vec2 aTexCoord;                 //2
layout (location = 2) in vec3 aNormal;                   //3
layout (location = 3) in vec3 aTangent;                  //3
layout (location = 4) in vec3 aBiTangent;                //3
                                                         //14

uniform mat4 ViewMat;
uniform mat4 ProjectionMat;
uniform mat4 RotateMat;
uniform mat4 TranslateMat;
uniform mat4 ScaleMat;

out vec2 TexCoord;
out vec3 NNormal;
out vec3 FragPos;
out mat4 fsin_RotateMat;
out vec3 Tangent;
out vec3 BiTangent;
out mat3 TBN;
void main()
{
   mat4 modleMat=TranslateMat*RotateMat*ScaleMat;
   NNormal=normalize((RotateMat*vec4(aNormal,1.0)).xyz);
   Tangent=normalize((RotateMat*vec4(aTangent,1.0)).xyz);
   BiTangent=normalize((RotateMat*vec4(aBiTangent,1.0)).xyz);
   TBN= mat3(BiTangent,Tangent,NNormal);

	TexCoord=vec2(aTexCoord.x,aTexCoord.y);
   FragPos=vec3((modleMat*vec4(aPos,1.0)).xyz);
   fsin_RotateMat=RotateMat;


   gl_Position =ProjectionMat*ViewMat*modleMat*vec4(aPos, 1.0);
}