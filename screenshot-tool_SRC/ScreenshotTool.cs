using Raylib_cs;
using raylib_particles.Scenes;
namespace raylib_particles;
public class ScreenshotTool {
    public RenderTexture2D target {get; private set;} = Raylib.LoadRenderTexture(WindowProps.WIDTH, WindowProps.HEIGHT);
    public Image image;
    public bool isTakingPicture = false;
    public void TakePicture(SceneManager sceneManager){
        if(isTakingPicture){
            Raylib.BeginTextureMode(target);
                sceneManager.GetScene().Draw(Raylib.GetFrameTime());
            Raylib.EndTextureMode();

            image = Raylib.LoadImageFromTexture(target.Texture);
            Raylib.ImageFlipVertical(ref image);
            Raylib.ExportImage(image, $"source/{DateTime.Now.ToFileTime()}.png");
            isTakingPicture = false;
        }
    }
}
