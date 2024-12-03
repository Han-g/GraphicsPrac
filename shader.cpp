#include "shader.h"

// 쉐이더 로딩 함수
char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;

    if (fopen_s(&fptr, file, "rb") != 0) {
        return NULL;
    }

    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}

GLuint vertexShader, fragmentShader, shaderProgram;
GLchar* vertexSource, * fragmentSource;

// 쉐이더 생성 함수
//void make_vertexShaders()
//{
//    GLchar* vertexSource;
//    vertexSource = filetobuf("vertex.glsl");
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//
//    glShaderSource(vertexShader, 1, &vertexSource, NULL);
//    glCompileShader(vertexShader);
//
//    GLint result;
//    GLchar errorLog[512];
//
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//
//    if (!result)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
//        exit(-1);
//    }
//
//    free(vertexSource);
//}
//
//void make_fragmentShaders()
//{
//    GLchar* fragmentSource;
//    fragmentSource = filetobuf("fragment.glsl");
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//    glCompileShader(fragmentShader);
//    GLint result;
//    GLchar errorLog[512];
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//    if (!result)
//    {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//        std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
//        exit(-1);
//    }
//
//    free(fragmentSource);
//}
//
//GLuint make_shaderProgram()
//{
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    GLint result;
//    GLchar errorLog[512];
//
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
//    if (!result) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, errorLog);
//        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
//        exit(-1);
//    }
//
//    glUseProgram(shaderProgram);
//    return shaderProgram;
//}

void make_vertexShaders()
{
    vertexSource = filetobuf("pongVertex.glsl");
    //--- 버텍스 세이더 객체 만들기
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //--- 세이더 코드를 세이더 객체에 넣기
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
    //--- 버텍스 세이더 컴파일하기
    glCompileShader(vertexShader);
    //--- 컴파일이 제대로 되지 않은 경우: 에러 체크
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }
}
void make_fragmentShaders()
{
    fragmentSource = filetobuf("pongFragment.glsl");
    //--- 프래그먼트 세이더 객체 만들기
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //--- 세이더 코드를 세이더 객체에 넣기
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
    //--- 프래그먼트 세이더 컴파일
    glCompileShader(fragmentShader);
    //--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
        return;
    }
}
GLuint make_shaderProgram()
{
    GLuint ShaderProgramID;

    ShaderProgramID = glCreateProgram();
    glAttachShader(ShaderProgramID, vertexShader);
    glAttachShader(ShaderProgramID, fragmentShader);
    glLinkProgram(ShaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];

    glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
        exit(-1);
    }
    glUseProgram(ShaderProgramID);

    return ShaderProgramID;
}