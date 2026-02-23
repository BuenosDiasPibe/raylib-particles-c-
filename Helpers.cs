using Raylib_cs;
namespace raylib_particles;

public static class WindowProps {
    public static int WIDTH  = 1000;
    public static int HEIGHT = 700;
    public static string NAME = "Particles";
    public static int FPS     = 60;
    public static Color bgClear = Color.Black; // this would probably not explode
}

public class HSV {
    public float hue = 0;
    public float saturation = 0;
    public float brightness = 0;
    public HSV(float hue, float sat, float bri) {
        this.hue = hue % 360;
        this.saturation = Math.Max(sat, 1);
        this.brightness = Math.Max(bri,1);
    }
    public Color toColor() {
        return Color.FromHSV(hue, saturation, brightness);
    }
}
public static class Helper{
    public static Color addColorHSV(Color color1, HSV color2) {
        Color colorHSV = color2.toColor();
        return Raylib.ColorAlphaBlend(colorHSV, color1, Color.White);
    }
    public static Rectangle invert {get; private set;}= new Rectangle(0,0,WindowProps.WIDTH, -WindowProps.HEIGHT);
}
