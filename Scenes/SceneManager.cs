namespace raylib_particles.Scenes;
public class SceneManager {
    private Stack<IScene> scenes = new();

    public void AddScene(IScene scene){
        scene.LoadContent();
        scenes.Push(scene);
    }

    public IScene RemoveScene(){
        scenes.Peek().UnloadContent();
        return scenes.Pop();
    }

    public IScene GetScene(){
        return scenes.Peek();
    }

    public bool hasScenes() {return scenes.Any();}

    public void RemoveAllScenes() {
        scenes.Clear();
    }
}
