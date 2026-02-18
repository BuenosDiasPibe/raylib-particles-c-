using System.Numerics;
using Raylib_cs;
using raylib_particles.Scenes;
namespace raylib_particles;

internal static class Program
{

    [System.STAThread]
    public static void Main()
    {
        Raylib.InitWindow(WindowProps.WIDTH, WindowProps.HEIGHT, WindowProps.NAME);
        Raylib.SetTargetFPS(WindowProps.FPS);

        SceneManager sceneManager = new();
        sceneManager.AddScene(new testScene1());
        ScreenshotTool st = new();


        while (!Raylib.WindowShouldClose())
        {
            sceneManager.GetScene().Update(Raylib.GetFrameTime());
            if(Raylib.IsKeyPressed(KeyboardKey.F2)){
                st.isTakingPicture = true;
            }

            Raylib.BeginDrawing();
                Raylib.ClearBackground(WindowProps.bgClear);
                st.TakePicture(sceneManager);
                sceneManager.GetScene().Draw(Raylib.GetFrameTime());
            Raylib.EndDrawing();
        }
        sceneManager.GetScene().UnloadContent();
        Raylib.CloseWindow();
    }
}

