#version 400 core                                        //offset

void main()
{
  //gl_FragColor =vec4(1.0,0.0,0.0,1.0) ;
  //gl_FragDepth=0.5;
  //gl_FragColor=vec4(pow(gl_FragCoord.z,32),pow(gl_FragCoord.z,32),pow(gl_FragCoord.z,32),pow(gl_FragCoord.z,32)) ;
   gl_FragColor=vec4(100*gl_FragCoord.z,100*gl_FragCoord.z,100*gl_FragCoord.z,1.0);
}


