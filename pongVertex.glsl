#version 330 core

layout(location = 0) in vec3 vPos;   // 위치
layout(location = 1) in vec3 vNormal; // 노멀 벡터

out vec3 FragPos;  // 프래그먼트에서 사용할 월드 좌표
out vec3 Normal;   // 프래그먼트에서 사용할 노멀 벡터

uniform mat4 model;      // 모델 변환 행렬
uniform mat4 view;       // 뷰 변환 행렬
uniform mat4 projection; // 투영 변환 행렬

void main()
{
    FragPos = vec3(model * vec4(vPos, 1.0));                // 월드 좌표 계산
    Normal = mat3(transpose(inverse(model))) * vNormal;     // 노멀 벡터 변환
    gl_Position = projection * view * vec4(FragPos, 1.0);  // 최종 변환 좌표
}