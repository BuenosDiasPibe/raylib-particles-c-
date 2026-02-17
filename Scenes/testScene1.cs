using System.Numerics;
using Raylib_cs;
namespace raylib_particles;

public class testScene1 : IScene
{
    ParticleSystem ps = new(10000);


    public void LoadContent() {
        ps.CreateEmmitor(test_create_bunch_of_stuff(new(WindowProps.WIDTH/4, WindowProps.HEIGHT/2, 1, 1)));
        WindowProps.bgClear = new(28,28,28,255);
    }

    public void Update(float delta) {
        if(Raylib.IsMouseButtonPressed(MouseButton.Left)) {
            Vector2 p = Raylib.GetMousePosition();
            ps.CreateEmmitor(test_create_bunch_of_stuff(new(p, new(1))));
        }
        ps.Emmit();

        ps.Update();
    }
    public void Draw(float delta) {
            ps.Draw();
            Raylib.DrawText("p: "+ps.particles_active_count().ToString(), 0,0,50, Color.White);
            Raylib.DrawText("fps: "+Raylib.GetFPS().ToString(), 0,50,50, Color.White);
    }

    public void UnloadContent() {
        ps.emmiters.Clear();
        ps.particles.Clear();
    }

    public ParticleEmmition test_create_bunch_of_stuff(Rectangle pos){
        Random r = new();
        ParticleEmmition emmiter = new(
            particleListLenght: 100, 
            hsvVariation: new(100, 1f, 0.5f)
        );
        emmiter.particleListLenght = 200;
        emmiter.EmmiterPos = pos;
        emmiter.size = new(5);
        emmiter.sizeVariation = new(0);
        emmiter.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter.velocity = new Vector2(300, -200);
        emmiter.velocityVariation = new Vector2(-100, 100);
        emmiter.lifeTime = 1f;
        emmiter.lifeTimeVariation = 6;
        return emmiter;
    }
}
