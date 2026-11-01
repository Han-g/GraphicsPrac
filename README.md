# OpenGL Graphics Practice

도형·변환·카메라·텍스처·조명을 단계별로 실험하는 C++ / OpenGL 예제 모음.

| 항목 | 내용 |
|---|---|
| 기술 | C++, OpenGL, GLSL |
| 라이브러리 | GLEW, freeglut, GLM |
| 빌드 설정 | Visual Studio, v143, Windows SDK 10.0 |
| 실행 형태 | 번호별 예제 소스와 전처리 매크로 선택 |

> 문서 기준: 2026-11-01. 구현 상태는 소스 검토 기준이며, 이 문서 갱신 과정에서 빌드·플레이·성능 실험을 새로 실행하지 않았습니다.

## 1. 프로젝트 소개

그래픽스의 주요 개념을 작은 예제로 구현하고 화면 결과를 확인하는 학습 저장소입니다. 도형 출력과 입력 처리부터 3D 변환·카메라·텍스처·조명으로 이어집니다.

번호별 파일은 서로 다른 실습이며, 모든 기능이 하나의 실행 파일에서 동시에 선택되는 구조는 아닙니다.

## 2. 실행·시연 방법

1. [GLSL.vcxproj](https://github.com/Han-g/GraphicsPrac/blob/main/GLSL.vcxproj)을 Visual Studio에서 엽니다.
2. GLEW·freeglut·GLM의 include 경로와 라이브러리·DLL을 설정합니다. shader.h는 gl/glm 경로를 사용하므로 설치 구조와 맞는지 확인합니다.
3. 실행할 소스의 전처리 조건을 확인하고, main 함수가 하나만 활성화되도록 설정합니다.
4. 작업 디렉터리에 필요한 GLSL과 texture.bmp를 배치합니다.
5. 해당 예제의 키 입력 설명에 따라 결과를 확인합니다.

현재 main.cpp는 PROB를 8로 정의하지만 내부에는 PROB == 7 조건 코드가 있고, 29.cpp는 ON == 1 조건이 비활성 상태입니다. 따라서 체크아웃 직후 대표 예제가 바로 실행된다고 보장하지 않습니다.

정확한 외부 라이브러리 버전과 실행 구성은 아직 검증하지 않았습니다.

## 3. 현재 구현된 기능

| 주제 | 확인한 구성 |
|---|---|
| 도형 | 정점·버퍼·그리기와 입력 관련 예제 |
| 변환·카메라 | 모델·뷰·투영 행렬 사용 |
| 텍스처 | BMP 로딩 및 텍스처 관련 코드 |
| 조명 | Ambient·Diffuse·Specular 계산 |
| 셰이더 | 외부 GLSL 로딩·컴파일·링크 |

조명 파일 이름은 pongFragment.glsl이지만, 코드의 계산 방식은 Phong 조명 모델에 해당합니다.

## 4. 담당 범위와 사용 자료

GLEW, freeglut, GLM을 사용합니다.

개인 작성·수정 예제와 수업·참고 코드의 출처, 텍스처 등 리소스의 출처는 추가로 정리할 예정입니다. 각 실습의 기반 코드와 직접 확장한 수식·입력·표현을 구분합니다.

## 5. 핵심 구조와 설계

```text
예제별 main / 초기화
  → 정점·색상·텍스처 데이터
  → VAO / VBO
  → 모델·뷰·투영 및 조명 uniform
  → Vertex Shader
  → Fragment Shader
  → GLUT 화면·입력 콜백
```

| 파일 | 역할 |
|---|---|
| [shader.h](https://github.com/Han-g/GraphicsPrac/blob/main/shader.h) / [shader.cpp](https://github.com/Han-g/GraphicsPrac/blob/main/shader.cpp) | 공용 셰이더 로딩·컴파일 |
| [29.cpp](https://github.com/Han-g/GraphicsPrac/blob/main/29.cpp) | 텍스처·변환·조명 관련 예제 코드 |
| [pongVertex.glsl](https://github.com/Han-g/GraphicsPrac/blob/main/pongVertex.glsl) | 조명용 정점 단계 |
| [pongFragment.glsl](https://github.com/Han-g/GraphicsPrac/blob/main/pongFragment.glsl) | 주변광·확산광·반사광 |

현재 공용 셰이더 로더는 pongVertex.glsl / pongFragment.glsl을 참조합니다. 다른 GLSL 파일이 저장소에 있다고 해서 자동으로 선택되지는 않습니다.

## 6. 문제 해결과 검증 기록

| 비교할 항목 | 보여줄 결과 | 현재 상태 |
|---|---|---|
| 변환 순서 | 같은 객체에 서로 다른 변환 순서를 적용한 화면 | 예제별 캡처 필요 |
| 조명 | 주변광·확산광·반사광을 구분한 화면 | 코드 확인, 캡처 필요 |
| 텍스처 | 좌표와 샘플링 결과 | 실행 구성 확정 필요 |
| 실행 재현성 | 예제 하나를 선택하는 절차 | 매크로·의존 경로 정리 필요 |

예제의 이론 설명은 실제 사용한 수식과 연결하고, 캡처는 해당 예제를 실행해 얻은 화면을 사용합니다.

## 7. 현재 제한과 다음 작업

- [ ] 번호·주제·실행 파일·활성화 조건·조작법을 표로 정리
- [ ] 정상 실행 예제 하나와 라이브러리 버전을 먼저 확정
- [ ] 예제별 실제 결과 이미지 추가
- [ ] 이후 예제를 개별 빌드 대상으로 나누거나 하나의 선택 메뉴로 연결
- [ ] 수식과 화면 변화의 관계를 짧게 설명

---

저장소: [GraphicsPrac](https://github.com/Han-g/GraphicsPrac)
