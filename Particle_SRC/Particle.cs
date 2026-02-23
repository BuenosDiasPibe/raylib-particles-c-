using Raylib_cs;
using System.Numerics;
namespace raylib_particles.Particle_SRC;
public struct Particle() {
    public Rectangle rec {get; set;}
    public Vector2 velocity;
    public Color colorBegin;
    public Color colorEnd;
    public float LifeTime;
    public float remainLifeTime;
}
