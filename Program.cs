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
        ParticleSystem ps = new(10000);

        ParticleEmmition emmiter = new(new(100, 1f, 0.5f));
        emmiter.particleListLenght = 200;
        emmiter.EmmiterPos = new(WindowProps.WIDTH/4, WindowProps.HEIGHT/2, 30, 30);
        emmiter.size = new(5);
        emmiter.sizeVariation = new(0);
        emmiter.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter.colorEnd = new();
        emmiter.velocity = new Vector2(300, -200);
        emmiter.velocityVariation = new Vector2(-100, 100);
        emmiter.lifeTime = 1f;
        emmiter.lifeTimeVariation = 15;
        ps.CreateEmmitor(emmiter);

        ParticleEmmition emmiter2 = new(new(100, 1f, 0.5f));
        emmiter2.particleListLenght = 20;
        emmiter2.EmmiterPos = new(WindowProps.WIDTH/2, WindowProps.HEIGHT/2, 30, 30);
        emmiter2.size = new(5);
        emmiter2.sizeVariation = new(0);
        emmiter2.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter2.colorEnd = new();
        emmiter2.velocity = new Vector2(-300, -200);
        emmiter2.velocityVariation = new Vector2(100, 100);
        emmiter2.lifeTime = 6f;
        emmiter2.lifeTimeVariation = -4;
        ps.CreateEmmitor(emmiter2);

        string particleCounter = "";

        while (!Raylib.WindowShouldClose())
        {
            ps.Emmit();

            ps.Update();
            particleCounter = ps.particles_active_count().ToString();

            Raylib.BeginDrawing();
                Raylib.ClearBackground(Color.Black);
                ps.Draw();

                Raylib.DrawText("p: "+particleCounter, 0,0,50, Color.White);
                Raylib.DrawText("fps: "+Raylib.GetFPS().ToString(), 0,50,50, Color.White);
            Raylib.EndDrawing();
        }

        Raylib.CloseWindow();
    }
}

