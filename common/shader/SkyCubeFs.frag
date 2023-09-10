#version 400 core                                        //offset

uniform sampler2D targetTex;

in vec2 TexCoord; 

void main()
{
  vec4 targetTexColor= texture(targetTex,TexCoord);
  float testP=100*pow(targetTexColor.r,1);
  //  float testP=targetTexColor.r-0.5;

  gl_FragColor =vec4(testP,testP,testP,1.0) ;
  //gl_FragColor =vec4(targetTexColor.rgb,1.0) ;
 // gl_FragColor =vec4(1.0,0.0,0.0,1.0) ;
}


