using System.Numerics;
using Raylib_cs;
using raylib_particles.Particle_SRC;

namespace raylib_particles.Scenes;

public class testScene1(SceneManager sceneManager) : IScene
{
    ParticleSystem ps = new(1_000_000);
    Random r = new();
    SceneManager sceneManager = sceneManager;

    public void LoadContent() {
        WindowProps.bgClear = new(28,28,28,255);
    }

    public void Update(float delta) {
        if(Raylib.IsKeyPressed(KeyboardKey.Enter)) {
            sceneManager.RemoveScene();
        }
        if(Raylib.IsKeyDown(KeyboardKey.LeftControl) && Raylib.IsMouseButtonDown(MouseButton.Left)) {
            Vector2 p = Raylib.GetMousePosition();
            ps.CreateEmmitor(test_create_bunch_of_stuff(new Rectangle(p, new Vector2(1)), new(r.NextSingle()*50*r.Next(-10,20), r.NextSingle()*10*r.Next(-10,35))));
            ps.Emmit(100);
        }
        if(Raylib.IsMouseButtonPressed(MouseButton.Left)) {
            Vector2 p = Raylib.GetMousePosition();
            ps.CreateEmmitor(test_create_bunch_of_stuff(new Rectangle(p, new Vector2(1)), new(r.NextSingle()*50*r.Next(-10,20), r.NextSingle()*10*r.Next(-10,35))));
            ps.Emmit(100);
        }
        if(Raylib.IsKeyDown(KeyboardKey.P)){
            ps.Emmit(100);
        }
        ps.Update(delta);
    }
    public void Draw(float delta) {
        ps.Draw(delta);
        Raylib.DrawText("p: "+ps.particles_active_count().ToString() + " - c: "+ps.listIndex, 0,0,50, Color.White);
        Raylib.DrawText("fps: "+Raylib.GetFPS().ToString(), 0,50,50, Color.White);
        Raylib.DrawText($"emmiters: {ps.emmiters.Count}", 0, 100, 25, Color.White);
    }

    public void UnloadContent() {
        ps.Unload();
    }

    public ParticleEmmition test_create_bunch_of_stuff(Rectangle pos, Vector2 velocity){
        Random r = new();
        ParticleEmmition emmiter = new(10000);
        emmiter.EmmiterPos = pos;
        emmiter.sizeStart = new(5);
        emmiter.sizeEnd = new(1);
        emmiter.sizeVariation = new(0);
        emmiter.colorBegin = Raylib.ColorFromHSV(r.NextSingle()*360, 1, 1);
        emmiter.colorEnd = Raylib.ColorFromHSV(r.NextSingle()*360, 1, 1);
        emmiter.colorEnd.A = 10;
        emmiter.velocity = velocity;
        emmiter.velocityVariation = new(500,300);
        emmiter.gravity = new Vector2(0,-100);
        emmiter.lifeTime = 2f;
        emmiter.lifeTimeVariation = 1;
        return emmiter;
    }
}
