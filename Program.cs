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
        emmiter.velocity = new Vector2(300, -200);
        emmiter.velocityVariation = new Vector2(-100, 100);
        emmiter.lifeTime = 1f;
        emmiter.lifeTimeVariation = 6;
        ps.CreateEmmitor(emmiter);

        ParticleEmmition emmiter2 = new(new(100, 1f, 0.5f));
        emmiter2.particleListLenght = 10;
        emmiter2.EmmiterPos = new(WindowProps.WIDTH/2, WindowProps.HEIGHT/2, 30, 30);
        emmiter2.size = new(5);
        emmiter2.sizeVariation = new(0);
        emmiter2.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter2.velocity = new Vector2(-300, -200);
        emmiter2.velocityVariation = new Vector2(100, 100);
        emmiter2.lifeTime = 6f;
        emmiter2.lifeTimeVariation = 4;
        ps.CreateEmmitor(emmiter2);

        ParticleEmmition emmiter3 = new(new(100, 1f, 0.5f));
        emmiter3.particleListLenght = 2000;
        emmiter3.EmmiterPos = new(WindowProps.WIDTH/2+50, WindowProps.HEIGHT/2, 30, 30);
        emmiter3.size = new(5);
        emmiter3.sizeVariation = new(0);
        emmiter3.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter3.velocity = new Vector2(-300, 200);
        emmiter3.velocityVariation = new Vector2(100, 100);
        emmiter3.lifeTime = 6f;
        emmiter3.lifeTimeVariation = -4;

        ps.CreateEmmitor(emmiter3);
        ParticleEmmition emmiter4 = new(new(100, 1f, 0.5f));
        emmiter4.particleListLenght = 200;
        emmiter4.EmmiterPos = new(WindowProps.WIDTH/2-10, WindowProps.HEIGHT*0.2f, 30, 30);
        emmiter4.size = new(5);
        emmiter4.sizeVariation = new(0);
        emmiter4.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter4.colorEnd = new();
        emmiter4.velocity = new Vector2(300, -200);
        emmiter4.velocityVariation = new Vector2(-50, 100);
        emmiter4.lifeTime = 6f;
        emmiter4.lifeTimeVariation = 4;
        ps.CreateEmmitor(emmiter4);

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

