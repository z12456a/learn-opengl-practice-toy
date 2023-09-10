#version 400 core                                        //offset
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
//uniform mat4 ScaleMat;


void main()
{
   mat4 modleMat=TranslateMat*RotateMat;//*ScaleMat;
   gl_Position =ProjectionMat*ViewMat*modleMat*vec4(aPos, 1.0);
   //gl_Position=vec4(aPos, 1.0);
}