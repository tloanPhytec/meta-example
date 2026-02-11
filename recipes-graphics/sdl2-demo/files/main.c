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
    "    v_color = a_color; \n"
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

    float rot_x = 0.0f, rot_y = 0.0f;
    int running = 1;

    Uint32 last_fps_print = SDL_GetTicks();
    int frame_count = 0;

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

        for (int i = 0; i < num_objects; i++) {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), my_model[i].vertices);
	    glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(my_model[i].vertices + 3));
            glDrawElements(GL_TRIANGLES, my_model[i].num_indices, GL_UNSIGNED_INT, my_model[i].indices);
        }

        frame_count++;
        Uint32 now = SDL_GetTicks();
        if (now - last_fps_print >= 1000) {
            printf("[FPS] %d | Objects: %d\n", frame_count, num_objects);
            fflush(stdout);
            frame_count = 0;
            last_fps_print = now;
        }

        SDL_GL_SwapWindow(window);
    }
    return 0;
}
