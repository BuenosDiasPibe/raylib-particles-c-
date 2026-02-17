using Raylib_cs;
using System.Numerics;
namespace raylib_particles;
public class ParticleEmmition(HSV hsvVariation)
{
    public Rectangle EmmiterPos;
    public Vector2 size, sizeVariation;
    public Color colorBegin;
    public Color colorEnd;
    public HSV hsvVariation = hsvVariation;
    public float lifeTime, lifeTimeVariation;
    public Vector2 velocity, velocityVariation;

    Random rand = new();

    public void Emmit(Particle[] particles, ref uint listIndex) {
        Particle p = particles[(int)listIndex-1];
        p.active = true;
        p.LifeTime = lifeTime+(float)(rand.NextDouble()*lifeTimeVariation);
        p.remainLifeTime = p.LifeTime;
        p.colorBegin = colorBegin;
        p.colorEnd = colorEnd;
        p.velocity = velocity+ new Vector2(
                    (float)(rand.NextDouble()*velocityVariation.X),
                    (float)(rand.NextDouble()*velocityVariation.Y)
        );
        var r = p.rec;
        r.Position = new Vector2(
                rand.Next((int)EmmiterPos.X, (int)(EmmiterPos.Width+EmmiterPos.X)),
                rand.Next((int)EmmiterPos.Y, (int)(EmmiterPos.Height+EmmiterPos.Y))
        );
        r.Size = new Vector2((float)(size.X+rand.NextDouble()*sizeVariation.X), (float)(size.Y+rand.NextDouble()*sizeVariation.Y));
        p.rec = r;

        particles[(int)listIndex-1] = p;

        listIndex = ((listIndex) % (uint)particles.Length)+1;
    }
}
