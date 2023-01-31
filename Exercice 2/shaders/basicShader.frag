#version 330 core

in vec3 fcolor;

uniform int malo;

out vec4 FragColor;

void main() {
    if(malo == 1){
        FragColor = vec4(fcolor,1)*vec4(1,0,0,1);
    }
    else FragColor = vec4(fcolor, 1);
}

