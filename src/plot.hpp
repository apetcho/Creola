/*
2D figure model and layout
*/
#pragma once
#include<optional>
#include<vector>
#include<string>


// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
struct Rgb {
    double r = 0;
    double g = 0;
    double b = 0;
};

struct Point {
    double x = 0;
    double y = 0;
};

struct Font {
    double size = 11;
    std::string family;
    bool italic = false;
    bool bold = false;
};

struct TextFmt {
    Rgb color;
    Font font;
};

struct Item {
    enum class Kind { Line, Point, Arrow, Poly, Text };
    Kind kind = Kind::Line;
    std::vector<Point> pts;     // -
    Rgb color;
    double width = 1.5;
    std::string dash = "-";     // "-", "--", ":". "-."
    bool filled = false;
    Rgb fill;
    double size = 4;
    std::string marker = "none";
    std::optional<Rgb> markerColor;
    bool closed = false;
    std::string label;
    std::string text;
    Font font;
    Rgb textColor;
};

// -
struct Figure {
    std::vector<Item> items;
    std::string title;
    std::string xlabel;
    std::string ylabel;
    TextFmt titleFmt{{}, {14, "", false, false }};
    TextFmt xlabelFmt{{}, {12, "", false, false }};
    TextFmt ylabelFmt{{}, {12, "", false, false }};
    TextFmt labelFmt{{}, {11, "", false, false }};
    std::optional<double> xmin;
    std::optional<double> xmax;
    std::optional<double> ymin;
    std::optional<double> ymax;
    bool gridXMajor = true;
    bool gridXMinor = false;
    bool gridYMajor = true;
    bool gridYMinor = false;
    bool legend = false;
    bool dark = false;
    int eqault = -1;
    bool shapes = false;
    int width = 640;
    int height = 480;
    int nextColor = 0;

    Rgb cycleColor(void);
};

//
Rgb parseColor(const std::string& s);

// -
class Canvas {
public:
    virtual ~Canvas() = default;
    virtual void begin(double w, double h) = 0;
    virtual void end(void) = 0;
    virtual void clip(double x, double y, double w, double h) = 0;
    virtual void unclip(void) = 0;
    virtual void stroke(const std::vector<Point>& pt, bool closed, Rgb color, double width, const std::string& dash) = 0;
    virtual void fill(const std::vector<Point>& pt, Rgb color) = 0;
    // -
    virtual void text(double x, double y, const std::string& s, const Font& font, Rgb color, double rotateDeg) = 0;
    virtual double textWidth(const std::string& s, const Font& font) const = 0;
};

// -
void render(const Figure& fig, Canvas& canvas);

// -
std::string toBytes(const Figure& fig, const std::string& format);
std::string toEps(const Figure& fig);
void save(const Figure& fig, const std::string& path);

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------