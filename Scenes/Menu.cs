using System.Numerics;
using Raylib_cs;

namespace raylib_particles.Scenes;

public class Menu(SceneManager sceneManager) : IScene
{
    Rectangle btn1 = new(WindowProps.WIDTH/2-200, WindowProps.HEIGHT/2-100, 400, 100);
    Rectangle btn2 = new(WindowProps.WIDTH/2-200, WindowProps.HEIGHT/2, 400, 100);
    Color btn1Color = Color.White;
    Color btn2Color = Color.Red;

    Rectangle btn3 = new(WindowProps.WIDTH/2-200,WindowProps.HEIGHT/2+100, 400,100);
    Color btn3Color = Color.Green;
    SceneManager sceneManager = sceneManager;

    public void LoadContent() {
        WindowProps.bgClear = new Color((int)0x1D, (int)0x20, (int)0x21, 255);
    }

    public void Update(float delta) {
        Vector2 mousePos = Raylib.GetMousePosition();
        btn1Color = Color.White;
        btn2Color = Color.Red;
        btn3Color = Color.Green;

        if(Raylib.CheckCollisionPointRec(mousePos, btn1)){
            btn1Color.A = 127;
            if(Raylib.IsMouseButtonPressed(MouseButton.Left)){
                sceneManager.AddScene(new testScene1(sceneManager));
            }
        }
        if(Raylib.CheckCollisionPointRec(mousePos, btn2)){
            btn2Color.A = 127;
            if(Raylib.IsMouseButtonPressed(MouseButton.Left)){
                sceneManager.AddScene(new Test2(sceneManager));
            }
        }
        if(Raylib.CheckCollisionPointRec(mousePos,btn3)){
            btn3Color.A = 127;
            if(Raylib.IsMouseButtonPressed(MouseButton.Left)){
                sceneManager.AddScene(new test3(sceneManager));
            }
        }
    }

    public void Draw(float delta) {
        Raylib.DrawRectangleRec(btn1, btn1Color);
        Raylib.DrawRectangleRec(btn2, btn2Color);
        Raylib.DrawRectangleRec(btn3, btn3Color);
    }
    public void UnloadContent() {
    }
}
