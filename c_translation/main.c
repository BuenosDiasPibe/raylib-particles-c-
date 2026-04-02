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
#define SCREEN_HEIGHT 700

typedef struct {
    Rectangle rec;
    Vector2 velocity, sizeStart;
    Color color, colorBegin, colorEnd;
    float lifeTime;
    float remainLifeTime;
} Particle;

typedef struct {
    Rectangle area;
    Vector2 sizeStart, sizeEnd, sizeVariation;
    Vector2 velocity, velocityVariation;
    Vector2 gravity;
    Color colorBegin, colorEnd;
    float lifeTime, lifeTimeVariation;
    size_t index,  // should be given by some component
    ammount, // ammount asked
    active;
} ParticleEmittor;

typedef struct {
    Particle* items;
    size_t used;
    size_t capacity;
} ParticleList;
typedef struct {
    ParticleEmittor *items;
    size_t count;
    size_t capacity;
} ParticleEmittorList;

typedef enum {
    SUCCESS = 0,
    IS_FULL,
    AMMOUNT_REDUCED,
    ERROR,
} AskEmmitorAmountResults;

AskEmmitorAmountResults askForEmmitorAmmount(ParticleEmittor *emmitor, ParticleList *particles){
    if(particles->used >= particles->capacity) {
        emmitor->ammount = 0;
        return IS_FULL;
    }
    if(particles->used+emmitor->ammount > particles->capacity) {
        if(particles->used >= particles->capacity-1) return ERROR;
        emmitor->ammount = particles->capacity - particles->used - 1;
        particles->used = particles->capacity;
        return AMMOUNT_REDUCED;
    }
    emmitor->index = particles->used;
    particles->used += emmitor->ammount;
    return SUCCESS;
}
void particleAlloc(ParticleList *particles) {
    particles->items = realloc(particles->items, particles->capacity*sizeof(Particle));
    if(particles->items == NULL) exit(1);
}
void emmitorListAppend(ParticleEmittorList *list, ParticleEmittor emmitor){
    if(list->count >= list->capacity) {
        if(list->capacity == 0) list->capacity = 256;
        else list->capacity *=2;
        list->items = realloc(list->items, sizeof(ParticleEmittor) * list->capacity);
    }
    list->items[list->count++] = emmitor;
}


void emmitParticle(ParticleList *particles, ParticleEmittor *emmitor){
    if(emmitor->active >= emmitor->ammount ||
       emmitor->index + emmitor->active+1 >= particles->capacity) return; // +1 to check if next emmitor.active number wouldnt overflow the capacity
    emmitor->active++;
    size_t in = emmitor->index+emmitor->active;

    float lifetime = emmitor->lifeTime + Random()*emmitor->lifeTimeVariation;
    particles->items[in] = (Particle){
        .rec            = {
            .x          = emmitor->area.x + Random()*emmitor->area.width,
            .y          = emmitor->area.y + Random()*emmitor->area.height,
        },
        .sizeStart      = (Vector2) {
            .x          = emmitor->sizeStart.x + Random()*emmitor->sizeVariation.x * (GetRandomValue(0, 1) ? -1 : 1),
            .y          = emmitor->sizeStart.y + Random()*emmitor->sizeVariation.y * (GetRandomValue(0, 1) ? -1 : 1)
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
    size_t final_index = 0;
    size_t point0 = emmitor->index;
    for(size_t point1 = emmitor->index; point1 < emmitor->active + emmitor->index; ++point1){
        particles->items[point0] = particles->items[point1];
        if(particles->items[point1].remainLifeTime > 0){
            point0++;
            final_index++;
        }
    }
    emmitor->active = final_index; // thank you Cyberpunk2007
}

void updateParticle(ParticleEmittor *emmitor, ParticleList *particles, const float delta) {
    float t = 0;
    Particle *p;
    for(size_t i = emmitor->index; i < emmitor->active+emmitor->index; ++i) {
        p = &particles->items[i];
        vector2Sub(&p->velocity, Vector2FloatMul(emmitor->gravity, delta));
        p->remainLifeTime -= delta;
        t                  = 1-(float)(p->remainLifeTime/p->lifeTime);
        p->rec             = (Rectangle){
            .x             = p->rec.x + (p->velocity.x*delta),
            .y             = p->rec.y + (p->velocity.y*delta),
            .width         = ffLerp(p->sizeStart.x, emmitor->sizeEnd.x, t),
            .height        = ffLerp(p->sizeStart.y, emmitor->sizeEnd.y, t)
        };
        p->color           = colorLerp(p->colorBegin, p->colorEnd, t);
    }
    removeDeathParticles(particles, emmitor);
}

void drawParticlesRec(const ParticleList *particles, const ParticleEmittor emmitor) {
    for(size_t i = emmitor.index; i < emmitor.active+emmitor.index; ++i) {
        Particle p = particles->items[i];
        DrawRectangleRec(p.rec, p.color);
    }
}
void drawParticlesImg(const ParticleList *particles, const ParticleEmittor emmitor, const  Texture2D *image) {
    for(size_t i = emmitor.index; i < emmitor.active+emmitor.index; ++i) {
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

// not important
// --------------------------------------
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
void makeFunnyThing(ParticleEmittorList *emmitor, ParticleList *particles, const int ammount){
    const Color c = ColorFromHSV(Random()*360, 1, 1);
    ParticleEmittor emmitorT = {
        .area = {(float)(SCREEN_WIDTH)/2, (float)(SCREEN_HEIGHT)/2, 1,1},
        .sizeStart ={10,10},
        .sizeEnd = (Vector2){0},
        .colorEnd = c,
        .colorBegin = WHITE,
        .velocityVariation = (Vector2){.x = 20, .y = 20},
        .lifeTime = 0,
        .lifeTimeVariation = 5,
        .ammount = ammount,
    };
    askForEmmitorAmmount(&emmitorT, particles);
    emmitorListAppend(emmitor, emmitorT);
}
void updateFunnyThing(ParticleEmittorList *emmitor, const Vector2 pos, const float hue, const float time){
    Vector2 rotator = pos;
    Vector2 past_velocity = {0};
    float s, c = 1;
    for(size_t e = 0; e < emmitor->count; e++){
        RecChangePosition(&emmitor->items[e].area, rotator);
        past_velocity = emmitor->items[e].velocity;
        s =   sinf(((e*e*time))/(PI*hue));
        c =   cosf(((e*PI*2*time))/(PI*e+hue));
        for(size_t i = 0; i < e; i++){
            s +=  cosf((  PI*2 * e*time ) / (PI*300+e) );
            c +=  sinf(( hue *e) / (PI*300+e) );
        }
        rotator = (Vector2){
            .x = emmitor->items[e].area.x - s+(s/7)*cosf(time),
            .y = emmitor->items[e].area.y - c+(c/7)*sinf(time),
        };

        emmitor->items[e].gravity = Vec2Distance(getRecPosition(emmitor->items[e].area), rotator);
        emmitor->items[e].velocity = Vec2Distance(emmitor->items[e].gravity, past_velocity);
        emmitor->items[e].colorBegin = ColorFromHSV(fmod( (hue+10*e), 360), 1, 1);
    }
}
// --------------------------------------

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "particles");
    const size_t max_particles = 20000;
    int emmit = 5;
    ParticleList particles = {.capacity = max_particles };
    particleAlloc(&particles);
    float hue = 10;
    ParticleEmittorList list = {0};
    const size_t ammount_particle_emitters = 100;
    for(size_t i = 0; i < ammount_particle_emitters; i++){
        ParticleEmittor emmitor = {
            .area = {(float)(SCREEN_WIDTH)/2, (float)(SCREEN_HEIGHT)/2, 10,10},
            .sizeStart ={10,10},
            .sizeEnd = {10,10},
            .colorBegin = ColorFromHSV(hue, 1, 1),
            //.colorEnd = BLACK,
            .velocityVariation = (Vector2){.x = 20, .y = 20},
            .gravity = {.x = 0, .y = -5},
            .lifeTime = 1,
            .lifeTimeVariation = 3,
            .ammount = max_particles/ammount_particle_emitters,
        };
        askForEmmitorAmmount(&emmitor, &particles);
        emmitorListAppend(&list, emmitor);
    }

    char fps_chr[20] = {0};
    char particle_count[100] = {0};

    SetTargetFPS(60);
    float delta = 0;
    const Vector2 mouse = {(float)(SCREEN_WIDTH)/2, (float)(SCREEN_HEIGHT)/2};
    Texture2D img = LoadTexture("Acover.png");

    Color bgClear = ColorFromHSV(hue, 1, 0.5f);
    float time = 0;

    while (!WindowShouldClose()) {
        time = GetTime();
        bgClear = ColorFromHSV(hue, 0.8f, .3f);
        delta = GetFrameTime()*2;
        hue = (float)(fmod(hue+delta*10, 360.f));

        for(size_t i = 0; i < list.count; i++) {
            for(int e = 0; e < emmit; ++e) {
                emmitParticle(&particles, &list.items[i]);
            }
        }
        updateFunnyThing(&list, mouse, hue, time);
        for(size_t i = 0; i < list.count; i++) {
            updateParticle(&list.items[i], &particles, delta);
        }

        sprintf(fps_chr, "fps: %i",GetFPS());
        //sprintf(particle_count, "p->active: %zu ; p->count: %zu",emmitor.active, particles.used);

        BeginDrawing();
            ClearBackground(bgClear);
            for(size_t i = 0; i < list.count; i++) {
                drawParticlesRec(&particles,list.items[i]);
                //DrawRectangleRec(list.items[i].area, list.items[i].colorBegin);
            }
            DrawText(fps_chr, 0, 0, 50, WHITE);
            DrawText(particle_count, 0, 50, 50, WHITE);
        EndDrawing();
    }
    free((void*)particles.items);
    free((void*)list.items);
    UnloadTexture(img);

    CloseWindow();
    return 0;
}
