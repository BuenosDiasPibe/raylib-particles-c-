using Raylib_cs;
using System.Numerics;
namespace raylib_particles.Particle_SRC;
public class ParticleEmmition(uint particleListLenght) {
    public Rectangle EmmiterPos;
    public Color colorBegin;
    public Color colorEnd;
    public Vector2 sizeStart, sizeEnd,  sizeVariation;
    public Vector2 velocity, velocityVariation;
    public Vector2 gravity;
    public float lifeTime, lifeTimeVariation;

    public uint particleListLenght = particleListLenght;

    private List<Particle> particlesActive = new();
    private List<Particle> particlesDeath = new();
    private int index;

    private readonly int[] sign = [-1,1];

    Random rand = new();
    public void setEmmiter(uint ammount) {
        particlesActive.EnsureCapacity((int)particleListLenght);
        for(int a = 0; a < particleListLenght; a++){
            particlesDeath.Add(new());
        }
    }

    public void Emmit(int ammount) {
        if( particlesActive.Count + ammount > particleListLenght) return;
        int count = particlesDeath.Count-1;
        for(int particle = 0; particle < ammount; particle++){
            Particle p = particlesDeath[particlesActive.Count];

            p.LifeTime = lifeTime+(float)(rand.NextDouble()*lifeTimeVariation * rand.GetItems(sign,1)[0]);
            p.remainLifeTime = p.LifeTime;
            p.color = colorBegin;
            p.velocity = velocity + new Vector2(
                    rand.NextSingle()*velocityVariation.X * rand.GetItems(sign, 1)[0],
                    rand.NextSingle()*velocityVariation.Y * rand.GetItems(sign, 1)[0]
            );
            var r = p.rec; // idk how to do this without a temp variable
            r.Position = new Vector2(
                    rand.Next((int)EmmiterPos.X, (int)(EmmiterPos.Width+EmmiterPos.X)),
                    rand.Next((int)EmmiterPos.Y, (int)(EmmiterPos.Height+EmmiterPos.Y))
            );
            r.Size = new Vector2(
                    (float)(sizeStart.X+rand.NextDouble()*sizeVariation.X),
                    (float)(sizeStart.Y+rand.NextDouble()*sizeVariation.Y)
            );
            p.rec = r;

            particlesActive.Add(p);
        }
    }

    public void Update(float delta) {
        float t = 0;
        int count = particlesActive.Count;
        for(int i = 0; i < count; i++) {
            Particle p = particlesActive[i];

            p.remainLifeTime -= delta;
            t = 1-(float)(p.remainLifeTime/p.LifeTime);
            p.velocity -= gravity * delta;
            Rectangle r = p.rec;
            r.Position += p.velocity * delta;
            r.Size =LERP(sizeStart, sizeEnd, t);
            p.rec = r;
            p.color = Raylib.ColorLerp(colorBegin, colorEnd, t);
            if(p.remainLifeTime <= 0){
                particlesActive.RemoveAt(i);
                count--;
            }else{
                particlesActive[i] = p;
            }
        }
    }
    public void Draw(float delta) {
        foreach(var p in particlesActive) {
            Raylib.DrawRectangleRec(p.rec, p.color);
        }
    }
    public int getActiveParticles() {return particlesActive.Count;}
    private static Vector2 LERP(Vector2 v1, Vector2 v2, float t){
        return (1-t)*v1-v2*t;
    }
    private static Color LERP(Color c1, Color c2, float t) {
        return new(
                c1.R + (c1.R-c2.R)*t,
                c1.G + (c1.G-c2.G)*t,
                c1.B + (c1.B-c2.B)*t,
                c1.A + (c1.A-c2.A)*t
        );
    }
}
