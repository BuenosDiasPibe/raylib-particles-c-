using Raylib_cs;
using System.Numerics;
namespace raylib_particles.Particle_SRC;
public class Particle() {
    public Rectangle rec {get; set;}
    public Vector2 velocity;
    public Color color;
    public float LifeTime;
    public float remainLifeTime;
}
