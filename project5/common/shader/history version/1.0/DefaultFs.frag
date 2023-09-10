#version 330 core
/* Color palette */
#define BLACK           vec3(0.0, 0.0, 0.0)
#define WHITE           vec3(1.0, 1.0, 1.0)
#define RED             vec3(1.0, 0.0, 0.0)
#define GREEN           vec3(0.0, 1.0, 0.0)
#define BLUE            vec3(0.0, 0.0, 1.0)
#define YELLOW          vec3(1.0, 1.0, 0.0)
#define CYAN            vec3(0.0, 1.0, 1.0)
#define MAGENTA         vec3(1.0, 0.0, 1.0)
#define ORANGE          vec3(1.0, 0.5, 0.0)
#define PURPLE          vec3(1.0, 0.0, 0.5)
#define LIME            vec3(0.5, 1.0, 0.0)
#define ACQUA           vec3(0.0, 1.0, 0.5)
#define VIOLET          vec3(0.5, 0.0, 1.0)
#define AZUR            vec3(0.0, 0.5, 1.0)

out vec4 FragColor;
in vec3 NNormal;
in vec2 TexCoord;
in vec3 FragPos;
uniform vec3 CamPos;
uniform sampler2D texture_main0;
uniform sampler2D texture_main1;
/*
uniform sampler2D texture_normal1;
*/
uniform vec3 Light0_Direct;//ambient
uniform vec3 Light0_Color;

uniform vec3 Light1_Pos;//point
uniform vec3 Light1_Color;

void main()
{
   /*diffuse*/
   float ambient =0.15;
   vec3 ambientLight=ambient*Light0_Color;

   vec3 NLight0_Direct=normalize(Light0_Direct);
   float diffuse=0.5;
   float diffuseStrength=max(diffuse*dot(NLight0_Direct,NNormal),0.0);
   vec4 lightResult=vec4(ambientLight+diffuseStrength*Light0_Color,diffuseStrength);

   vec4 texMix1 =mix(texture(texture_main0, TexCoord),texture(texture_main1, TexCoord),0.5) ;
   vec4 diffuseresult=mix(texMix1,lightResult,0.5);


   /*specular*/
   vec3 eyeRay=normalize(CamPos-FragPos);
   vec3 sunRay=normalize(Light1_Pos-FragPos);
   vec3 BulingNorm=normalize((eyeRay+sunRay)/2);
   float specularStrength=pow(max(dot(BulingNorm,NNormal),0.0),8);
   vec4 specularResult=vec4(specularStrength*Light0_Color,1.0);


   /*
   /*normal texture*/
   */

   vec4 result=diffuseresult+specularResult;
   gl_FragColor=result;
//   FragColor =texture(texture_main1, TexCoord) ;
//  FragColor =mix(texture(texture_main0, TexCoord),texture(texture_main1, TexCoord),0.2) ;
}


