using Raylib_cs;
namespace raylib_particles;

public class WindowProps {
    public const int WIDTH = 1280;
    public const int HEIGHT = 720;
    public const string NAME = "Particles";
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
