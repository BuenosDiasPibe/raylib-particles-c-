#include "./math.h"
#include "raylib.h"

Vector2 vector2Add(const Vector2 v1,const Vector2 v2) {
    return (Vector2) {
        .x = v1.x+v2.x,
        .y = v1.y+v2.y
    };
}
Vector2 vector2Mul(const Vector2 v1,const Vector2 v2) {
    return (Vector2) {
        .x = v1.x*v1.x,
        .y = v2.y*v2.y
    };
}
Vector2 Vector2FloatMul(const Vector2 v,const float f) {
    return (Vector2) {
        .x = v.x*f,
        .y = v.y*f
    };
}
void vector2Sub(Vector2 *v1, const Vector2 v2) {
    v1->x -= v2.x;
    v1->y -= v2.y;
}
void RecVec2AddPosition(Rectangle *r1, const Vector2 v2) {
    r1->x += v2.x;
    r1->y += v2.y;
}
void RecVec2AddSize(Rectangle *r1, const Vector2 v2) {
    r1->width += v2.x;
    r1->height += v2.y;
}
void RecRecAdd(Rectangle *r1, const Rectangle r2){
    r1->width  +=r2.width;
    r1->height +=r2.height;
}
Rectangle RecVecSizeLerp(const Rectangle r1,const Vector2 v2,const float t) {
    return (Rectangle) {
        .x = r1.x,
        .y = r1.y,
        .width  = r1.width*(1-t) + v2.x*t,
        .height = r1.height*(1-t) + v2.y*t
    };
}
Color colorLerp(const Color color1,const Color color2, const float t) {
    return (Color){
        (unsigned char)((1.f-t)*color1.r + color2.r*t),
        (unsigned char)((1.f-t)*color1.g + color2.g*t),
        (unsigned char)((1.f-t)*color1.b + color2.b*t),
        (unsigned char)((1.f-t)*color1.a + color2.a*t),
    };
}
float ffLerp(const float f1, const float f2, const float t) {
    return (1-t)*f1 + f2*t;
}

float Random(){ // between 0 and 1
    return GetRandomValue(0, 327867) / 327867.0;
}
void RecChangePosition(Rectangle *r, const Vector2 pos){
    r->x = pos.x;
    r->y = pos.y;
}
Vector2 Vec2Distance(const Vector2 p1, const Vector2 p2){
    return (Vector2){
        .x = p1.x-p2.x,
        .y = p1.y-p2.y
    };
}
Vector2 getRecPosition(const Rectangle r){
    return (Vector2){.x = r.x, .y = r.y};
}
Color ColorInvert(Color color){
    return (Color){
        .r = 255 - color.r,
        .g = 255 - color.g,
        .b = 255 - color.b,
        .a = 255
    };
}

void vector2FloatSub(Vector2 *v, const float f) {
    v->x -= f;
    v->y -= f;
}
