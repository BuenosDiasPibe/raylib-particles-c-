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

    public uint particleListIndex = 0;
    public uint particleListLenght = particleListLenght;
    public int particleActive = 0;

    private readonly int[] sign = [-1,1];
    Random rand = new();
    public List<int> particleActiveID {get; private set;} = new();

    public void setEmmiter(uint index) {
        particleListIndex = index;
        particleActive = (int)index;
    }

    public void Emmit(List<Particle> particles, int ammount) {
        for(int particle = 0; particle < ammount; particle++){
            particleActive = (particleActive+1) % (int)(particleListIndex+particleListLenght);
            if(particleActive == 0) particleActive = (int)particleListIndex; // i hate my puppy life

            if(particleActiveID.Contains(particleActive)){ 
                continue;
            }

            particleActiveID.Add(particleActive);
            Particle p = particles[particleActive];

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

            particles[particleActive] = p;
        }
    }

    public void Update(List<Particle> particles, float delta) {
        float t = 0;
        foreach(var i in particleActiveID.ToList()) {
            Particle p = particles[i];

            p.remainLifeTime -= delta;
            t = 1-(float)(p.remainLifeTime/p.LifeTime);
            p.velocity -= gravity * delta;
            Rectangle r = p.rec;
            r.Position += p.velocity * delta;
            r.Size =LERP(sizeStart, sizeEnd, t);
            p.rec = r;
            p.color = Raylib.ColorLerp(colorBegin, colorEnd, t);

            if(p.remainLifeTime <= 0){
                particleActiveID.Remove(i);
            }

            particles[i] = p;
        }
    }
    public void Draw(List<Particle> particles, float delta) {
        foreach(int i in particleActiveID) {
            Particle p = particles[i];
            Raylib.DrawRectangleRec(p.rec, p.color);
        }
    }
    public int getActiveParticles() {return particleActiveID.Count;}
    private static Vector2 LERP(Vector2 v1, Vector2 v2, float t){ return (1-t)*v1-v2*t; }
}
