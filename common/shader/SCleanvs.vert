#version 450 core                                        //offset
layout (location = 0) in vec3 aPos;                      //3
layout (location = 1) in vec2 aTexCoord;                 //2
layout (location = 2) in vec3 aNormal;                   //3
layout (location = 3) in vec3 aTangent;                  //3
layout (location = 4) in vec3 aBiTangent;                //3
                                                         //14

out vec2 TexCoord; 

void main()
{
   gl_Position =vec4(aPos, 1.0);
   TexCoord=aTexCoord;
}