#include "raylib.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

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
    size_t index, ammount, active;
} ParticleEmittor;

void particleAlloc(ParticleList *particles){
    // if(particles->count >= particles->capacity) {
    //     if(particles->capacity == 0) particles->capacity = 256;
    //     else particles->capacity *= 2;
    // }
    particles->items = realloc(particles->items, particles->capacity*sizeof(Particle));
}

void emmitParticle(ParticleList *particles, ParticleEmittor *emmitor){
    if(emmitor->active >= emmitor->ammount+emmitor->index) return;

    float lifetime = emmitor->lifeTime + Random()*emmitor->lifeTimeVariation;
    particles->items[emmitor->active++] = (Particle){
        .rec            = (Rectangle) {
            .x          = emmitor->position.x,
            .y          = emmitor->position.y,
        },
        .sizeStart =(Vector2) {
            .x      = emmitor->sizeStart.x + Random()*emmitor->sizeVariation.x * (GetRandomValue(0, 1) ? -1 : 1),
            .y     = emmitor->sizeStart.y + Random()*emmitor->sizeVariation.y * (GetRandomValue(0, 1) ? -1 : 1)
        },
        .remainLifeTime = lifetime,
        .lifeTime       = lifetime,
        .velocity       = (Vector2) {
            .x          = emmitor->velocity.x + (Random()*emmitor->velocityVariation.x) * (GetRandomValue(0, 1) ? -1 : 1),
            .y          = emmitor->velocity.y + (Random()*emmitor->velocityVariation.y) * (GetRandomValue(0, 1) ? -1 : 1)
        },
        .color          = emmitor->colorBegin,
        .colorBegin     = emmitor->colorBegin,
        .colorEnd       = emmitor->colorEnd
    };
}

void removeDeathParticles(ParticleList *particles, ParticleEmittor *emmitor){
    size_t point0 = emmitor->index;
    for(size_t point1 = emmitor->index; point1 < emmitor->active; ++point1){
        particles->items[point0] = particles->items[point1];
        if(particles->items[point1].remainLifeTime > 0){
            point0++;
        }
    }
    emmitor->active = point0; // thank you Cyberpunk2007
}

void updateParticle(ParticleEmittor *emmitor, ParticleList *particles, float delta) {
    float t = 0;
    for(int i = emmitor->index; i < emmitor->active; ++i) {
        Particle *p = &particles->items[i];
        vector2Sub(&p->velocity, Vector2FloatMul(emmitor->gravity, delta));
        p->remainLifeTime -= delta;
        t                  = 1-(float)(p->remainLifeTime/p->lifeTime);
        p->rec             = (Rectangle){
            .x             = p->rec.x + p->velocity.x*delta,
            .y             = p->rec.y + p->velocity.y*delta,
            .width         = ffLerp(p->sizeStart.x, emmitor->sizeEnd.x, t),
            .height        = ffLerp(p->sizeStart.y, emmitor->sizeEnd.y, t)
        };
        p->color           = colorLerp(p->colorBegin, p->colorEnd, t);
    }
    removeDeathParticles(particles, emmitor);
}

void drawParticlesRec(ParticleList *particles,ParticleEmittor *emmitor, float delta) {
    for(size_t i = emmitor->index; i < emmitor->active; ++i) {
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
    ParticleList particles = {.capacity = 30000};
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
        .index = 0,
        .ammount = 30000,
        .active = 0
    };
    emmitParticle(&particles, &emmitor);
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
                emmitParticle(&particles, &emmitor);
            }
        }
        updateParticle(&emmitor, &particles, delta);

        sprintf(particles_chr, "pActive:%zu, pPool:%zu", particles.count, particles.capacity);
        sprintf(fps_chr, "%i", GetFPS());
        sprintf(emmit_shower, "eps:%i", emmit);
        BeginDrawing();
            ClearBackground(BLACK);
            drawParticlesRec(&particles,&emmitor, delta);
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
