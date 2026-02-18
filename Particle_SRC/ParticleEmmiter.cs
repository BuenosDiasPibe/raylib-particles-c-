using Raylib_cs;
using System.Numerics;
namespace raylib_particles.Particle_SRC;
public class ParticleEmmition(uint particleListLenght,HSV hsvVariation) {
    public Rectangle EmmiterPos;
    public Color colorBegin;
    public Color colorEnd;
    public HSV hsvVariation = hsvVariation;
    public Vector2 sizeStart, sizeEnd,  sizeVariation;
    public Vector2 velocity, velocityVariation;
    public Vector2 gravity;
    public float lifeTime, lifeTimeVariation;

    public uint particleListIndex = 0;
    public uint particleListLenght = particleListLenght;
    public int particleActive = 0;

    Random rand = new();
    public void setEmmiter(uint index) {
        particleListIndex = index;
        particleActive = (int)index;
    }

    public void Emmit(List<Particle> particles, int ammount) {
        for(int particle = 0; particle < ammount; particle++){

            particleActive = (particleActive+1) % (int)(particleListIndex+particleListLenght);
            if(particleActive == 0) particleActive = (int)particleListIndex; // i hate my puppy life

            Particle p = particles[particleActive];

            p.active = true;
            p.LifeTime = lifeTime+(float)(rand.NextDouble()*lifeTimeVariation * rand.Next(-1,1));
            p.remainLifeTime = p.LifeTime;
            p.colorBegin = colorBegin;
            p.colorEnd = colorEnd;
            p.velocity = velocity + new Vector2(
                    rand.NextSingle()*velocityVariation.X * rand.Next(-1,1),
                    rand.NextSingle()*velocityVariation.Y * rand.Next(-1,1)
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

    public void Update(List<Particle> particles) {
        float delta = Raylib.GetFrameTime();
        for(int count = (int)particleListIndex; count < particleListLenght+particleListIndex; count++) {
            if(!particles[count].active) continue;
            var p = particles[count];

            p.remainLifeTime -= delta;
            p.velocity -= gravity * delta;
            Rectangle r = p.rec;
            r.Position += p.velocity * delta;
            r.Size = Vector2.Lerp(sizeStart, sizeEnd, 1-(float)(p.remainLifeTime/p.LifeTime));
            
            p.rec = r;

            if(p.remainLifeTime <= 0) {
                p.active = false;
                particles[count] = p;
                continue;
            }

            particles[count] = p;
        }
    }
    public void Draw(List<Particle> particles){
        for(int count = (int)particleListIndex; count < particleListLenght+particleListIndex; count++) {
            Particle p = particles[count];
            if(!p.active) continue;
            Color lerp = Raylib.ColorLerp(p.colorBegin, p.colorEnd, 1-(float)(p.remainLifeTime/p.LifeTime));
            Raylib.DrawRectangleRec(p.rec, lerp);
            }
    }
}
