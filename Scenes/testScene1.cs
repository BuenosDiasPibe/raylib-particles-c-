using System.Numerics;
using Raylib_cs;
using raylib_particles.Particle_SRC;

namespace raylib_particles.Scenes;

public class testScene1 : IScene
{
    ParticleSystem ps = new(10000);
    Random r = new();

    public void LoadContent() {
        WindowProps.bgClear = new(28,28,28,255);
    }

    public void Update(float delta) {
        if(Raylib.IsMouseButtonPressed(MouseButton.Left)) {
            Vector2 p = Raylib.GetMousePosition();
            ps.CreateEmmitor(test_create_bunch_of_stuff(new Rectangle(p, new Vector2(1)), new(r.NextSingle()*50*r.Next(-10,10), r.NextSingle()*10*r.Next(-10,20))));
        }
        foreach(var e in ps.emmiters){
            e.velocity.X = (float)(100*Math.Cos((delta+Math.Tau*Math.PI)/Math.Tau*2));
            e.velocity.Y = (float)(100*Math.Cos(delta/Math.Tau*3));
        }
        ps.Emmit();
        ps.Update();
    }
    public void Draw(float delta) {
        ps.Draw();
        Raylib.DrawText("p: "+ps.particles_active_count().ToString() + " - c: "+ps.particles.Capacity, 0,0,50, Color.White);
        Raylib.DrawText("fps: "+Raylib.GetFPS().ToString(), 0,50,50, Color.White);
        Raylib.DrawText($"emmiters: {ps.emmiters.Count}", 0, 100, 25, Color.White);
    }

    public void UnloadContent() {
        ps.emmiters.Clear();
        ps.particles.Clear();
    }

    public ParticleEmmition test_create_bunch_of_stuff(Rectangle pos, Vector2 velocity){
        Random r = new();
        ParticleEmmition emmiter = new(
            particleListLenght: 100, 
            hsvVariation: new(100, 1f, 0.5f)
        );
        emmiter.particleListLenght = 100;
        emmiter.EmmiterPos = pos;
        emmiter.size = new(5);
        emmiter.sizeVariation = new(0);
        emmiter.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter.velocity = velocity;
        emmiter.gravity = new Vector2(0,-50);
        emmiter.lifeTime = 1f;
        emmiter.lifeTimeVariation = 6;
        return emmiter;
    }
}
