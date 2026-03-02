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
Vector2 Vector2Neg(Vector2 v) {
    return (Vector2) {.x = -v.x, .y = -v.y};
}
Vector2 vector2Sub(Vector2 v1, Vector2 v2) {
    return (Vector2) {
        .x = v1.x-v2.x,
        .y = v1.y-v2.y
    };
}
Rectangle RecVec2AddPosition(Rectangle r1, Vector2 v2) {
    return (Rectangle) {
        .x = r1.x + v2.x,
        .y = r1.y + v2.y,
        .width = r1.width,
        .height = r1.height
    };
}
Rectangle RecVec2AddSize(Rectangle r1, Vector2 v2) {
    return (Rectangle) {
        .x = r1.x,
        .y = r1.y,
        .width = r1.width + v2.x,
        .height = r1.height + v2.y
    };
}
Rectangle RecRecAdd(Rectangle r1, Rectangle r2){
    return (Rectangle) {
        .x = r1.x+r2.x,
        .y = r1.y+r2.y,
        .width = r1.width+r2.width,
        .height = r1.height+r2.height
    };
}

float Random(){ // between 0 and 1
    return (float)(GetRandomValue(0, 327867)) / 327867;
}

typedef struct {
    Color color;
    Rectangle rec;
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
    Rectangle size, sizeVariation;
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
    for(size_t i = index; i < particles->count; i++) { // probably much slower, but avoids ghost particles
        particles->items[i] = particles->items[i+1];
    }
    // this algorithm for some reason gets particles which no longer exist. So it creates ghost particles
    //particles->items[index] = particles->items[particles->count-1]; // scratch that, this literally destroys the visuals, im dumb
    particles->count--;
}

void update_particle(ParticleEmittor emmitor, ParticleList *particles, float delta) {
    float t = 0;
    size_t count = particles->count;
    for(int i = 0; i < count; ++i) {
        Particle *p = &particles->items[i];
        p->remainLifeTime -= delta;
        t = 1-(float)(p->remainLifeTime/p->lifeTime);
        p->velocity = vector2Sub(p->velocity, Vector2FloatMul(emmitor.gravity, delta));
        p->rec = RecVec2AddPosition(p->rec,Vector2FloatMul(p->velocity, delta));
        p->color = ColorLerp(emmitor.colorBegin, emmitor.colorEnd, t);
        if(p->remainLifeTime <= 0) {
            remove_at(particles, i);
            count--;
        }
    }
}

void draw_particles_rec(ParticleList *particles, float delta) {
    for(size_t i = 0; i <= particles->count; i++) {
        Particle p = particles->items[i];
        DrawRectangleRec(
            (Rectangle){.x=p.rec.x, .y=p.rec.y, .width = 10, .height = 10},
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
    Vector2 randomVel = {
        .x = (Random()*emmitor.velocityVariation.x) * (GetRandomValue(0, 1) ? -1 : 1),
        .y = (Random()*emmitor.velocityVariation.y) * (GetRandomValue(0, 1) ? -1 : 1)
    };
    float lifetime = emmitor.lifeTime + Random()*emmitor.lifeTimeVariation;
    Rectangle sizeVariation = {
        .x      = (Random()*emmitor.sizeVariation.x) * (GetRandomValue(0, 1) ? -1 : 1),
        .y      = (Random()*emmitor.sizeVariation.y) * (GetRandomValue(0, 1) ? -1 : 1),
        .width  = (Random()*emmitor.sizeVariation.width) * (GetRandomValue(0, 1) ? -1 : 1),
        .height = (Random()*emmitor.sizeVariation.height) * (GetRandomValue(0, 1) ? -1 : 1)
    };
    Rectangle final = RecRecAdd(emmitor.size, sizeVariation);
    Particle p = {
        .rec = (Rectangle){emmitor.position.x, emmitor.position.y, final.width, final.height},
        .remainLifeTime =  lifetime,
        .lifeTime = lifetime,
        .velocity = vector2Add(emmitor.velocity, randomVel),
        .color = emmitor.colorBegin
    };
    add_particle(particles, p);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "particles");
    int emmit = 40;
    ParticleList particles = {0};
    ParticleEmittor emmitor = {
        .position = (Vector2){.x = (float)(SCREEN_WIDTH)/2, .y = (float)(SCREEN_HEIGHT)/2},
        .size = (Rectangle){0,0, .width = 10, .height = 10},
        .sizeVariation = (Rectangle){0,0, .width = 50, .height = 50},
        .colorBegin = ColorFromHSV(Random()*360, 1, 1),
        .colorEnd = {0},
        .velocity = (Vector2){0},
        .velocityVariation = (Vector2){.x = 30, .y = 30},
        .lifeTime = 1,
        .lifeTimeVariation = 5,
    };
    for(int i = 0; i < emmit; i++) {
        emmit_particle(&particles, emmitor);
    }
    char fps_chr[5] = {0};
    char particles_chr[100000] = {0};
    char emmit_shower[1000] = {0};

    //SetTargetFPS(60);
    float delta = 0;
    Texture2D img = LoadTexture("p.jpeg");

    while (!WindowShouldClose()) {
        delta = GetFrameTime();
        emmitor.position = GetMousePosition();
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            for(int i = 0; i < emmit; i++) {
                emmit_particle(&particles, emmitor);
            }
        }
        update_particle(emmitor, &particles, delta);
        sprintf(particles_chr, "pActive:%zu, pPool:%zu", particles.count, particles.capacity);
        sprintf(fps_chr, "%i", GetFPS());
        sprintf(emmit_shower, "eps:%i", emmit);
        BeginDrawing();
            ClearBackground(BLACK);
            draw_particles_img(&particles, delta, &img);
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
