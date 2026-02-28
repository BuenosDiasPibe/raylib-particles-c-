using Raylib_cs;
using System.Diagnostics;
using System.Numerics;
namespace raylib_particles.Particle_SRC;
public class ParticleSystem
{
    public uint listIndex = 0;
    public uint particleEmmiterIndex = 0;
    public int c = 0;
    public List<ParticleEmmition> emmiters {get; private set;} = new();
    private readonly Rectangle ViewPort = new(0,0,WindowProps.WIDTH, WindowProps.HEIGHT);
    Stopwatch sw = new();
    long maxTimeWait = 0;

    Random rand = new();

    public ParticleSystem(uint ammount_particles) {
        listIndex = ammount_particles;
        particleEmmiterIndex = ammount_particles;
    }
    public void CreateEmmitor(ParticleEmmition emmitor) {
        if(particleEmmiterIndex <= 0){
            //throw new IndexOutOfRangeException("range requested by emmitor is bigger than what ammount of particles");
            Console.WriteLine("range requested by emmitor is bigger than what ammount of particles");
            return;
        }
        emmitor.setEmmiter(emmitor.particleListLenght);
        particleEmmiterIndex -= emmitor.particleListLenght;
        emmiters.Add(emmitor);
    }

    public void Update(float delta) {
        sw.Start();
        foreach(ParticleEmmition emiter in emmiters) {
            emiter.Update(delta);
        }
        sw.Stop();
        if(maxTimeWait < sw.ElapsedMilliseconds){
            maxTimeWait = sw.ElapsedMilliseconds;
            Console.WriteLine($"update: {maxTimeWait}ms");
        }

        sw.Reset();
    }
    public void Draw(float delta) {
        foreach(var e in emmiters) {
            e.Draw(delta);
        }
    }
    public void Emmit(int ammount) {
        foreach(ParticleEmmition m in emmiters) {
            m.Emmit(ammount);
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
        emmiters.Clear();
    }
}
