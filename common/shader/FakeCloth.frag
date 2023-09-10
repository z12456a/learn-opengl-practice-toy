#version 400 core
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
in mat4 fsin_RotateMat;
in vec3 Tangent;
in vec3 BiTangent;
in mat3 TBN;
in float vsOut_KeyConUni;

uniform float Alpha;

uniform vec3 CamPos;
uniform sampler2D texture_main0;
uniform sampler2D texture_main1;
uniform sampler2D texture_normal0;
uniform sampler2D texture_ShadowMap;

uniform vec3 Light0_Direct;//ambient
uniform vec3 Light0_Color;

uniform vec3 Light1_Pos;//point
uniform vec3 Light1_Color;

uniform mat4 ViewMat;
uniform mat4 ProjectionMat;
uniform mat4 RotateMat;
uniform mat4 TranslateMat;

void main()
{
   /*Parameter*/
   float ambient =0.05;
   float gamma=1.0;
   float diffuse=0.25;
   float Bump_Scale_TB=1.0*vsOut_KeyConUni;
   float Bump_Scale_N=1.0;//normalize(Bump_Scale_TB);

   /*NormalMap_TBNNormal*/
     //vec3 normal=normalize(TBN*vec3(texture(texture_normal0,TexCoord).rgb*2-1) );
     //vec3 normal=normalize(TBN*(vec3(0,0,1)) );

   /*NormalMap_TBNNormal*/// fragment dF cal
   vec3 ftengent=vec3((inverse(TranslateMat)*dFdx(vec4(FragPos,1.0))).xyz) ;
   vec3 fbitengent=vec3((inverse(TranslateMat)*dFdy(vec4(FragPos,1.0))).xyz) ;
   mat3 TBN_dF=mat3(Bump_Scale_TB*normalize(ftengent),Bump_Scale_TB*normalize(fbitengent),Bump_Scale_N*NNormal)  ;
   vec3 normal=normalize(TBN_dF*vec3(texture(texture_normal0,TexCoord).rgb*2-1) );
   ///*test*/
   //vec3 normal=normalize(TBN_dF*(vec3(0,0,1)) );

   /*No NormalMap*/
   // vec3 normal=NNormal;


   /*diffuse=lambet*/
   vec3 ambientLight=ambient*Light0_Color;
   vec3 NLight0_Direct=normalize(Light0_Direct);

   float diffuseStrength=max(diffuse*dot(-NLight0_Direct,normal),0.0);
   vec4 lightResult=vec4(ambientLight+diffuseStrength*Light0_Color,0.0);
   vec4 texMix1 =mix(texture(texture_main0, TexCoord),texture(texture_main1, TexCoord),0.5) ;
   vec4 diffuseresult=texMix1+lightResult;


   // /*specular=blingPhonhe*/
   // vec3 camDir=normalize(CamPos-FragPos);
   // vec3 Light1_Dir=normalize(Light1_Pos-FragPos);
   // vec3 BlingNorm=normalize((camDir+Light1_Dir)/2);
   // float specularStrength=pow(max(dot(BlingNorm,normal),0.0),128);
   // vec4 specularResult=vec4(specularStrength*Light0_Color,0.0) ;

   /*specular=Phonhe*/
   vec3 camDir=normalize(CamPos-FragPos);
   vec3 Light1_Dir=normalize(Light1_Pos-FragPos);
   vec3 reflectDir = reflect(Light1_Dir, normal);
   float specularStrength=pow(max(dot(-reflectDir,camDir),0.0),63);
   vec4 specularResult=vec4(specularStrength*Light0_Color,0.0) ;




   /*Normal mipMap*/ //warning: bad code without TBN mat,
   // /*normal texture*/
   // vec4 TNNorm=fsin_RotateMat*vec4(texture(texture_normal0, TexCoord).rgb,1.0);
   // vec3 NNormal=normalize(vec3((TNNorm).xyz*2-1)); 
   //    /*diffuse*/
   // float ambient =0.15;
   // vec3 ambientLight=ambient*Light0_Color;

   // vec3 NLight0_Direct=normalize(Light0_Direct);
   // float diffuse=0.5;
   // float diffuseStrength=max(diffuse*dot(NLight0_Direct,NNormal),0.0);
   // vec4 lightResult=vec4(ambientLight+diffuseStrength*Light0_Color,diffuseStrength);

   // vec4 texMix1 =mix(texture(texture_main0, TexCoord),texture(texture_main1, TexCoord),0.5) ;
   // vec4 diffuseresult=texMix1+lightResult;


   // /*specular*/
   // vec3 camDir=normalize(CamPos-FragPos);
   // vec3 Light1_Dir=normalize(Light1_Pos-FragPos);
   // vec3 BlingNorm=normalize((camDir+Light1_Dir)/2);
   // float specularStrength=pow(max(dot(BlingNorm,NNormal),0.0),32);
   // vec4 specularResult=vec4(specularStrength*Light0_Color,1.0);
 


   vec4 result=diffuseresult+specularResult;

   if(result.a < 0.01)
      discard;
   //gl_FragColor=vec4(pow(result.rgb,vec3(1/gamma)),result.a*Alpha) ;
   gl_FragColor=vec4(pow(result.rgb,vec3(1/gamma)),Alpha) ;
   //gl_FragColor=vec4(TexCoord.x,TexCoord.y,0.0,1.0);
   //gl_FragColor=result;
   //gl_FragColor=vec4(pow(gl_FragCoord.z,32),pow(gl_FragCoord.z,32),pow(gl_FragCoord.z,32),Alpha) ;

//   gl_FragColor=vec4(result.rgb,0.5);
//   gl_FragColor =texture(texture_main1, TexCoord) ;
//   gl_FragColor =mix(texture(texture_main0, TexCoord),texture(texture_main1, TexCoord),0.2) ;
}


