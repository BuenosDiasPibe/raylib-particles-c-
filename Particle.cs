using Raylib_cs;
using System.Numerics;
namespace raylib_particles;
public class Particle {
    public Rectangle rec;
    public Vector2 velocity;
    public Color colorBegin;
    public Color colorEnd;
    public float LifeTime;
    public float remainLifeTime;

    public bool active = false;

}
