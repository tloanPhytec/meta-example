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
    "    float shade = (a_position.z + 5.0) / 10.0; \n"
    "    shade = clamp(shade, 0.7, 1.0); \n"
    "    gl_Position = a_position * u_matrix; \n"
    "    v_color = vec4(a_color.rgb * shade, 1.0); \n"
    "} \n";

const char *fragment_src =
    "precision mediump float; \n"
    "varying vec4 v_color; \n"
    "void main() { \n"
    "    gl_FragColor = v_color; \n"
    "} \n";

const char *hud_vs =
    "attribute vec4 a_pos; \n"
    "attribute vec2 a_uv; \n"
    "varying vec2 v_uv; \n"
    "void main() { \n"
    "    gl_Position = a_pos; \n"
    "    v_uv = a_uv; \n"
    "} \n";

const char *hud_fs =
    "precision mediump float; \n"
    "varying vec2 v_uv; \n"
    "uniform sampler2D u_tex; \n"
    "void main() { \n"
    "    vec4 color = texture2D(u_tex, v_uv); \n"
    "    if(color.r < 0.1) discard; \n" // Make background transparent
    "    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0); \n" // Black HUD text
    "} \n";

/* --- Simple 8x8 Font Data for 0-9 and 'FPS: ' --- */
const unsigned char font8x8[14][8] = {
    {0x3E,0x61,0x61,0x61,0x61,0x61,0x3E,0x00}, // 0
    {0x18,0x08,0x08,0x08,0x08,0x08,0x08,0x00}, // 1
    {0x3E,0x01,0x01,0x3E,0x60,0x60,0x3E,0x00}, // 2
    {0x3E,0x01,0x01,0x3E,0x01,0x01,0x3E,0x00}, // 3
    {0x61,0x61,0x61,0x3F,0x01,0x01,0x01,0x00}, // 4
    {0x3F,0x60,0x60,0x3E,0x01,0x01,0x3E,0x00}, // 5
    {0x3E,0x60,0x60,0x3E,0x61,0x61,0x3E,0x00}, // 6
    {0x3F,0x01,0x01,0x02,0x04,0x08,0x08,0x00}, // 7
    {0x3E,0x61,0x61,0x3E,0x61,0x61,0x3E,0x00}, // 8
    {0x3E,0x61,0x61,0x3F,0x01,0x01,0x3E,0x00}, // 9
    {0x7F,0x40,0x40,0x7E,0x40,0x40,0x40,0x00}, // F (Index 10)
    {0x7E,0x41,0x41,0x7E,0x40,0x40,0x40,0x00}, // P (Index 11)
    {0x3E,0x40,0x40,0x3E,0x01,0x01,0x3E,0x00}, // S (Index 12)
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}  // Space (Index 13)
};

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

/* --- Render FPS to Texture --- */
void update_fps_texture(GLuint tex, int fps) {
    char buf[16];
    sprintf(buf, "FPS %d", fps);
    unsigned char pixels[32 * 128] = {0}; // Tiny buffer
    for(int i=0; buf[i] != '\0'; i++) {
	int glyph;
        if      (buf[i] >= '0' && buf[i] <= '9') glyph = buf[i] - '0';
        else if (buf[i] == 'F') glyph = 10;
        else if (buf[i] == 'P') glyph = 11;
        else if (buf[i] == 'S') glyph = 12;
        else                    glyph = 13; // Space
        for(int y=0; y<8; y++) {
            for(int x=0; x<8; x++) {
                if(font8x8[glyph][y] & (1 << (7-x)))
                    pixels[(y+4)*128 + (i*8+x+4)] = 255;
            }
        }
    }
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 128, 32, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
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
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

    //printf("[DEBUG] SDL Video Driver: %s\n", SDL_GetCurrentVideoDriver());

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

    /* Setup HUD Shader */
    GLuint hvs = compile_shader(GL_VERTEX_SHADER, hud_vs);
    GLuint hfs = compile_shader(GL_FRAGMENT_SHADER, hud_fs);
    GLuint hud_prog = glCreateProgram();
    glAttachShader(hud_prog, hvs); glAttachShader(hud_prog, hfs);
    glLinkProgram(hud_prog);

    GLint u_matrix = glGetUniformLocation(prog, "u_matrix");

    /* Setup FPS Overlay Texture */
    GLuint fps_tex;
    glGenTextures(1, &fps_tex);
    glBindTexture(GL_TEXTURE_2D, fps_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    update_fps_texture(fps_tex, 0);

    /* 5. Move data to GPU once using VBOs */
    for (int i = 0; i < num_objects; i++) {
        glGenBuffers(1, &my_model[i].vbo);
        glBindBuffer(GL_ARRAY_BUFFER, my_model[i].vbo);
        glBufferData(GL_ARRAY_BUFFER, my_model[i].num_vertices * 6 * sizeof(GLfloat), my_model[i].vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &my_model[i].ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_model[i].ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, my_model[i].num_indices * sizeof(GLuint), my_model[i].indices, GL_STATIC_DRAW);
    }

    float rot_x = 0.0f, rot_y = 0.0f;
    int running = 1;

    float zoom_z = -7.0f;

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
	    else if(e.type == SDL_MULTIGESTURE) {
                // dDist is positive for pinching out, negative for pinching in
                zoom_z += e.mgesture.dDist * 20.0f;
                if (zoom_z > -2.0f) zoom_z = -2.0f; // Don't clip through the camera
                if (zoom_z < -20.0f) zoom_z = -20.0f; // Don't disappear
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

        model = multiply(model, translate(0, 0, zoom_z));

        /* 6. Combine (MVP = Projection * Model) */
        Mat4 mvp = multiply(model, proj);

        /* 7. Draw (BLUE Background) */
	glClearColor(0.820f, 0.812f, 0.824f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

        glUniformMatrix4fv(u_matrix, 1, GL_TRUE, (const GLfloat*)mvp.m);

        for (int i = 0; i < num_objects; i++) {
	    glBindBuffer(GL_ARRAY_BUFFER, my_model[i].vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_model[i].ibo);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glDrawElements(GL_TRIANGLES, my_model[i].num_indices, GL_UNSIGNED_INT, (void*)0);
        }

	/* --- DRAW FPS HUD --- */
        glUseProgram(hud_prog);
        glDisable(GL_DEPTH_TEST); // Draw on top
        glBindTexture(GL_TEXTURE_2D, fps_tex);

        // Simple 2D Quad in top-left (-1.0 to 1.0 space)
        float hud_verts[] = {
            -0.95f,  0.95f, 0.0f, 0.0f, // Pos X, Y, UV U, V
            -0.65f,  0.95f, 1.0f, 0.0f,
            -0.95f,  0.85f, 0.0f, 1.0f,
            -0.65f,  0.85f, 1.0f, 1.0f
        };
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Use client-side for tiny quad
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), hud_verts);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), &hud_verts[2]);
        glEnableVertexAttribArray(0); glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glUseProgram(prog); // Switch back to 3D shader

        frame_count++;
        Uint32 now = SDL_GetTicks();
        if (now - last_fps_print >= 1000) {
	    update_fps_texture(fps_tex, frame_count);
            //printf("[FPS] %d | Objects: %d\n", frame_count, num_objects);
            fflush(stdout);
            frame_count = 0;
            last_fps_print = now;
        }

        SDL_GL_SwapWindow(window);
    }
    return 0;
}
