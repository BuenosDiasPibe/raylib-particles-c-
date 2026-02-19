using Raylib_cs;
using raylib_particles.Particle_SRC;
namespace raylib_particles.Scenes;

public class Test2(SceneManager manager) : IScene {
    ParticleSystem  st = new(1000);
    Random r = new();
    SceneManager sceneManager = manager;

    public void LoadContent() {
        //WindowProps.bgClear = new Color((int)0xFB, (int)0xF1, (int)0xC7, 255);
        var emmitor = new ParticleEmmition(1000, new HSV(r.NextSingle()*360, 1,1));
        emmitor.EmmiterPos = new(WindowProps.WIDTH/2, WindowProps.HEIGHT/2, 0,0);
        emmitor.sizeStart = new(10);
        emmitor.sizeEnd = new(0);
        emmitor.sizeVariation = new(5);
        emmitor.colorBegin = Color.Red;
        emmitor.colorEnd = new();
        emmitor.velocity = new(0,100);
        emmitor.velocityVariation = new(100, 50);
        emmitor.gravity = new(0,100);
        emmitor.lifeTime = 2;
        emmitor.lifeTimeVariation = 1;

        st.CreateEmmitor(emmitor);
        st.Emmit(10);
    }

    public void Update(float delta) {
        if(Raylib.IsKeyPressed(KeyboardKey.Enter)) {
            sceneManager.RemoveScene();
        }
        st.Emmit(10);
        st.Update();
    }
    public void Draw(float delta) {
        st.Draw();
        Raylib.DrawText("fps: "+Raylib.GetFPS().ToString(), 0,50,50, Color.White);
    }
    public void UnloadContent()
    {
        st.Unload();
    }

}
