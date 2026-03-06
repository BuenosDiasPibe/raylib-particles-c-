#include "raylib.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define MAX_EMMITOR_PARTICLES 327867

Vector2 vector2Add(Vector2 v1, Vector2 v2) {
    return (Vector2) {
        .x = v1.x+v2.x,
        .y = v1.y+v2.y
    };
}
Vector2 vector2Mul(Vector2 v1, Vector2 v2) {
    return (Vector2) {
        .x = v1.x*v1.x,
        .y = v2.y*v2.y
    };
}
Vector2 Vector2FloatMul(Vector2 v, float f) {
    return (Vector2) {
        .x = v.x*f,
        .y = v.y*f
    };
}
void vector2Sub(Vector2 *v1, Vector2 v2) {
    v1->x -= v2.x;
    v1->y -= v2.y;
}
void RecVec2AddPosition(Rectangle *r1, Vector2 v2) {
    r1->x += v2.x;
    r1->y += v2.y;
}
void RecVec2AddSize(Rectangle *r1, Vector2 v2) {
    r1->width += v2.x;
    r1->height += v2.y;
}
void RecRecAdd(Rectangle *r1, Rectangle r2){
    r1->width  +=r2.width;
    r1->height +=r2.height;
}
Rectangle RecVecSizeLerp(Rectangle r1, Vector2 v2, float t) {
    return (Rectangle) {
        .x = r1.x,
        .y = r1.y,
        .width  = r1.width*(1-t) + v2.x*t,
        .height = r1.height*(1-t) + v2.y*t
    };
}
Color colorLerp(Color color1, Color color2, float t) {
    if(t > 1.f) t = 1;
    else if(t < 0.f) t = 0;
    return (Color){
        (unsigned char)((1.f-t)*color1.r + color2.r*t),
        (unsigned char)((1.f-t)*color1.g + color2.g*t),
        (unsigned char)((1.f-t)*color1.b + color2.b*t),
        (unsigned char)((1.f-t)*color1.a + color2.a*t),
    };
}
float ffLerp(float f1, float f2, float t) {
    return (1-t)*f1 + f2*t;
}

float Random(){ // between 0 and 1
    return (float)(GetRandomValue(0, 327867)) / 327867;
}

typedef struct {
    Rectangle rec;
    Vector2 velocity, sizeStart;
    Color color, colorBegin, colorEnd;
    float lifeTime;
    float remainLifeTime;
} Particle;

typedef struct {
    Particle* items;
    size_t count;
    size_t capacity;
} ParticleList;

typedef struct {
    Vector2 sizeStart, sizeEnd, sizeVariation;
    Vector2 position;
    Vector2 velocity, velocityVariation;
    Vector2 gravity;
    Color colorBegin, colorEnd;
    float lifeTime, lifeTimeVariation;
} ParticleEmittor;

void particleAlloc(ParticleList *particles){
    if(particles->count >= particles->capacity) {
        if(particles->capacity == 0) particles->capacity = 256;
        else particles->capacity *= 2;
        particles->items = realloc(particles->items, particles->capacity*sizeof(Particle));
    }
}

void emmitParticle(ParticleList *particles, ParticleEmittor emmitor){
    particleAlloc(particles);
    float lifetime = emmitor.lifeTime + Random()*emmitor.lifeTimeVariation;
    Particle *p = &particles->items[particles->count++];
    *p = (Particle){
        .rec            = (Rectangle) {
            .x          = emmitor.position.x,
            .y          = emmitor.position.y,
        },
        .sizeStart =(Vector2) {
            .x      = emmitor.sizeStart.x + Random()*emmitor.sizeVariation.x * (GetRandomValue(0, 1) ? -1 : 1),
            .y     = emmitor.sizeStart.y + Random()*emmitor.sizeVariation.y * (GetRandomValue(0, 1) ? -1 : 1)
        },
        .remainLifeTime = lifetime,
        .lifeTime       = lifetime,
        .velocity       = (Vector2) {
            .x          = emmitor.velocity.x + (Random()*emmitor.velocityVariation.x) * (GetRandomValue(0, 1) ? -1 : 1),
            .y          = emmitor.velocity.y + (Random()*emmitor.velocityVariation.y) * (GetRandomValue(0, 1) ? -1 : 1)
        },
        .color          = emmitor.colorBegin,
        .colorBegin     = emmitor.colorBegin,
        .colorEnd       = emmitor.colorEnd
    };
}

void removeDeathParticles(ParticleList *particles){
    if(particles->count == 0) return;
    size_t point0 = 0;
    for(size_t point1 = 0; point1 < particles->count; ++point1){
        particles->items[point0] = particles->items[point1];
        if(!(particles->items[point1].remainLifeTime <= 0)){
            point0++;
        }
    }
    particles->count = point0; // thank you Cyberpunk2007
}

void updateParticle(ParticleEmittor emmitor, ParticleList *particles, float delta) {
    float t = 0;
    size_t count = particles->count;
    for(int i = 0; i < count; ++i) {
        Particle *p = &particles->items[i];
        vector2Sub(&p->velocity, Vector2FloatMul(emmitor.gravity, delta));
        p->remainLifeTime -= delta;
        t                  = 1-(float)(p->remainLifeTime/p->lifeTime);
        p->rec             = (Rectangle){
            .x             = p->rec.x + p->velocity.x*delta,
            .y             = p->rec.y + p->velocity.y*delta,
            .width         = ffLerp(p->sizeStart.x, emmitor.sizeEnd.x, t),
            .height        = ffLerp(p->sizeStart.y, emmitor.sizeEnd.y, t)
        };
        p->color           = colorLerp(p->colorBegin, p->colorEnd, t);
    }
    removeDeathParticles(particles);
}

void drawParticlesRec(ParticleList *particles, float delta) {
    for(size_t i = 0; i < particles->count; ++i) {
        Particle p = particles->items[i];
        DrawRectangleRec(p.rec, p.color);
    }
}
void drawParticlesImg(ParticleList *particles, float delta, Texture2D *image) {
    for(size_t i = 0; i < particles->count; ++i) {
        Particle *p = &particles->items[i];
        DrawTexturePro(
            *image,
            (Rectangle){0,0,image->width, image->height},
            p->rec,
            (Vector2){0},
            0,
            p->color
        );
    }
}

void checkFileDropped(Texture2D *img){
    if(IsFileDropped())
    {
        FilePathList path = LoadDroppedFiles();
        if(IsFileExtension(path.paths[0],".png")
        || IsFileExtension(path.paths[0],".jpg")
        || IsFileExtension(path.paths[0],".jpeg"))
        {
            *img = LoadTexture(path.paths[0]);
        }
        UnloadDroppedFiles(path);
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "particles");
    int emmit = 10;
    ParticleList particles = {0};
    particleAlloc(&particles);
    Color c =  ColorFromHSV(Random()*360, 1, 1);
    c.a = 0;
    ParticleEmittor emmitor = {
        .position = (Vector2){.x = (float)(SCREEN_WIDTH)/2, .y = (float)(SCREEN_HEIGHT)/2},
        .sizeStart = (Vector2){0},
        .sizeVariation = (Vector2){.x = 50, .y = 50},
        .sizeEnd = (Vector2){0},
        .colorBegin = {0},
        .colorEnd = c,
        //.velocity = (Vector2){0,-30},
        .velocityVariation = (Vector2){.x = 10, .y = 10},
        .lifeTime = 0.5f,
        .lifeTimeVariation = 15,
        //.gravity = (Vector2){0,-40}
    };
    char fps_chr[5] = {0};
    char particles_chr[100] = {0};
    char emmit_shower[50] = {0};

    SetTargetFPS(60);
    float delta = 0;
    Texture2D img = LoadTexture("Acover.png");
    float hue = 0;

    while (!WindowShouldClose()) {
        delta = GetFrameTime()*2;
        hue = (float)(fmod(hue+delta*10, 360.f));
        emmitor.colorBegin = ColorFromHSV(hue, 1, 1);
        emmitor.position = GetMousePosition();
        emmitor.velocity = GetMouseDelta();
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            for(int i = 0; i < emmit; ++i) {

                emmitParticle(&particles, emmitor);
            }
        }
        updateParticle(emmitor, &particles, delta);

        sprintf(particles_chr, "pActive:%zu, pPool:%zu", particles.count, particles.capacity);
        sprintf(fps_chr, "%i", GetFPS());
        sprintf(emmit_shower, "eps:%i", emmit);
        BeginDrawing();
            ClearBackground(BLACK);
            drawParticlesRec(&particles, delta);
            DrawText(fps_chr, 0, 0, 50, WHITE);
            DrawText(particles_chr, 0, 50, 50, WHITE);
            DrawText(emmit_shower, 0, 100, 50, WHITE);
        EndDrawing();
    }
    free((void*)particles.items);
    UnloadTexture(img);

    CloseWindow();
    return 0;
}
