#ifndef MATH_H
#define MATH_H
#include <raylib.h>

Vector2 vector2Add(Vector2 v1, Vector2 v2);
Vector2 vector2Mul(Vector2 v1, Vector2 v2);
Vector2 Vector2FloatMul(Vector2 v, float f);
void vector2Sub(Vector2 *v1, Vector2 v2);
void RecVec2AddPosition(Rectangle *r1, Vector2 v2);
void RecVec2AddSize(Rectangle *r1, Vector2 v2);
void RecRecAdd(Rectangle *r1, Rectangle r2);
Rectangle RecVecSizeLerp(Rectangle r1, Vector2 v2, float t);
float ffLerp(float f1, float f2, float t);
float Random();
Color colorLerp(Color color1, Color color2, float t);

#endif // MATH_H
