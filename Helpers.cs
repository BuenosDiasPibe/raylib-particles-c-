using Raylib_cs;
namespace raylib_particles;

public static  class WindowProps {
    public const int WIDTH = 720;
    public const int HEIGHT = 900;
    public const string NAME = "Particles";
    public const int FPS = 75;
    public static Color bgClear = Color.Black; // this would probably not explode
}

public class HSV(float hue, float sat, float bri) {
    public float hue = hue % 360;
    public float saturation = Math.Max(sat, 1);
    public float brightness = Math.Max(bri,1);
    public Color toColor() {
        return Color.FromHSV(hue, saturation, brightness);
    }
}
public static class Helper{
    public static Color addColorHSV(Color color1, HSV color2) {
        Color colorHSV = color2.toColor();
        return Raylib.ColorAlphaBlend(colorHSV, color1, Color.White);
    }
}
