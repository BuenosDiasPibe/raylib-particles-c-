using Raylib_cs;
using System.Numerics;
namespace raylib_particles;
public class ParticleEmmition(HSV hsvVariation) {
    public Rectangle EmmiterPos;
    public Vector2 size, sizeVariation;
    public Color colorBegin;
    public Color colorEnd;
    public HSV hsvVariation = hsvVariation;
    public float lifeTime, lifeTimeVariation;
    public Vector2 velocity, velocityVariation;

    public uint particleListIndex = 0;
    public uint particleListLenght = 1;
    public int particleActive = 0;

    Random rand = new();
    public void setEmmiter(uint index) {
        particleListIndex = index;
        particleActive = (int)index;
    }

    public void Emmit(List<Particle> particles) {
        particleActive = (particleActive+1) % (int)(particleListIndex+particleListLenght);
        Particle p = particles[particleActive];

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

        particles[particleActive] = p;
    }

    public void Update(List<Particle> particles) {
        for(int count = (int)particleListIndex; count < particleListLenght+particleListIndex; count++) {
            if(!particles[count].active) continue;
            var p = particles[count];

            p.remainLifeTime -= Raylib.GetFrameTime();
            Rectangle r = p.rec;
            r.Position += p.velocity * new Vector2(Raylib.GetFrameTime());
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
            Color lerp = Raylib.ColorLerp(p.colorEnd, p.colorBegin, (float)(p.remainLifeTime/p.LifeTime));
            Raylib.DrawRectangleRec(p.rec, lerp);
            }
    }
}
