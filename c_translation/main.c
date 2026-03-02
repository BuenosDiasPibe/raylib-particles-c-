#include "raylib.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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
Vector2 Vector2Neg(Vector2 v) {
    return (Vector2) {.x = -v.x, .y = -v.y};
}
Vector2 vector2Sub(Vector2 v1, Vector2 v2) {
    return (Vector2) {
        .x = v1.x-v2.x,
        .y = v1.y-v2.y
    };
}
float Random(){ // between 0 and 1
    return (float)(GetRandomValue(0, 327867)) / 327867;
}

typedef struct {
    Color color;
    Vector2 position;
    Vector2 velocity;
    float lifeTime;
    float remainLifeTime;
} Particle;

typedef struct {
    Particle* items;
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

void add_particle(ParticleList *particles, Particle particle) {
    if(particles->count >= particles->capacity) {
        if(particles->capacity == 0) particles->capacity = 256;
        else particles->capacity *=2;
        particles->items = realloc(particles->items, particles->capacity*sizeof(*particles->items));
    }

    particles->items[particles->count] = particle;
    particles->count++;
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
        p->velocity = vector2Sub(p->velocity, Vector2FloatMul(emmitor.gravity, delta));
        p->position = vector2Add(p->position, Vector2FloatMul(p->velocity, delta));
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
    int emmit = 1000;
    ParticleList particles = {0};
    ParticleEmittor emmitor = {
        .position = (Vector2){.x = (float)(SCREEN_WIDTH)/2, .y = (float)(SCREEN_HEIGHT)/2},
        .colorBegin = ColorFromHSV(Random()*360, 1, 1),
        .colorEnd = ColorFromHSV(Random()*360, 1, 1),
        .velocity = (Vector2){0},
        .velocityVariation = (Vector2){.x = 30, .y = 30},
        .lifeTime = 1,
        .lifeTimeVariation = 10,
    };
    for(int i = 0; i < emmit; i++) {
        Vector2 randomVel = {
            .x = (Random()*emmitor.velocityVariation.x) * (GetRandomValue(0, 1) ? -1 : 1),
            .y = (Random()*emmitor.velocityVariation.y) * (GetRandomValue(0, 1) ? -1 : 1)
        };
        float lifetime = emmitor.lifeTime + Random()*emmitor.lifeTimeVariation;
        Particle p = {
            .position = emmitor.position,
            .remainLifeTime =  lifetime,
            .lifeTime = lifetime,
            .velocity = vector2Add(emmitor.velocity, randomVel),
            .color = emmitor.colorBegin
        };
        add_particle(&particles, p);
    }

    SetTargetFPS(60);
    float delta = 0;
    while (!WindowShouldClose()) {
        delta = GetFrameTime();
        char buf[5] = {0};
        sprintf(buf, "%i", GetFPS());
        size_t c = particles.count;
        for(int i = c; i < emmit+c; i++) {
            Vector2 randomVel = {
                .x = (Random()*emmitor.velocityVariation.x) * (GetRandomValue(0, 1) ? -1 : 1),
                .y = (Random()*emmitor.velocityVariation.y) * (GetRandomValue(0, 1) ? -1 : 1)
            };
            float lifetime = emmitor.lifeTime + Random()*emmitor.lifeTimeVariation;
            Particle p = {
                .position = emmitor.position,
                .remainLifeTime =  lifetime,
                .lifeTime = lifetime,
                .velocity = vector2Add(emmitor.velocity, randomVel),
                .color = emmitor.colorBegin
            };
            add_particle(&particles, p);
        }
        update_particle(emmitor, &particles, delta);
        BeginDrawing();
            ClearBackground(BLACK);
            draw_particles(&particles, delta);
            DrawText(buf, 0, 0, 50, WHITE);
        EndDrawing();
    }
    free((void*)particles.items);

    CloseWindow();
    return 0;
}
