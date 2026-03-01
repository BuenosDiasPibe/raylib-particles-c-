#include "raylib.h"
#include <stddef.h>
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
    size_t count;
    size_t capacity;
    Particle *particles;
} ParticleList;

typedef struct {
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
        particles->particles = realloc(particles->particles, particles->capacity);
    }

    particles->particles[particles->count++] = particle;
}
void remove_at(ParticleList *particles, size_t index){
    if(particles->count < index) return;
    for(size_t i = index; i < particles->count; i++) {
        particles->particles[i] = particles->particles[i+1];
    }
    particles->count--;
}

void update_particle(ParticleEmittor emmitor, ParticleList *particles, float delta) {
    float t = 0;
    size_t count = particles->count;
    for(int i = 0; i < count; ++i) {
        Particle *p = &particles->particles[i];
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
        Particle p = particles->particles[i];
        DrawRectangleRec(
            (Rectangle){.x=p.position.x, .y=p.position.y, .width = 10, .height = 10},
            p.color
        );
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "particles");
    int initial_capacity = 20;
    ParticleList particles = {0};
    ParticleEmittor emmitor = {
        .position = (Vector2){.x = (float)(SCREEN_WIDTH)/2, .y = (float)(SCREEN_HEIGHT)/2},
        .colorBegin = ColorFromHSV(GetRandomValue(0, 360), 1, 1),
        .colorEnd = ColorFromHSV(GetRandomValue(0, 360), 1, 1),
        .velocity = (Vector2){0,0},
        .velocityVariation = (Vector2){.x = 300, .y = 300},
        .lifeTime = 10,
        .lifeTimeVariation = 1,
    };
    Particle p = {0};
    p = (Particle){
        .remainLifeTime = emmitor.lifeTime + emmitor.lifeTimeVariation,
        .lifeTime = emmitor.lifeTime + emmitor.lifeTimeVariation,
        .velocity = vecto2Add(emmitor.velocity, emmitor.velocityVariation),
        .color = emmitor.colorBegin
    };
    add_particle(&particles, p);

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

    CloseWindow();
    return 0;
}
