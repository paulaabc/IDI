#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform mat4 TGMorty;

uniform vec3 POSFOCO1;
uniform vec3 POSFOCO2;
uniform vec3 POSFOCO3;
uniform vec3 POSFOCO4;
uniform vec3 COLORMAGENTA;
uniform vec3 COLORAMARILLO;
uniform vec3 POSLINTERNA;


out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus){
  // Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus){
  // Fong!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3(0); //se puede llamar a la difusa para calcular espc llama difusa
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main(){

    vec4 POSFOCOE1 = (View * vec4(POSFOCO1,1.0));
    vec4 POSFOCOE2 = (View * vec4(POSFOCO2,1.0));
    vec4 POSFOCOE3 = (View * vec4(POSFOCO3,1.0));
    vec4 POSFOCOE4 = (View * vec4(POSFOCO4,1.0));
    vec4 POSFOCOE5 = (View * TGMorty * vec4(POSLINTERNA,1.0));

    vec3 NM = normalize(fnormal);
    vec3 L1 = normalize(POSFOCOE1.xyz - fvertex);
    vec3 L2 = normalize(POSFOCOE2.xyz - fvertex);
    vec3 L3 = normalize(POSFOCOE3.xyz - fvertex);
    vec3 L4 = normalize(POSFOCOE4.xyz - fvertex);
    vec3 L5 = normalize(POSFOCOE5.xyz - fvertex);

    vec3 Color1 = Ambient() + Difus(NM, L1, COLORMAGENTA) + Especular(NM, L1, fvertex, COLORMAGENTA)
    + Difus(NM, L2, COLORMAGENTA) + Especular(NM, L2, fvertex, COLORMAGENTA)
    + Difus(NM, L3, COLORMAGENTA) + Especular(NM, L3, fvertex, COLORMAGENTA)
    + Difus(NM, L4, COLORMAGENTA) + Especular(NM, L4, fvertex, COLORMAGENTA)
    + Difus(NM, L5, COLORAMARILLO) + Especular(NM, L5, fvertex, COLORAMARILLO);

    FragColor = vec4(Color1, 1);

}
