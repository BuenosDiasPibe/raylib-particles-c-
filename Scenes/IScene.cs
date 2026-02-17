namespace raylib_particles;
public interface IScene {
    public void LoadContent();
    public void Update(float delta);
    public void Draw(float delta);
    public void UnloadContent();
}
