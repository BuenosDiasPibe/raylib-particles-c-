using System.Numerics;
using Raylib_cs;

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

        while (!Raylib.WindowShouldClose())
        {
            sceneManager.GetScene().Update(Raylib.GetFrameTime());

            Raylib.BeginDrawing();
                Raylib.ClearBackground(WindowProps.bgClear);

                sceneManager.GetScene().Draw(Raylib.GetFrameTime());

            Raylib.EndDrawing();
        }
        sceneManager.GetScene().UnloadContent();
        Raylib.CloseWindow();
    }
}

