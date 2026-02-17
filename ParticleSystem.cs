using Raylib_cs;
using System.Numerics;
namespace raylib_particles;
public class ParticleSystem
{
    public List<Particle> particles {get; private set;}= new();
    public uint listIndex = 0;
    public List<ParticleEmmition> emmiters {get; private set;} = new();

    Random rand = new();

    public ParticleSystem(uint ammount_particles) {
        listIndex = ammount_particles;
        particles.EnsureCapacity((int)listIndex);
        for(int i = 0; i < ammount_particles; i++) {
            Particle p = new();
            particles.Add(p);
        }
    }
    public void CreateEmmitor(ParticleEmmition emmitor) {
        emmiters.Add(emmitor);
    }

    public void Update() {
        for(int count = 0; count < particles.Count; count++) {
            var p = particles[count];
            if(!p.active) continue;


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
    public void Draw() {
        foreach(Particle p in particles) {
            if(!p.active) continue;
            Color lerp = Color.Lerp(p.colorEnd, p.colorBegin, (float)(p.remainLifeTime/p.LifeTime));
            Raylib.DrawRectangleRec(p.rec, lerp);
        }
    }
    public void Emmit() {
        foreach(ParticleEmmition m in emmiters) {
            m.Emmit(particles, ref listIndex);
        }
    }
    public int particles_active_count() {
        return particles.Where(p => p.active).Count();
    }
}
