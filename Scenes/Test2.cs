using System.Numerics;
using Raylib_cs;
using raylib_particles.Particle_SRC;
namespace raylib_particles.Scenes;

public class Test2(SceneManager manager) : IScene {
    const int MAX_PARTICLES = 350_000;
    ParticleSystem  st = new(MAX_PARTICLES);
    Random r = new();
    SceneManager sceneManager = manager;
    ParticleEmmition emmitor = new(MAX_PARTICLES);
    float time = 0;

    Color c1 = Color.White;
    Color c2 = Raylib.GetColor(0xAAFFAAFF);


    public void LoadContent() {
        WindowProps.bgClear = Raylib.GetColor(0x282828FF);
        emmitor.EmmiterPos = new(
            0,
            0,
            WindowProps.WIDTH,
            WindowProps.HEIGHT
        );
        emmitor.sizeStart = new(10);
        emmitor.sizeEnd = new();
        emmitor.sizeVariation = new(9);
        emmitor.colorEnd = Raylib.GetColor(0x282828FF);
        emmitor.lifeTime = 2f;
        emmitor.lifeTimeVariation = 0.5f;

        st.CreateEmmitor(emmitor);
        st.Emmit(10);
    }

    public void Update(float delta) {
        if(Raylib.IsKeyPressed(KeyboardKey.Enter)) {
            sceneManager.RemoveScene();
        }
        time += delta * 20;

        Vector2 hi = new(
            WindowProps.HEIGHT/2-200*(float)(Math.Sin(time/Math.PI)/Math.PI),
            WindowProps.WIDTH/2-100*(float)(Math.Cos(time/Math.PI)/Math.PI)
        );
        emmitor.velocityVariation = hi;
        emmitor.gravity = -hi;

        emmitor.colorBegin = Raylib.ColorLerp(
                c1,
                c2,
                (float)(Math.Sin(time/Math.PI))
        );
        emmitor.colorEnd = Raylib.ColorLerp(
                c1,
                c2,
                (float)(Math.Cos(time/Math.PI))
        );
        st.Emmit(10);
        st.Update(delta);
    }
    public void Draw(float delta) {
        st.Draw(delta);
        Raylib.DrawText("fps: "+Raylib.GetFPS().ToString(), 0,50,50, Color.White);
    }
    public void UnloadContent()
    {
        st.Unload();
    }

}
