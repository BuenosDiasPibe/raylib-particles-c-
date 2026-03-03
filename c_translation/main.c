#include "raylib.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define SCREEN_WIDTH 1910
#define SCREEN_HEIGHT 1000

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
float ffLerp(float f1, float f2, float t) {
    return (1-t)*f1 + f2*t;
}

float Random(){ // between 0 and 1
    return (float)(GetRandomValue(0, 327867)) / 327867;
}

typedef struct {
    Rectangle rec;
    Vector2 velocity;
    Color color;
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

void add_particle(ParticleList *particles, Particle particle) {
    if(particles->count >= particles->capacity) {
        if(particles->capacity == 0) particles->capacity = 256;
        else particles->capacity *=2;
        particles->items = realloc(particles->items, particles->capacity*sizeof(*particles->items));
    }

    particles->items[particles->count++] = particle;
}
void remove_at(ParticleList *particles, size_t index){
    if(particles->count < index && index <= 0) return;
    // for(size_t i = index; i < particles->count; i++) { // a lot slower, but avoids ghost particles
    //     particles->items[i] = particles->items[i+1];
    // }
    // this algorithm for some reason gets particles which no longer exist. So it creates ghost particles
    particles->items[index] = particles->items[particles->count-1];
    particles->count--;
}

void update_particle(ParticleEmittor emmitor, ParticleList *particles, float delta) {
    float t = 0;
    size_t count = particles->count;
    for(int i = 0; i < count; ++i) {
        Particle *p = &particles->items[i];
        p->remainLifeTime -= delta;
        if(p->remainLifeTime <= 0) {
            remove_at(particles, i);
            count--;
            continue;
        }
        t           = 1-(float)(p->remainLifeTime/p->lifeTime);
        p->rec      = (Rectangle){
            .x      = p->rec.x + p->velocity.x*delta,
            .y      = p->rec.y + p->velocity.y*delta,
            .width  = ffLerp(emmitor.sizeStart.x, emmitor.sizeEnd.x, t),
            .height = ffLerp(emmitor.sizeStart.y, emmitor.sizeEnd.y, t)
        };
        p->color    = ColorLerp(emmitor.colorBegin, emmitor.colorEnd, t);
        vector2Sub(&p->velocity, Vector2FloatMul(emmitor.gravity, delta));
    }
}

void draw_particles_rec(ParticleList *particles, float delta) {
    for(size_t i = 0; i <= particles->count; i++) {
        Particle p = particles->items[i];
        DrawRectangleRec(
            p.rec,
            p.color
        );
    }
}
void draw_particles_img(ParticleList *particles, float delta, Texture2D *image) {
    for(size_t i = 0; i <= particles->count; i++) {
        Particle p = particles->items[i];
        DrawTexturePro(
            *image,
            (Rectangle){0,0,image->width, image->height},
            p.rec,
            (Vector2){0},
            0,
            p.color
        );
    }
}
void emmit_particle(ParticleList *particles, ParticleEmittor emmitor){
    float lifetime = emmitor.lifeTime + Random()*emmitor.lifeTimeVariation;
    Particle p = {
        .rec = (Rectangle){
            emmitor.position.x,
            emmitor.position.y,
            .width  = emmitor.sizeStart.x+(Random()*emmitor.sizeVariation.x) * (GetRandomValue(0, 1) ? -1 : 1),
            .height = emmitor.sizeStart.y+(Random()*emmitor.sizeVariation.y) * (GetRandomValue(0, 1) ? -1 : 1)
            },
        .remainLifeTime =  lifetime,
        .lifeTime = lifetime,
        .velocity = (Vector2){
            .x = emmitor.velocity.x + (Random()*emmitor.velocityVariation.x) * (GetRandomValue(0, 1) ? -1 : 1),
            .y = emmitor.velocity.y + (Random()*emmitor.velocityVariation.y) * (GetRandomValue(0, 1) ? -1 : 1)
            },
        .color = emmitor.colorBegin
    };
    add_particle(particles, p);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "particles");
    int emmit = 100;
    ParticleList particles = {0};
    ParticleEmittor emmitor = {
        .position = (Vector2){.x = (float)(SCREEN_WIDTH)/2, .y = (float)(SCREEN_HEIGHT)/2},
        .sizeStart = (Vector2){.x = 2, .y = 2},
        .sizeEnd = (Vector2){0},
        .sizeVariation = (Vector2){.x= 50, .y = 50},
        .colorBegin = ColorFromHSV(Random()*360, 1, 1),
        .colorEnd = {0},
        .velocity = (Vector2){0},
        .velocityVariation = (Vector2){.x = 30, .y = 30},
        .lifeTime = 5,
        .lifeTimeVariation = 10,
    };

    for(int i = 0; i < emmit; i++) {
        emmit_particle(&particles, emmitor);
    }
    char fps_chr[5] = {0};
    char particles_chr[100000] = {0};
    char emmit_shower[1000] = {0};

    //SetTargetFPS(60);
    float delta = 0;
    Texture2D img = LoadTexture("Acover.png");

    int minFPS = 327867;
    int maxParticles = 0;
    Color fpsColor = WHITE;

    while (!WindowShouldClose()) {
        delta = GetFrameTime();
        fpsColor = WHITE;
        for(int i = 0; i < emmit; i++) {
            emmit_particle(&particles, emmitor);
        }
        update_particle(emmitor, &particles, delta);
        sprintf(particles_chr, "pActive:%zu, pPool:%zu", particles.count, particles.capacity);
        sprintf(fps_chr, "%i", GetFPS());
        sprintf(emmit_shower, "eps:%i", emmit);
        if(minFPS > GetFPS()){
            minFPS = GetFPS();
            maxParticles = particles.count;
            fpsColor = RED;
        }
        if(minFPS == 0) minFPS = 327867;
        BeginDrawing();
            ClearBackground(BLACK);
            draw_particles_rec(&particles, delta);
            DrawText(fps_chr, 0, 0, 50, fpsColor);
            DrawText(particles_chr, 0, 50, 50, WHITE);
            DrawText(emmit_shower, 0, 100, 50, WHITE);
        EndDrawing();
    }
    free((void*)particles.items);
    UnloadTexture(img);
    printf("\nmaxParticles: %i\nminFPS: %i\nparticleEmmitorSize: %zu\n", maxParticles, minFPS, sizeof(ParticleEmittor));

    CloseWindow();
    return 0;
}
