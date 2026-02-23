using Raylib_cs;
using raylib_particles.Particle_SRC;

namespace raylib_particles.Scenes;

public class test3(SceneManager manager) : IScene {

    const int max_particles = 5000;
    ParticleSystem st = new(max_particles);
    SceneManager manager = manager;

    public void LoadContent() {
        WindowProps.bgClear = Raylib.GetColor(0x3C3836FF);

        ParticleEmmition e1 = new(max_particles);
        e1.EmmiterPos = new(WindowProps.WIDTH/2,WindowProps.HEIGHT/2, 1,1);

        e1.sizeStart = new(10);
        e1.sizeEnd = new();
        e1.sizeVariation = new(10,0);

        e1.colorBegin = Raylib.GetColor(0x98971AFF);
        e1.colorEnd = new();

        e1.velocity = new(200,100);
        e1.velocityVariation = new(400,200);
        e1.gravity = new(0,-100);

        e1.lifeTime = 1.5f;
        e1.lifeTimeVariation = 1f;
        st.CreateEmmitor(e1);
    }

    public void Update(float delta) {
        st.Emmit(1);
        st.Update(delta);
    }

    public void Draw(float delta) {
        st.Draw(delta);
        Raylib.DrawText("fps: "+Raylib.GetFPS().ToString(), 0,50,50, Color.White);
    }

    public void UnloadContent() {
        st.Unload();
    }

}
