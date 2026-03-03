using System.Diagnostics;
using System.Numerics;
using Raylib_cs;
using raylib_particles.Particle_SRC;

namespace raylib_particles.Scenes;

public class Test4(SceneManager manager) : IScene
{
    SceneManager sceneManager = manager;
    const int MAX_PARTICLES = 1_000_000;
    ParticleSystem ps;
    ParticleEmmition emmiter = new(MAX_PARTICLES);
    public void LoadContent() {
        ps = new(MAX_PARTICLES);
        Random r = new();
        emmiter.EmmiterPos = new(new(WindowProps.WIDTH/2, WindowProps.HEIGHT/2), new(0));
        emmiter.sizeStart = new(5);
        emmiter.sizeEnd = new(1);
        emmiter.sizeVariation = new(10);
        emmiter.colorBegin = Raylib.ColorFromHSV(r.NextSingle()*360, 1, 1);
        emmiter.colorEnd = Raylib.ColorFromHSV(r.NextSingle()*360, 1, 1);
        emmiter.colorEnd.A = 10;
        emmiter.velocityVariation = new(500,300);
        emmiter.lifeTime = 2f;
        emmiter.lifeTimeVariation = 1;
        ps.CreateEmmitor(emmiter);
    }

    int cap = 500;
    public void Update(float delta) {
        if(Raylib.IsKeyPressed(KeyboardKey.Enter)) {
            sceneManager.RemoveScene();
        }
        //emmiter.EmmiterPos.Position = new Vector2(WindowProps.WIDTH, -WindowProps.HEIGHT/2)+ Helper.Rotate(emmiter.EmmiterPos.Position, 1);
        if(Raylib.GetFPS() < 60) sceneManager.RemoveScene();
        ps.Emmit(cap);
        ps.Update(delta);
    }

    public void Draw(float delta) {
        ps.Draw(delta);
        Raylib.DrawText($"particles: {ps.particles_active_count()}", 0,0,50, Color.White);
        Raylib.DrawText($"FPS: {Raylib.GetFPS()}", 0,50,50,Color.Beige);
    }

    public void UnloadContent() {
        Console.WriteLine($"cap: {cap}\nparticles: {ps.particles_active_count()}");
        ps.Unload();
    }

}
