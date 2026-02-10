#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "model_data.h"

/* --- Configuration --- */
#define WIN_WIDTH  1920
#define WIN_HEIGHT 1200

/* --- Shaders --- */
const char *vertex_src =
    "attribute vec4 a_position; \n"
    "attribute vec4 a_color; \n"
    "uniform mat4 u_matrix; \n"
    "varying vec4 v_color; \n"
    "void main() { \n"
    "    gl_Position = a_position * u_matrix; \n"
    "    float shade = (a_position.z + 2.0) / 4.0; \n"
    "    v_color = vec4(shade, shade, shade, 1.0); \n"
    "} \n";

const char *fragment_src =
    "precision mediump float; \n"
    "varying vec4 v_color; \n"
    "void main() { \n"
    "    gl_FragColor = v_color; \n"
    "} \n";

/* --- Math Helpers --- */
typedef struct { float m[4][4]; } Mat4;

Mat4 identity() {
    Mat4 res = {0};
    for(int i=0; i<4; i++) res.m[i][i] = 1.0f;
    return res;
}

Mat4 multiply(Mat4 A, Mat4 B) {
    Mat4 res = {0};
    for(int r=0; r<4; r++)
        for(int c=0; c<4; c++)
            for(int i=0; i<4; i++) res.m[r][c] += A.m[r][i] * B.m[i][c];
    return res;
}

Mat4 rotate_y(float angle) {
    Mat4 res = identity();
    float c = cosf(angle), s = sinf(angle);
    res.m[0][0] = c; res.m[0][2] = s;
    res.m[2][0] = -s; res.m[2][2] = c;
    return res;
}

Mat4 rotate_x(float angle) {
    Mat4 res = identity();
    float c = cosf(angle), s = sinf(angle);
    res.m[1][1] = c; res.m[1][2] = -s;
    res.m[2][1] = s; res.m[2][2] = c;
    return res;
}

/* Perspective Projection */
Mat4 perspective(float fov, float aspect, float near, float far) {
    Mat4 res = {0};
    float f = 1.0f / tanf(fov / 2.0f);
    res.m[0][0] = f / aspect;
    res.m[1][1] = f;
    res.m[2][2] = (far + near) / (near - far);
    res.m[2][3] = -1.0f;
    res.m[3][2] = (2.0f * far * near) / (near - far);
    return res;
}

Mat4 translate(float x, float y, float z) {
    Mat4 res = identity();
    res.m[3][0] = x; res.m[3][1] = y; res.m[3][2] = z;
    return res;
}

/* --- Debug Helpers --- */
void check_gl_error(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        printf("[GL ERROR] After %s() -> Code 0x%x\n", op, error);
    }
}

GLuint compile_shader(GLenum type, const char *src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printf("[ERROR] Shader Compile Failed for type 0x%x!\n", type);
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            printf("Error Log:\n%s\n", infoLog);
            free(infoLog);
        }
    }
    return shader;
}

int main(int argc, char **argv) {
    /* 1. Initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    
    /* 2. Configure GLES 2.0 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* 3. Create Window */
    SDL_Window *window = SDL_CreateWindow("AM62P 3D", 0, 0, WIN_WIDTH, WIN_HEIGHT,
                                          SDL_WINDOW_OPENGL);

    printf("[DEBUG] SDL Video Driver: %s\n", SDL_GetCurrentVideoDriver());

    SDL_GL_CreateContext(window);

    /* 4. Compile Shaders */
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_src);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_src);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs); glAttachShader(prog, fs);
    glBindAttribLocation(prog, 0, "a_position");
    glBindAttribLocation(prog, 1, "a_color");
    glLinkProgram(prog);
    glUseProgram(prog);

    GLint u_matrix = glGetUniformLocation(prog, "u_matrix");

    /* 5. Load Data from model_data.h */

    /* ADDED: Bounds Check to see if model is off-screen */
    float min_y = 1000.0f, max_y = -1000.0f;
    /* Vertices are Stride 6 (X, Y, Z, R, G, B). Check Y (index 1) */
    for(int i=1; i < num_vertices; i+=6) {
        if(model_vertices[i] < min_y) min_y = model_vertices[i];
        if(model_vertices[i] > max_y) max_y = model_vertices[i];
    }
    printf("[DEBUG] Model Y-Range: %.2f to %.2f\n", min_y, max_y);
    printf("[DEBUG] Num Indices: %d (Short Limit: 65535)\n", num_indices);

    GLuint vbo, ibo;
    glGenBuffers(1, &vbo); glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model_vertices), model_vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &ibo); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model_indices), model_indices, GL_STATIC_DRAW);

    float rot_x = 0.0f, rot_y = 0.0f;
    int running = 1;

    /* 6. Render Loop */
    while(running) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = 0;
            else if(e.type == SDL_FINGERMOTION) {
                rot_y -= e.tfinger.dx * 5.0f;
                rot_x -= e.tfinger.dy * 5.0f;
		//printf("[DEBUG] Finger Press!");
            }
            else if(e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON_LMASK)) {
                rot_y += e.motion.yrel * 0.01f;
                rot_x += e.motion.xrel * 0.01f;
		//printf("[DEBUG] Mouse Press!");
            }
        }

        /* 1. Get Actual Window Size (MOVED TO TOP) */
        int w, h;
        SDL_GetWindowSize(window, &w, &h);

	//printf("[DEBUG] Frame Height: %d\n", h);
	//printf("[DEBUG] Frame Width: %d\n", w);

	if (h == 0) {
	    printf("[DEBUG] Frame Height ERROR: Height = 0 pixels!\n");
	    break;
	}

        glViewport(0, 0, w, h);

        /* 2. Define Projection Matrix (MOVED TO TOP) */
        /* Now 'proj' exists before we use it */
        Mat4 proj = perspective(1.0f, (float)w/h, 0.1f, 100.0f);

        /* 3. Create Model Matrix */
        Mat4 model = multiply(rotate_x(rot_x), rotate_y(rot_y));

        /* 4. SCALE: Shrink by 10x */
        Mat4 scale_mat = identity(); 
        scale_mat.m[0][0] = 0.1f; 
        scale_mat.m[1][1] = 0.1f; 
        scale_mat.m[2][2] = 0.1f;
	model = multiply(scale_mat, model);

        model = multiply(model, translate(0, 0, -7.0f));

        /* 6. Combine (MVP = Projection * Model) */
        Mat4 mvp = multiply(model, proj);

        /* 7. Draw (BLUE Background) */
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

        glUniformMatrix4fv(u_matrix, 1, GL_TRUE, (const GLfloat*)mvp.m);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));

        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);

        static int frame_count = 0;
        if(frame_count++ % 60 == 0) {
             check_gl_error("glDrawElements");
             printf("[DEBUG] Frame %d rendered\n", frame_count);
             fflush(stdout); 
        }

        SDL_GL_SwapWindow(window);
    }
    return 0;
}
