#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <cctype>
#include <vector>
#include <string>
#include <algorithm>
#include <cwchar>

// ---- 8x8 monospace bitmap font (public domain) ----

static const unsigned char FONT8x8[][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x18,0x3c,0x3c,0x18,0x18,0x00,0x18,0x00},
    {0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00},
    {0x6c,0x6c,0xfe,0x6c,0xfe,0x6c,0x6c,0x00},
    {0x18,0x3e,0x60,0x3c,0x06,0x7c,0x18,0x00},
    {0x00,0xc6,0xcc,0x18,0x30,0x66,0xc6,0x00},
    {0x38,0x6c,0x38,0x76,0xdc,0xcc,0x76,0x00},
    {0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00},
    {0x0c,0x18,0x30,0x30,0x30,0x18,0x0c,0x00},
    {0x30,0x18,0x0c,0x0c,0x0c,0x18,0x30,0x00},
    {0x00,0x66,0x3c,0xff,0x3c,0x66,0x00,0x00},
    {0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30},
    {0x00,0x00,0x00,0x7e,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00},
    {0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00},
    {0x3c,0x66,0x76,0x7e,0x6e,0x66,0x3c,0x00},
    {0x18,0x38,0x18,0x18,0x18,0x18,0x7e,0x00},
    {0x3c,0x66,0x06,0x0c,0x18,0x30,0x7e,0x00},
    {0x3c,0x66,0x06,0x1c,0x06,0x66,0x3c,0x00},
    {0x0c,0x1c,0x3c,0x6c,0x7e,0x0c,0x1e,0x00},
    {0x7e,0x60,0x7c,0x06,0x06,0x66,0x3c,0x00},
    {0x3c,0x66,0x60,0x7c,0x66,0x66,0x3c,0x00},
    {0x7e,0x06,0x0c,0x18,0x30,0x30,0x30,0x00},
    {0x3c,0x66,0x66,0x3c,0x66,0x66,0x3c,0x00},
    {0x3c,0x66,0x66,0x3e,0x06,0x66,0x3c,0x00},
    {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00},
    {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30},
    {0x06,0x0c,0x18,0x30,0x18,0x0c,0x06,0x00},
    {0x00,0x00,0x7e,0x00,0x7e,0x00,0x00,0x00},
    {0x60,0x30,0x18,0x0c,0x18,0x30,0x60,0x00},
    {0x3c,0x66,0x06,0x0c,0x18,0x00,0x18,0x00},
    {0x3c,0x66,0x6e,0x6e,0x6e,0x60,0x3e,0x00},
    {0x18,0x3c,0x66,0x66,0x7e,0x66,0x66,0x00},
    {0x7c,0x66,0x66,0x7c,0x66,0x66,0x7c,0x00},
    {0x3c,0x66,0x60,0x60,0x60,0x66,0x3c,0x00},
    {0x78,0x6c,0x66,0x66,0x66,0x6c,0x78,0x00},
    {0x7e,0x60,0x60,0x7c,0x60,0x60,0x7e,0x00},
    {0x7e,0x60,0x60,0x7c,0x60,0x60,0x60,0x00},
    {0x3c,0x66,0x60,0x6e,0x66,0x66,0x3c,0x00},
    {0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x00},
    {0x7e,0x18,0x18,0x18,0x18,0x18,0x7e,0x00},
    {0x3e,0x0c,0x0c,0x0c,0x0c,0x6c,0x38,0x00},
    {0x66,0x6c,0x78,0x70,0x78,0x6c,0x66,0x00},
    {0x60,0x60,0x60,0x60,0x60,0x60,0x7e,0x00},
    {0xc6,0xee,0xfe,0xfe,0xd6,0xc6,0xc6,0x00},
    {0x66,0x76,0x7e,0x7e,0x6e,0x66,0x66,0x00},
    {0x3c,0x66,0x66,0x66,0x66,0x66,0x3c,0x00},
    {0x7c,0x66,0x66,0x7c,0x60,0x60,0x60,0x00},
    {0x3c,0x66,0x66,0x66,0x6e,0x3c,0x07,0x00},
    {0x7c,0x66,0x66,0x7c,0x78,0x6c,0x66,0x00},
    {0x3c,0x66,0x60,0x3c,0x06,0x66,0x3c,0x00},
    {0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
    {0x66,0x66,0x66,0x66,0x66,0x66,0x3c,0x00},
    {0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00},
    {0xc6,0xc6,0xc6,0xd6,0xfe,0xee,0xc6,0x00},
    {0x66,0x66,0x3c,0x18,0x3c,0x66,0x66,0x00},
    {0x66,0x66,0x66,0x3c,0x18,0x18,0x18,0x00},
    {0x7e,0x06,0x0c,0x18,0x30,0x60,0x7e,0x00},
    {0x3c,0x30,0x30,0x30,0x30,0x30,0x3c,0x00},
    {0xc0,0x60,0x30,0x18,0x0c,0x06,0x02,0x00},
    {0x3c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3c,0x00},
    {0x18,0x3c,0x66,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff},
    {0x30,0x18,0x0c,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x3c,0x06,0x3e,0x66,0x3e,0x00},
    {0x60,0x60,0x7c,0x66,0x66,0x66,0x7c,0x00},
    {0x00,0x00,0x3c,0x66,0x60,0x66,0x3c,0x00},
    {0x06,0x06,0x3e,0x66,0x66,0x66,0x3e,0x00},
    {0x00,0x00,0x3c,0x66,0x7e,0x60,0x3c,0x00},
    {0x1c,0x36,0x30,0x78,0x30,0x30,0x78,0x00},
    {0x00,0x00,0x3e,0x66,0x66,0x3e,0x06,0x7c},
    {0x60,0x60,0x7c,0x66,0x66,0x66,0x66,0x00},
    {0x18,0x00,0x38,0x18,0x18,0x18,0x3c,0x00},
    {0x0c,0x00,0x1c,0x0c,0x0c,0x0c,0x6c,0x38},
    {0x60,0x60,0x66,0x6c,0x78,0x6c,0x66,0x00},
    {0x38,0x18,0x18,0x18,0x18,0x18,0x3c,0x00},
    {0x00,0x00,0xec,0xfe,0xd6,0xc6,0xc6,0x00},
    {0x00,0x00,0x7c,0x66,0x66,0x66,0x66,0x00},
    {0x00,0x00,0x3c,0x66,0x66,0x66,0x3c,0x00},
    {0x00,0x00,0x7c,0x66,0x66,0x7c,0x60,0x60},
    {0x00,0x00,0x3e,0x66,0x66,0x3e,0x06,0x06},
    {0x00,0x00,0x7c,0x66,0x60,0x60,0x60,0x00},
    {0x00,0x00,0x3e,0x60,0x3c,0x06,0x7c,0x00},
    {0x30,0x30,0x7e,0x30,0x30,0x36,0x1c,0x00},
    {0x00,0x00,0x66,0x66,0x66,0x66,0x3e,0x00},
    {0x00,0x00,0x66,0x66,0x66,0x3c,0x18,0x00},
    {0x00,0x00,0xc6,0xc6,0xd6,0xfe,0x6c,0x00},
    {0x00,0x00,0x66,0x3c,0x18,0x3c,0x66,0x00},
    {0x00,0x00,0x66,0x66,0x66,0x3e,0x06,0x7c},
    {0x00,0x00,0x7e,0x0c,0x18,0x30,0x7e,0x00},
    {0x0e,0x18,0x18,0x70,0x18,0x18,0x0e,0x00},
    {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18},
    {0x70,0x18,0x18,0x0e,0x18,0x18,0x70,0x00},
    {0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x10,0x28,0x44,0x82,0x00,0x00,0x00},
};

static const int FONT_CHARS = sizeof(FONT8x8) / sizeof(FONT8x8[0]);
static const int FONT_FIRST = 32;
static const int FONT_LAST = FONT_FIRST + FONT_CHARS - 1;

// ---- Truss data structures ----

struct Point {
    int x, y;
};

struct TrussPiece {
    std::string type;
    std::string label;
    std::vector<Point> negative;
    std::vector<Point> positive;
};

struct TrussData {
    double span = 0;
    double height = 0;
    double leftHeelHeight = 0;
    double rightHeelHeight = 0;
    std::vector<TrussPiece> pieces;
};

// ---- Minimal JSON parser (subset needed for truss files) ----

static const char *skipWS(const char *p) {
    while (*p && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) p++;
    return p;
}

static const char *parseString(const char *p, std::string &out) {
    p = skipWS(p);
    if (*p != '"') return p;
    p++;
    out.clear();
    while (*p && *p != '"') {
        if (*p == '\\') { p++; if (*p) { out += *p; p++; } }
        else { out += *p; p++; }
    }
    if (*p == '"') p++;
    return p;
}

static const char *parseNumber(const char *p, double &out) {
    p = skipWS(p);
    char *end = nullptr;
    out = strtod(p, &end);
    if (end && end != p) p = end;
    return p;
}

static const char *parseValue(const char *p, TrussData &data, const std::string &contextKey);
static const char *parseObject(const char *p, TrussData &data);
static const char *parsePieceArray(const char *p, std::vector<TrussPiece> &pieces);
static const char *parsePieceObject(const char *p, TrussPiece &piece);

static const char *parseObject(const char *p, TrussData &data) {
    p = skipWS(p);
    if (*p != '{') return p;
    p++;
    while (*p && *p != '}') {
        p = skipWS(p);
        std::string key;
        p = parseString(p, key);
        p = skipWS(p);
        if (*p == ':') p++;
        p = parseValue(p, data, key);
        p = skipWS(p);
        if (*p == ',') p++;
    }
    if (*p == '}') p++;
    return p;
}

static const char *parsePieceValue(const char *p, TrussPiece &piece, const std::string &key);
static const char *parseEndCapObject(const char *p, std::vector<Point> &neg, std::vector<Point> &pos);
static const char *parsePointArray(const char *p, std::vector<Point> &out);

static const char *parsePieceObject(const char *p, TrussPiece &piece) {
    p = skipWS(p);
    if (*p != '{') return p;
    p++;
    while (*p && *p != '}') {
        p = skipWS(p);
        std::string key;
        p = parseString(p, key);
        p = skipWS(p);
        if (*p == ':') p++;
        p = parsePieceValue(p, piece, key);
        p = skipWS(p);
        if (*p == ',') p++;
    }
    if (*p == '}') p++;
    return p;
}

static const char *parsePieceValue(const char *p, TrussPiece &piece, const std::string &key) {
    p = skipWS(p);
    if (key == "Type") {
        std::string val;
        p = parseString(p, val);
        piece.type = val;
    } else if (key == "EngineeringLabel") {
        std::string val;
        p = parseString(p, val);
        piece.label = val;
    } else if (key == "EndCap") {
        std::vector<Point> neg, pos;
        p = parseEndCapObject(p, neg, pos);
        piece.negative = neg;
        piece.positive = pos;
    } else if (*p == '{' || *p == '[') {
        int depth = 0;
        do {
            if (*p == '{' || *p == '[') depth++;
            if (*p == '}' || *p == ']') depth--;
            p++;
        } while (depth > 0 && *p);
    } else if (*p == '"') {
        std::string tmp;
        p = parseString(p, tmp);
    } else if (*p == 't' || *p == 'f' || *p == 'n') {
        while (*p && (isalpha((unsigned char)*p) || *p == '_')) p++;
    } else {
        double tmp;
        p = parseNumber(p, tmp);
    }
    return p;
}

static const char *parseEndCapObject(const char *p, std::vector<Point> &neg, std::vector<Point> &pos) {
    p = skipWS(p);
    if (*p != '{') return p;
    p++;
    while (*p && *p != '}') {
        p = skipWS(p);
        std::string key;
        p = parseString(p, key);
        p = skipWS(p);
        if (*p == ':') p++;
        p = skipWS(p);
        if (key == "Negative") {
            p = parsePointArray(p, neg);
        } else if (key == "Positive") {
            p = parsePointArray(p, pos);
        } else {
            int depth = 0;
            do {
                if (*p == '{' || *p == '[') depth++;
                if (*p == '}' || *p == ']') depth--;
                p++;
            } while (depth > 0 && *p);
        }
        p = skipWS(p);
        if (*p == ',') p++;
    }
    if (*p == '}') p++;
    return p;
}

static const char *parsePointArray(const char *p, std::vector<Point> &out) {
    p = skipWS(p);
    if (*p != '[') return p;
    p++;
    while (*p && *p != ']') {
        p = skipWS(p);
        if (*p == '{') {
            p++;
            double x = 0, y = 0;
            while (*p && *p != '}') {
                p = skipWS(p);
                std::string key;
                p = parseString(p, key);
                p = skipWS(p);
                if (*p == ':') p++;
                p = skipWS(p);
                double val;
                p = parseNumber(p, val);
                if (key == "x") x = val;
                else if (key == "y") y = val;
                p = skipWS(p);
                if (*p == ',') p++;
            }
            if (*p == '}') p++;
            out.push_back({(int)std::round(x * 100), (int)std::round(y * 100)});
        }
        p = skipWS(p);
        if (*p == ',') p++;
    }
    if (*p == ']') p++;
    return p;
}

static const char *parseValue(const char *p, TrussData &data, const std::string &key) {
    p = skipWS(p);
    if (key == "Span" || key == "OverallTrussHeight" || key == "LeftHeelHeight" || key == "RightHeelHeight") {
        double val = 0;
        p = parseNumber(p, val);
        if (key == "Span") data.span = val;
        else if (key == "OverallTrussHeight") data.height = val;
        else if (key == "LeftHeelHeight") data.leftHeelHeight = val;
        else if (key == "RightHeelHeight") data.rightHeelHeight = val;
    } else if (key == "PieceData") {
        p = skipWS(p);
        if (*p == '[') {
            p++;
            while (*p && *p != ']') {
                TrussPiece piece;
                p = parsePieceObject(p, piece);
                data.pieces.push_back(piece);
                p = skipWS(p);
                if (*p == ',') p++;
            }
            if (*p == ']') p++;
        }
    } else if (*p == '{' || *p == '[') {
        int depth = 1;
        p++;
        while (depth > 0 && *p) {
            if (*p == '{' || *p == '[') depth++;
            if (*p == '}' || *p == ']') depth--;
            p++;
        }
    } else if (*p == '"') {
        std::string tmp;
        p = parseString(p, tmp);
    } else if (*p == 't' || *p == 'f' || *p == 'n') {
        while (*p && (isalpha((unsigned char)*p) || *p == '_')) p++;
    } else {
        double tmp;
        p = parseNumber(p, tmp);
    }
    return p;
}

static bool loadTrussData(const char *path, TrussData &data) {
    FILE *f = nullptr;
    if (fopen_s(&f, path, "rb") != 0 || !f) {
        fprintf(stderr, "ERROR: Cannot open %s\n", path);
        return false;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::vector<char> buf((size_t)len + 1);
    fread(buf.data(), 1, (size_t)len, f);
    fclose(f);
    buf[len] = 0;

    const char *p = parseObject(buf.data(), data);
    return true;
}

// ---- Pixel canvas with drawing primitives ----

class Canvas {
    int w, h;
    std::vector<uint32_t> pixels; // BGRA format

public:
    Canvas(int width, int height)
        : w(width), h(height), pixels((size_t)width * height, 0) {}

    int width() const { return w; }
    int height() const { return h; }
    void *bits() { return pixels.data(); }

    void clear(uint8_t r, uint8_t g, uint8_t b) {
        uint32_t color = packColor(r, g, b, 255);
        std::fill(pixels.begin(), pixels.end(), color);
    }

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        if (x < 0 || x >= w || y < 0 || y >= h) return;
        size_t idx = (size_t)y * w + x;
        uint8_t *dp = (uint8_t *)&pixels[idx];
        if (a == 255) {
            dp[0] = b; dp[1] = g; dp[2] = r; dp[3] = 255;
        } else {
            dp[0] = (uint8_t)(((uint32_t)dp[0] * (255 - a) + (uint32_t)b * a) / 255);
            dp[1] = (uint8_t)(((uint32_t)dp[1] * (255 - a) + (uint32_t)g * a) / 255);
            dp[2] = (uint8_t)(((uint32_t)dp[2] * (255 - a) + (uint32_t)r * a) / 255);
        }
    }

    void drawLine(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;
        while (true) {
            setPixel(x0, y0, r, g, b, a);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }

    void fillPolygon(const std::vector<Point> &pts, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 200) {
        if (pts.size() < 3) return;

        int yMin = INT_MAX, yMax = INT_MIN;
        for (auto &p : pts) {
            if (p.y < yMin) yMin = p.y;
            if (p.y > yMax) yMax = p.y;
        }
        if (yMin < 0) yMin = 0;
        if (yMax >= h) yMax = h - 1;
        if (yMin > yMax) return;

        std::vector<std::vector<int>> edges(yMax - yMin + 1);
        size_t n = pts.size();

        for (size_t i = 0; i < n; i++) {
            size_t j = (i + 1) % n;
            int x1 = pts[i].x, y1 = pts[i].y;
            int x2 = pts[j].x, y2 = pts[j].y;

            if (y1 == y2) continue;
            if (y1 > y2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            double invSlope = (double)(x2 - x1) / (double)(y2 - y1);
            double x = (double)x1;

            for (int y = y1; y < y2; y++) {
                if (y >= yMin && y <= yMax) {
                    edges[y - yMin].push_back((int)std::round(x));
                }
                x += invSlope;
            }
        }

        for (int yo = 0; yo <= yMax - yMin; yo++) {
            auto &xs = edges[yo];
            if (xs.empty()) continue;
            std::sort(xs.begin(), xs.end());
            for (size_t k = 0; k + 1 < xs.size(); k += 2) {
                int xStart = xs[k];
                int xEnd = xs[k + 1];
                int y = yMin + yo;
                for (int x = xStart; x <= xEnd; x++)
                    setPixel(x, y, r, g, b, a);
            }
        }
    }

    void strokePolygon(const std::vector<Point> &pts, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        if (pts.size() < 2) return;
        for (size_t i = 0; i < pts.size(); i++) {
            size_t j = (i + 1) % pts.size();
            drawLine(pts[i].x, pts[i].y, pts[j].x, pts[j].y, r, g, b, a);
        }
    }

    void drawChar(int x, int y, unsigned char ch, uint8_t r, uint8_t g, uint8_t b) {
        if (ch < (unsigned char)FONT_FIRST || ch > (unsigned char)FONT_LAST) return;
        const unsigned char *glyph = FONT8x8[ch - FONT_FIRST];
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (glyph[row] & (0x80 >> col))
                    setPixel(x + col, y + row, r, g, b);
            }
        }
    }

    void drawText(int x, int y, const char *text, uint8_t r, uint8_t g, uint8_t b) {
        while (*text) {
            drawChar(x, y, (unsigned char)*text, r, g, b);
            x += 8;
            text++;
        }
    }

    int textWidth(const char *text) {
        return (int)strlen(text) * 8;
    }

    bool savePNG(const wchar_t *path) {
        HDC hdc = GetDC(NULL);
        BITMAPINFO bmi = {0};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void *bits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
        ReleaseDC(NULL, hdc);

        if (!hBitmap || !bits) return false;
        memcpy(bits, pixels.data(), (size_t)w * h * 4);

        Gdiplus::Bitmap gdiBmp(hBitmap, NULL);
        CLSID clsid = {0};
        UINT num = 0, size = 0;
        Gdiplus::GetImageEncodersSize(&num, &size);
        if (size > 0) {
            std::vector<BYTE> buf(size);
            Gdiplus::ImageCodecInfo *encoders = (Gdiplus::ImageCodecInfo *)buf.data();
            Gdiplus::GetImageEncoders(num, size, encoders);
            for (UINT i = 0; i < num; i++) {
                if (wcscmp(encoders[i].MimeType, L"image/png") == 0) {
                    clsid = encoders[i].Clsid;
                    break;
                }
            }
        }

        Gdiplus::Status status = gdiBmp.Save(path, &clsid, NULL);
        DeleteObject(hBitmap);
        return status == Gdiplus::Ok;
    }

private:
    static uint32_t packColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    }
};

// ---- Truss rendering ----

static std::string formatInches(double v) {
    if (v < 0) v = -v;
    double whole;
    double frac = modf(v, &whole);
    int sixteenths = (int)round(frac * 16.0);
    if (sixteenths == 16) { whole += 1.0; sixteenths = 0; }

    char buf[32];
    if (sixteenths == 0) {
        sprintf_s(buf, "%.0f\"", whole);
    } else {
        int num = sixteenths, den = 16;
        while (den % 2 == 0 && num % 2 == 0) { num /= 2; den /= 2; }
        if (whole == 0)
            sprintf_s(buf, "%d/%d\"", num, den);
        else
            sprintf_s(buf, "%.0f-%d/%d\"", whole, num, den);
    }
    return std::string(buf);
}

static void computeTrussBounds(const TrussData &data, double &xMin, double &xMax, double &yMin, double &yMax) {
    xMin = 1e9; xMax = -1e9; yMin = 1e9; yMax = -1e9;
    for (auto &p : data.pieces) {
        for (auto &pt : p.negative) {
            if (pt.x < xMin) xMin = pt.x;
            if (pt.x > xMax) xMax = pt.x;
            if (pt.y < yMin) yMin = pt.y;
            if (pt.y > yMax) yMax = pt.y;
        }
        for (auto &pt : p.positive) {
            if (pt.x < xMin) xMin = pt.x;
            if (pt.x > xMax) xMax = pt.x;
            if (pt.y < yMin) yMin = pt.y;
            if (pt.y > yMax) yMax = pt.y;
        }
    }
}

struct ImageSize { int w, h; };

static ImageSize computeImageSize(double xMin, double xMax, double yMin, double yMax) {
    double dataW = xMax - xMin;
    double dataH = yMax - yMin;
    if (dataW < 1) dataW = 1;
    if (dataH < 1) dataH = 1;

    const double ppi = 13.0;
    const double leftM = 85, rightM = 70, topM = 35, bottomM = 85;
    const int minW = 600, minH = 200, maxW = 2500, maxH = 1500;

    double w = (dataW / 100.0) * ppi + leftM + rightM;
    double h = (dataH / 100.0) * ppi + topM + bottomM;

    if (w > maxW || h > maxH) {
        double s = fmin(maxW / w, maxH / h);
        w *= s;
        h *= s;
    }

    int wi = (int)round(fmax(w, (double)minW));
    int hi = (int)round(fmax(h, (double)minH));

    return {wi, hi};
}

static void drawTruss(Canvas &c, const TrussData &data) {
    double xMin = 1e9, xMax = -1e9, yMin = 1e9, yMax = -1e9;

    for (auto &p : data.pieces) {
        for (auto &pt : p.negative) {
            if (pt.x < xMin) xMin = pt.x;
            if (pt.x > xMax) xMax = pt.x;
            if (pt.y < yMin) yMin = pt.y;
            if (pt.y > yMax) yMax = pt.y;
        }
        for (auto &pt : p.positive) {
            if (pt.x < xMin) xMin = pt.x;
            if (pt.x > xMax) xMax = pt.x;
            if (pt.y < yMin) yMin = pt.y;
            if (pt.y > yMax) yMax = pt.y;
        }
    }

    int cw = c.width(), ch = c.height();

    double topM = 35, bottomM = 85, leftM = 85, rightM = 70;
    double availW = cw - leftM - rightM;
    double availH = ch - topM - bottomM;
    double dataW = xMax - xMin;
    double dataH = yMax - yMin;
    if (dataW < 1) dataW = 1;
    if (dataH < 1) dataH = 1;

    double scale = fmin(availW / dataW, availH / dataH);

    auto tx = [&](double x) -> int { return (int)round(leftM + (x - xMin) * scale); };
    auto ty = [&](double y) -> int { return (int)round(topM + (yMax - y) * scale); };

    c.clear(30, 30, 30);

    for (auto &p : data.pieces) {
        std::vector<Point> polygon;
        for (auto &pt : p.negative)
            polygon.push_back({tx(pt.x), ty(pt.y)});
        for (auto &pt : p.positive)
            polygon.push_back({tx(pt.x), ty(pt.y)});

        uint8_t fr, fg, fb;
        if (p.type == "TopChord") { fr = 191; fg = 146; fb = 89; }
        else if (p.type == "BottomChord") { fr = 89; fg = 146; fb = 191; }
        else { fr = 146; fg = 191; fb = 89; }

        c.fillPolygon(polygon, fr, fg, fb, 200);
        c.strokePolygon(polygon, 255, 255, 255, 255);
    }

    // Grid lines
    auto drawGridLine = [&](double x1, double y1, double x2, double y2) {
        c.drawLine(tx(x1), ty(y1), tx(x2), ty(y2), 60, 60, 60);
    };

    for (double y = ceil(yMin / 100.0) * 100; y <= floor(yMax / 100.0) * 100 + 0.01; y += 500.0)
        drawGridLine(xMin, y, xMax, y);
    for (double x = ceil(xMin / 100.0) * 100; x <= floor(xMax / 100.0) * 100 + 0.01; x += 1000.0)
        drawGridLine(x, yMin, x, yMax);

    // ---- Dimension lines (architectural style) ----
    auto drawHorizDim = [&](double x1d, double x2d, double yAnchor, double offsetY, const std::string &label) {
        int sx1 = tx(x1d), sx2 = tx(x2d);
        int syAnchor = ty(yAnchor);
        int sy = syAnchor + (int)offsetY;
        int gap = 3, over = 5, tickD = 5, textGap = 2;

        c.drawLine(sx1, syAnchor + gap, sx1, sy + over, 140, 140, 140);
        c.drawLine(sx2, syAnchor + gap, sx2, sy + over, 140, 140, 140);
        c.drawLine(sx1, sy, sx2, sy, 180, 180, 180);
        c.drawLine(sx1 - tickD, sy - tickD, sx1 + tickD, sy + tickD, 180, 180, 180);
        c.drawLine(sx2 - tickD, sy + tickD, sx2 + tickD, sy - tickD, 180, 180, 180);

        int tw = c.textWidth(label.c_str());
        int lx = (sx1 + sx2 - tw) / 2;
        c.drawText(lx, sy + textGap, label.c_str(), 255, 255, 255);
    };

    auto drawVertDim = [&](double y1d, double y2d, double xAnchor, double offsetX, const std::string &label, bool leftSide) {
        int sy1 = ty(y1d), sy2 = ty(y2d);
        int sxAnchor = tx(xAnchor);
        int sx = sxAnchor + (int)offsetX;
        int gap = 3, over = 5, tickD = 5;

        c.drawLine(sxAnchor + gap, sy1, sx + over, sy1, 140, 140, 140);
        c.drawLine(sxAnchor + gap, sy2, sx + over, sy2, 140, 140, 140);
        c.drawLine(sx, sy1, sx, sy2, 180, 180, 180);
        c.drawLine(sx - tickD, sy1 - tickD, sx + tickD, sy1 + tickD, 180, 180, 180);
        c.drawLine(sx - tickD, sy2 - tickD, sx + tickD, sy2 + tickD, 180, 180, 180);

        int tw = c.textWidth(label.c_str());
        int midY = (sy1 + sy2) / 2;
        int lx = leftSide ? (sx - tw - 6) : (sx + 6);
        c.drawText(lx, midY, label.c_str(), 255, 255, 255);
    };

    double bottomEdge = yMin;
    double topEdge = yMax;
    double leftEdgeD = 0.0;
    double rightEdgeD = data.span * 100.0;

    drawHorizDim(leftEdgeD, rightEdgeD, bottomEdge, 25.0, formatInches(data.span));
    drawVertDim(bottomEdge, topEdge, leftEdgeD, -25.0, formatInches(data.height), true);

    double leftHeelBottom = topEdge - data.leftHeelHeight * 100.0;
    drawVertDim(leftHeelBottom, topEdge, leftEdgeD, -47.0, formatInches(data.leftHeelHeight), true);

    double rightHeelBottom = topEdge - data.rightHeelHeight * 100.0;
    drawVertDim(rightHeelBottom, topEdge, rightEdgeD, 25.0, formatInches(data.rightHeelHeight), false);

    const char *legend[] = {"TopChord", "BottomChord", "Web"};
    int lx = 10, ly = 10;
    for (int i = 0; i < 3; i++) {
        uint8_t lr, lg, lb;
        if (i == 0) { lr = 191; lg = 146; lb = 89; }
        else if (i == 1) { lr = 89; lg = 146; lb = 191; }
        else { lr = 146; lg = 191; lb = 89; }

        for (int dy = 0; dy < 10; dy++)
            for (int dx = 0; dx < 14; dx++)
                c.setPixel(lx + dx, ly + dy, lr, lg, lb, 200);
        c.drawText(lx + 18, ly, legend[i], 255, 255, 255);
        ly += 16;
    }

    // Credits at bottom center
    std::string credits = "TrussGen 2.0 (c) 2026";
    int cx = (cw - c.textWidth(credits.c_str())) / 2;
    c.drawText(cx, ch - 12, credits.c_str(), 100, 100, 100);
}

// ---- SVG output ----

static std::string colorStr(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    char buf[32];
    if (a < 255)
        sprintf_s(buf, "rgba(%u,%u,%u,%.2f)", r, g, b, a / 255.0);
    else
        sprintf_s(buf, "#%02x%02x%02x", r, g, b);
    return buf;
}

static bool saveSVG(const char *path, const TrussData &data) {
    double xMin = 1e9, xMax = -1e9, yMin = 1e9, yMax = -1e9;
    for (auto &p : data.pieces) {
        for (auto &pt : p.negative) {
            if (pt.x < xMin) xMin = pt.x;
            if (pt.x > xMax) xMax = pt.x;
            if (pt.y < yMin) yMin = pt.y;
            if (pt.y > yMax) yMax = pt.y;
        }
        for (auto &pt : p.positive) {
            if (pt.x < xMin) xMin = pt.x;
            if (pt.x > xMax) xMax = pt.x;
            if (pt.y < yMin) yMin = pt.y;
            if (pt.y > yMax) yMax = pt.y;
        }
    }

    ImageSize img = computeImageSize(xMin, xMax, yMin, yMax);
    int cw = img.w, ch = img.h;
    double topM = 35, bottomM = 85, leftM = 85, rightM = 70;
    double availW = cw - leftM - rightM;
    double availH = ch - topM - bottomM;
    double dataW = xMax - xMin;
    double dataH = yMax - yMin;
    if (dataW < 1) dataW = 1;
    if (dataH < 1) dataH = 1;
    double scale = fmin(availW / dataW, availH / dataH);

    auto tx = [&](double x) -> double { return leftM + (x - xMin) * scale; };
    auto ty = [&](double y) -> double { return topM + (yMax - y) * scale; };

    FILE *f = nullptr;
    if (fopen_s(&f, path, "wb") != 0 || !f) return false;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 %d %d\">\n", cw, ch);
    fprintf(f, "<rect width=\"%d\" height=\"%d\" fill=\"#1e1e1e\"/>\n", cw, ch);

    // Pieces
    for (auto &p : data.pieces) {
        fprintf(f, "<polygon points=\"");
        for (auto &pt : p.negative)
            fprintf(f, "%.1f,%.1f ", tx(pt.x), ty(pt.y));
        for (auto &pt : p.positive)
            fprintf(f, "%.1f,%.1f ", tx(pt.x), ty(pt.y));

        uint8_t r, g, b;
        if (p.type == "TopChord") { r = 191; g = 146; b = 89; }
        else if (p.type == "BottomChord") { r = 89; g = 146; b = 191; }
        else { r = 146; g = 191; b = 89; }
        fprintf(f, "\" fill=\"%s\" stroke=\"white\" stroke-width=\"1\"/>\n", colorStr(r, g, b, 200).c_str());
    }

    // Grid lines
    auto gr = [](double x) -> double { return round(x / 100.0) * 100.0; };
    for (double y = ceil(yMin / 100.0) * 100; y <= floor(yMax / 100.0) * 100 + 0.01; y += 500.0)
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#3c3c3c\" stroke-width=\"0.5\" stroke-dasharray=\"4,4\"/>\n", tx(xMin), ty(y), tx(xMax), ty(y));
    for (double x = ceil(xMin / 100.0) * 100; x <= floor(xMax / 100.0) * 100 + 0.01; x += 1000.0)
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#3c3c3c\" stroke-width=\"0.5\" stroke-dasharray=\"4,4\"/>\n", tx(x), ty(yMin), tx(x), ty(yMax));

    // Dimension lines
    auto dimSvg = [&](double x1d, double x2d, double yAnchor, double offsetY, const std::string &label) {
        double sx1 = tx(x1d), sx2 = tx(x2d);
        double syAnchor = ty(yAnchor);
        double sy = syAnchor + offsetY;
        double gap = 3, over = 5, tickD = 5;

        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#8c8c8c\" stroke-width=\"0.5\"/>\n", sx1, syAnchor + gap, sx1, sy + over);
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#8c8c8c\" stroke-width=\"0.5\"/>\n", sx2, syAnchor + gap, sx2, sy + over);
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#b4b4b4\" stroke-width=\"0.8\"/>\n", sx1, sy, sx2, sy);
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#b4b4b4\" stroke-width=\"0.8\"/>\n", sx1 - tickD, sy - tickD, sx1 + tickD, sy + tickD);
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#b4b4b4\" stroke-width=\"0.8\"/>\n", sx2 - tickD, sy + tickD, sx2 + tickD, sy - tickD);
        fprintf(f, "<text x=\"%.1f\" y=\"%.1f\" fill=\"white\" font-family=\"monospace\" font-size=\"7\" text-anchor=\"middle\">%s</text>\n", (sx1 + sx2) / 2, sy + 10, label.c_str());
    };

    auto dimVSvg = [&](double y1d, double y2d, double xAnchor, double offsetX, const std::string &label, bool leftSide) {
        double sy1 = ty(y1d), sy2 = ty(y2d);
        double sxAnchor = tx(xAnchor);
        double sx = sxAnchor + offsetX;
        double gap = 3, over = 5, tickD = 5;

        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#8c8c8c\" stroke-width=\"0.5\"/>\n", sxAnchor + gap, sy1, sx + over, sy1);
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#8c8c8c\" stroke-width=\"0.5\"/>\n", sxAnchor + gap, sy2, sx + over, sy2);
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#b4b4b4\" stroke-width=\"0.8\"/>\n", sx, sy1, sx, sy2);
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#b4b4b4\" stroke-width=\"0.8\"/>\n", sx - tickD, sy1 - tickD, sx + tickD, sy1 + tickD);
        fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#b4b4b4\" stroke-width=\"0.8\"/>\n", sx - tickD, sy2 - tickD, sx + tickD, sy2 + tickD);

        double midY = (sy1 + sy2) / 2;
        double lx = leftSide ? (sx - 4) : (sx + 4);
        const char *anchor = leftSide ? "end" : "start";
        fprintf(f, "<text x=\"%.1f\" y=\"%.1f\" fill=\"white\" font-family=\"monospace\" font-size=\"7\" text-anchor=\"%s\">%s</text>\n", lx, midY + 3, anchor, label.c_str());
    };

    double bottomEdge = yMin, topEdge = yMax;
    double leftEdgeD = 0.0, rightEdgeD = data.span * 100.0;

    dimSvg(leftEdgeD, rightEdgeD, bottomEdge, 25.0, formatInches(data.span));
    dimVSvg(bottomEdge, topEdge, leftEdgeD, -25.0, formatInches(data.height), true);

    double leftHeelBottom = topEdge - data.leftHeelHeight * 100.0;
    dimVSvg(leftHeelBottom, topEdge, leftEdgeD, -47.0, formatInches(data.leftHeelHeight), true);

    double rightHeelBottom = topEdge - data.rightHeelHeight * 100.0;
    dimVSvg(rightHeelBottom, topEdge, rightEdgeD, 25.0, formatInches(data.rightHeelHeight), false);

    // Legend
    struct { const char *name; uint8_t r, g, b; } leg[] = {
        {"TopChord", 191, 146, 89}, {"BottomChord", 89, 146, 191}, {"Web", 146, 191, 89}
    };
    int ly = 10;
    for (int i = 0; i < 3; i++) {
        fprintf(f, "<rect x=\"10\" y=\"%d\" width=\"14\" height=\"10\" fill=\"%s\" opacity=\"0.78\"/>\n",
                ly, colorStr(leg[i].r, leg[i].g, leg[i].b).c_str());
        fprintf(f, "<text x=\"28\" y=\"%d\" fill=\"white\" font-family=\"monospace\" font-size=\"8\">%s</text>\n", ly + 9, leg[i].name);
        ly += 16;
    }

    // Credits at bottom center
    fprintf(f, "<text x=\"%d\" y=\"%d\" fill=\"#646464\" font-family=\"monospace\" font-size=\"7\" text-anchor=\"middle\">TrussGen 2.0 (c) 2026</text>\n", cw / 2, ch - 5);

    fprintf(f, "</svg>\n");
    fclose(f);
    return true;
}

// ---- Main ----

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: TrussGen.exe input.json [output.png|output.svg]\n");
        return 1;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
            fprintf(stdout, "TrussGen 2.0\n");
            return 0;
        }
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            fprintf(stdout,
                "TrussGen 2.0 - Floor truss PNG/SVG generator\n"
                "\n"
                "Usage: TrussGen.exe input.json [output.png|output.svg]\n"
                "\n"
                "  input.json   Path to truss JSON file\n"
                "  output.png   Output PNG file (default: input name + .png)\n"
                "  output.svg   Output SVG file\n"
                "\n"
                "Options:\n"
                "  -h, --help     Show this help\n"
                "  -v, --version  Show version\n"
            );
            return 0;
        }
    }

    const char *inputPath = argv[1];
    std::string outputPath;
    if (argc >= 3) {
        outputPath = argv[2];
    } else {
        const char *ext = strrchr(inputPath, '.');
        size_t baseLen = ext ? (size_t)(ext - inputPath) : strlen(inputPath);
        outputPath = std::string(inputPath, baseLen) + ".png";
    }

    TrussData data;
    if (!loadTrussData(inputPath, data)) return 1;

    // Detect format from output path extension
    const char *outExt = strrchr(outputPath.c_str(), '.');
    bool isSvg = outExt && (_stricmp(outExt, ".svg") == 0);

    if (isSvg) {
        if (!saveSVG(outputPath.c_str(), data)) {
            fprintf(stderr, "ERROR: Failed to save %s\n", outputPath.c_str());
            return 1;
        }
        fprintf(stdout, "%s\n", outputPath.c_str());
    } else {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        double xMin, xMax, yMin, yMax;
        computeTrussBounds(data, xMin, xMax, yMin, yMax);
        ImageSize img = computeImageSize(xMin, xMax, yMin, yMax);
        Canvas canvas(img.w, img.h);
        drawTruss(canvas, data);

        int wlen = MultiByteToWideChar(CP_UTF8, 0, outputPath.c_str(), -1, NULL, 0);
        std::vector<wchar_t> wpath(wlen);
        MultiByteToWideChar(CP_UTF8, 0, outputPath.c_str(), -1, wpath.data(), wlen);

        bool ok = canvas.savePNG(wpath.data());
        Gdiplus::GdiplusShutdown(gdiplusToken);

        if (!ok) {
            fprintf(stderr, "ERROR: Failed to save %s\n", outputPath.c_str());
            return 1;
        }
        fprintf(stdout, "%s\n", outputPath.c_str());
    }

    return 0;
}
