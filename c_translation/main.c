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
    Vector2 sizeStart, sizeEnd, sizeVariation;
    Vector2 position;
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

typedef enum {
    SUCCESS = 0,
    IS_FULL,
    AMMOUNT_REDUCED,
    ERROR,
} AskEmmitorAmountResults;

int askForEmmitorAmmount(ParticleEmittor *emmitor, ParticleList *particles){
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

void emmitParticle(ParticleList *particles, ParticleEmittor *emmitor){
    if(emmitor->active >= emmitor->ammount ||
       emmitor->index + emmitor->active+1 >= particles->capacity) return; // +1 to check if next emmitor.active number wouldnt overflow the capacity
    emmitor->active++;
    size_t in = emmitor->index+emmitor->active;

    float lifetime = emmitor->lifeTime + Random()*emmitor->lifeTimeVariation;
    particles->items[in] = (Particle){
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
    size_t final_index = 0;
    for(size_t point1 = emmitor->index; point1 < emmitor->active + emmitor->index; ++point1){
        particles->items[point0] = particles->items[point1];
        if(particles->items[point1].remainLifeTime > 0){
            point0++;
            final_index++;
        }
    }
    emmitor->active = final_index; // thank you Cyberpunk2007
}

void updateParticle(ParticleEmittor *emmitor, ParticleList *particles, float delta) {
    float t = 0;
    for(int i = emmitor->index; i < emmitor->active+emmitor->index; ++i) {
        Particle *p = &particles->items[i];
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

void drawParticlesRec(ParticleList *particles, ParticleEmittor emmitor, float delta) {
    if(emmitor.active == 0) return;
    for(size_t i = emmitor.index; i < emmitor.active+emmitor.index; ++i) {
        Particle p = particles->items[i];
        DrawRectangleRec(p.rec, p.color);
    }
}
void drawParticlesImg(ParticleList *particles, ParticleEmittor emmitor, float delta,  Texture2D *image) {
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

void test1(ParticleEmittorList *list, ParticleList *particles, Vector2 position){
    if(particles->used >= particles->capacity) return;
    Color c = ColorFromHSV(Random()*360, 1, 1);
    ParticleEmittor emmitorT = {
        .position = position,
        .sizeStart ={10,10},
        .sizeEnd = (Vector2){0},
        .colorBegin = c,
        .colorEnd = 0,
        .velocityVariation = (Vector2){.x = 20, .y = 20},
        //.velocity = (Vector2){.x = 20, .y = 20},
        .lifeTime = 2,
        .lifeTimeVariation = 5,
        .gravity = (Vector2){0,-40},
        .ammount = 255,
        .active = 0
    };
    const int code = askForEmmitorAmmount(&emmitorT, particles);
    printf("addedWithCode: %i\n", code);
    if(code != 2) {
        emmitorListAppend(list, emmitorT);
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "particles");
    int emmit = 255;
    ParticleList particles = {.capacity = 65025 };
    particleAlloc(&particles);
    float hue = 10;
    ParticleEmittorList emmitors = {0};
    test1(&emmitors, &particles, (Vector2){20,20});
    char fps_chr[5] = {0};
    char particles_chr[100] = {0};
    char emmit_shower[50] = {0};

    SetTargetFPS(60);
    float delta = 0;
    Texture2D img = LoadTexture("Acover.png");

    while (!WindowShouldClose()) {
        delta = GetFrameTime()*2;
        hue = (float)(fmod(hue+delta*10, 360.f));
        emmitors.items[0].colorBegin = ColorFromHSV(hue, 1, 1);
        emmitors.items[0].position = GetMousePosition();
        emmitors.items[0].velocity = GetMouseDelta();
        //if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        for(int j = 0; j < emmitors.count; ++j){
            for(int i = 0; i < emmit*Random(); ++i) {
                emmitParticle(&particles, &emmitors.items[j]);
            }
            updateParticle(&emmitors.items[j], &particles, delta);
        }
        //}
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            test1(&emmitors, &particles, GetMousePosition());
        }
        if(IsKeyPressed(KEY_ENTER) && emmitors.count > 0){
            size_t delete =  emmitors.items[emmitors.count-1].ammount;
            particles.used -= delete;
            assert(particles.used < particles.capacity);
            emmitors.count--;
        }

        sprintf(fps_chr, "%i", GetFPS());
        sprintf(particles_chr, "used: %zu - capacity: %zu", particles.used, particles.capacity);
        sprintf(emmit_shower, "eps:%i", emmit);
        BeginDrawing();
            ClearBackground(BLACK);
            for(int i = 0; i < emmitors.count; i++) {
                drawParticlesRec(&particles,emmitors.items[i], delta);
            }
            DrawText(fps_chr, 0, 0, 50, WHITE);
            DrawText(particles_chr, 0, 50, 50, WHITE);
            DrawText(emmit_shower, 0, 100, 50, WHITE);
        EndDrawing();
    }
    free((void*)particles.items);
    free((void*)emmitors.items);
    UnloadTexture(img);

    CloseWindow();
    return 0;
}
