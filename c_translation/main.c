#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

Vector2 vecto2Add(Vector2 v1, Vector2 v2) {
    return (Vector2) {
        .x = v1.x+v2.x,
        .y = v1.y+v2.y
    };
}

typedef struct {
    Color color;
    Vector2 position;
    Vector2 velocity;
    float lifeTime;
    float remainLifeTime;
} Particle;

typedef struct {
    Particle *items;
    size_t count;
    size_t capacity;
} ParticleList;

typedef struct {
    Vector2 position;
    Vector2 velocity, velocityVariation;
    Vector2 gravity;
    Color colorBegin, colorEnd;
    float lifeTime, lifeTimeVariation;
} ParticleEmittor;

void add_particle(ParticleList *particles, Particle *particle) {
    if(particles->count >= particles->capacity) {
        if(particles->capacity == 0) particles->capacity = 256;
        else particles->capacity *=2;
        particles->items = realloc(particles->items, particles->capacity*sizeof(*particles->items));
    }

    particles->items[particles->count++] = *particle;
}
void remove_at(ParticleList *particles, size_t index){
    // if some timing problems happend, maybe this is the problem
    // its really fucking fast tho
    if(particles->count < index) return;
    particles->items[index] = particles->items[particles->count];
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
        }
        t = 1-(float)(p->remainLifeTime/p->lifeTime);
        p->velocity = vecto2Add(p->velocity, vecto2Add(emmitor.gravity, (Vector2){.x=delta,.y=delta}));
        p->position = vecto2Add(p->position, vecto2Add(emmitor.velocity, (Vector2){.x=delta,.y=delta}));
        p->color = ColorLerp(emmitor.colorBegin, emmitor.colorEnd, t);
    }
}

void draw_particles(ParticleList *particles, float delta) {
    for(size_t i = 0; i < particles->count; ++i) {
        Particle p = particles->items[i];
        DrawRectangleRec(
            (Rectangle){.x=p.position.x, .y=p.position.y, .width = 10, .height = 10},
            p.color
        );
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "particles");
    int initial_capacity = 10;
    ParticleList particles = {0};
    ParticleEmittor emmitor = {
        .position = (Vector2){.x = (float)(SCREEN_WIDTH)/2, .y = (float)(SCREEN_HEIGHT)/2},
        .colorBegin = ColorFromHSV(GetRandomValue(0, 360), 1, 1),
        .colorEnd = ColorFromHSV(GetRandomValue(0, 360), 1, 1),
        .velocity = (Vector2){0,0},
        .velocityVariation = (Vector2){.x = 300, .y = 300},
        .lifeTime = 1,
        .lifeTimeVariation = 1,
    };
    for(int i = 0; i < initial_capacity; i++) {
        Vector2 randomVel = {
            .x = GetRandomValue(-emmitor.velocityVariation.x, emmitor.velocityVariation.x),
            .y = GetRandomValue(-emmitor.velocityVariation.y, emmitor.velocityVariation.y)
        };
        float lifetime = GetRandomValue(-emmitor.lifeTimeVariation/2, emmitor.lifeTimeVariation);
        Particle p = {
            .position = emmitor.position,
            .remainLifeTime = emmitor.lifeTime + lifetime,
            .lifeTime = emmitor.lifeTime +lifetime,
            .velocity = vecto2Add(emmitor.velocity, randomVel),
            .color = emmitor.colorBegin
        };
        add_particle(&particles, &p);
    }

    SetTargetFPS(60);
    float delta = 0;
    while (!WindowShouldClose()) {
        delta = GetFrameTime();
        update_particle(emmitor, &particles, delta);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw_particles(&particles, delta);
        EndDrawing();
    }
    free((void*)particles.items);

    CloseWindow();
    return 0;
}
