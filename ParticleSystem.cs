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
        foreach(Particle p in particles) {
            if(!p.active) continue;

            if(p.remainLifeTime <= 0) {
                p.active = false;
                continue;
            }

            p.remainLifeTime -= Raylib.GetFrameTime();
            p.rec.Position += p.velocity * new Vector2(Raylib.GetFrameTime());
        }
    }
    public void Draw() {
        foreach(Particle p in particles) {
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
