using Raylib_cs;
using System.Diagnostics;
using System.Numerics;
namespace raylib_particles.Particle_SRC;
public class ParticleSystem
{
    public List<Particle> particles {get; private set;} = new();
    public uint listIndex = 0;
    public uint particleEmmiterIndex = 0;
    public int c = 0;
    public List<ParticleEmmition> emmiters {get; private set;} = new();
    private readonly Rectangle ViewPort = new(0,0,WindowProps.WIDTH, WindowProps.HEIGHT);
    Stopwatch sw = new();

    Random rand = new();

    public ParticleSystem(uint ammount_particles) {
        listIndex = ammount_particles;
        particles.EnsureCapacity((int)ammount_particles);
        for(int i = 0; i < ammount_particles; i++) {
            particles.Add(new());
        }
    }
    public void CreateEmmitor(ParticleEmmition emmitor) {
        if(particleEmmiterIndex + emmitor.particleListLenght > listIndex){
            //throw new IndexOutOfRangeException("range requested by emmitor is bigger than what ammount of particles");
            Console.WriteLine("range requested by emmitor is bigger than what ammount of particles");
            return;
        }
        emmitor.setEmmiter(particleEmmiterIndex);
        particleEmmiterIndex += emmitor.particleListLenght;
        emmiters.Add(emmitor);
    }

    public void Update(float delta) {
        foreach(ParticleEmmition emiter in emmiters) {
            emiter.Update(particles, delta);
        }
    }
    public void Draw(float delta) {
        foreach(var e in emmiters) {
            e.Draw(particles, delta);
        }
    }
    public void Emmit(int ammount) {
        foreach(ParticleEmmition m in emmiters) {
            m.Emmit(particles, ammount);
        }
    }
    public int particles_active_count() {
        int a = 0;
        foreach(var e in emmiters){
            a+=e.getActiveParticles();
        }
        return a;
    }
    public void Unload(){
        particles.Clear();
        emmiters.Clear();
    }
}
