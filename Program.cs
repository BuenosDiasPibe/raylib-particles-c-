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
        Random r = new();
        ParticleSystem ps = new(100);

        ParticleEmmition emmiter = new(new(100, 1f, 0.5f));
        emmiter.EmmiterPos = new(WindowProps.WIDTH/2-30,WindowProps.HEIGHT/2-30, 10,10);
        emmiter.size = new(1);
        emmiter.sizeVariation = new(5);
        emmiter.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter.colorEnd = new();
        emmiter.velocity = new(300);
        emmiter.velocityVariation = new(100);
        emmiter.lifeTime = 0.1f;
        emmiter.lifeTimeVariation = 1;
        ps.CreateEmmitor(emmiter);

        string particleCounter = "";
        while (!Raylib.WindowShouldClose())
        {
            emmiter.EmmiterPos.Position = Raylib.GetMousePosition()+emmiter.EmmiterPos.Size;
            ps.Emmit();

            ps.Update();
            particleCounter = ps.particles_active_count().ToString();

            Raylib.BeginDrawing();
                Raylib.ClearBackground(Color.Black);

                ps.Draw();

                Raylib.DrawText(particleCounter, 0,0,50, Color.White);
                Raylib.DrawText(Raylib.GetFPS().ToString(), 0,50,50, Color.White);
            Raylib.EndDrawing();
        }

        Raylib.CloseWindow();
    }
}

