using Raylib_cs;
using System.Numerics;
namespace raylib_particles;
public class ParticleSystem
{
    public List<Particle> particles {get; private set;} = new();
    public uint listIndex = 0;
    public uint particleEmmiterIndex = 0;
    public int c = 0;
    public List<ParticleEmmition> emmiters {get; private set;} = new();

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
            throw new IndexOutOfRangeException("range requested by emmitor is bigger than what ammount of particles");
        }
        emmitor.setEmmiter(particleEmmiterIndex);
        particleEmmiterIndex += emmitor.particleListLenght;
        emmiters.Add(emmitor);
    }

    public void Update() {
        foreach(ParticleEmmition emiter in emmiters) {
            emiter.Update(particles);
        }
    }
    public void Draw() {
        foreach(ParticleEmmition emmit in emmiters) {
            emmit.Draw(particles);
        }
    }
    public void Emmit() {
        foreach(ParticleEmmition m in emmiters) {
            m.Emmit(particles);
        }
    }
    public int particles_active_count() {
        return particles.Where(p => p.active).Count();
    }
}
