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
        emmiter.EmmiterPos = new(WindowProps.WIDTH/2-30,WindowProps.HEIGHT/2-30, 1,1);
        emmiter.size = new(5);
        emmiter.sizeVariation = new(0);
        emmiter.colorBegin = Raylib.ColorFromHSV((float)(r.NextDouble()*360), 1, 1);
        emmiter.colorEnd = Raylib.GetColor(0xFF06009D);
        //emmiter.velocity = new Vector2(300, -200);
        //emmiter.velocityVariation = new Vector2(0);
        emmiter.lifeTime = 1f;
        emmiter.lifeTimeVariation = 1;
        ps.CreateEmmitor(emmiter);
        RenderTexture2D texture = Raylib.LoadRenderTexture(WindowProps.WIDTH, WindowProps.HEIGHT);

        string particleCounter = "";
        while (!Raylib.WindowShouldClose())
        {
            emmiter.EmmiterPos.Position = Raylib.GetMousePosition()-emmiter.EmmiterPos.Size;
            emmiter.velocity = Raylib.GetMouseDelta()*5;
            emmiter.velocityVariation = Raylib.GetMouseDelta();
            if(Raylib.IsMouseButtonDown(MouseButton.Left)) {
                ps.Emmit();
            }


            ps.Update();
            particleCounter = ps.particles_active_count().ToString();
            Raylib.BeginTextureMode(texture);
                ps.Draw();
            Raylib.EndTextureMode();

            Raylib.BeginDrawing();
                Raylib.ClearBackground(Color.Black);
                Raylib.DrawTextureRec(texture.Texture, new(0,0,WindowProps.WIDTH, -WindowProps.HEIGHT), Vector2.Zero, Color.White);

                Raylib.DrawText("p: "+particleCounter, 0,0,50, Color.White);
                Raylib.DrawText("fps: "+Raylib.GetFPS().ToString(), 0,50,50, Color.White);
            Raylib.EndDrawing();
        }
        Image image = Raylib.LoadImageFromTexture(texture.Texture);
        Raylib.ImageFlipVertical(ref image);
        Raylib.ExportImage(image, "source/test.png");
        Raylib.UnloadImage(image);

        Raylib.CloseWindow();
    }
}

