using Raylib_cs;
using raylib_particles.Particle_SRC;

namespace raylib_particles.Scenes;

public class Test4(SceneManager manager) : IScene
{
    SceneManager sceneManager = manager;
    public void LoadContent() {
    }

    public void Update(float delta) {
        if(Raylib.IsKeyPressed(KeyboardKey.Enter)) {
            sceneManager.RemoveScene();
        }
    }

    public void Draw(float delta) {
    }

    public void UnloadContent() {
    }

}
