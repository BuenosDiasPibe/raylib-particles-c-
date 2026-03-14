#ifndef MATH_H
#define MATH_H
#include <raylib.h>

Vector2 vector2Add(const Vector2 v1,const  Vector2 v2);
Vector2 vector2Mul(const Vector2 v1,const  Vector2 v2);
Vector2 Vector2FloatMul(const Vector2 v,const float f);
Vector2 Vec2Distance(const Vector2 p1, const Vector2 p2);
Vector2 getRecPosition(const Rectangle r);
void vector2Sub(Vector2 *v1, Vector2 v2);
void vector2FloatSub(Vector2 *v, const float f);

void RecVec2AddPosition(Rectangle *r1, const Vector2 v2);
void RecChangePosition(Rectangle *r, const Vector2 pos);
void RecVec2AddSize(Rectangle *r1, const Vector2 v2);
void RecRecAdd(Rectangle *r1, const Rectangle r2);
Rectangle RecVecSizeLerp(const Rectangle r1, const Vector2 v2, const float t);

float ffLerp(const float f1,const float f2, const float t);
float Random();
Color colorLerp(const Color color1,const Color color2, float t);
Color ColorInvert(Color color);

#endif // MATH_H
