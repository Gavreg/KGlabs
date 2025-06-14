#include "GUItextRectangle.h"

#include <GL/gl.h>
#include <windows.h>

class GuiTextRectanglePrivate
{
  public:
    HBITMAP bitmap;
    int w;
    int h;
    HDC dc;

    int pos_x;
    int pos_y;

    unsigned char* b;
    unsigned char* _tmp;

    GLuint tex_id;
    tagRECT r;

    GuiTextRectanglePrivate()
    {
        _tmp = nullptr;
    }
};

GuiTextRectangle::GuiTextRectangle()
{
    d_ptr = new GuiTextRectanglePrivate;
    d_func()->r.top = 0;
    d_func()->r.left = 0;
}

GuiTextRectangle::~GuiTextRectangle()
{
    glDeleteTextures(1, &d_func()->tex_id);
    DeleteObject(d_func()->bitmap);
    DeleteDC(d_func()->dc);
    delete d_func()->_tmp;
    delete d_ptr;
}

void GuiTextRectangle::setSize(int width, int height)
{
    GuiTextRectanglePrivate* _d = d_func();

    DeleteObject(d_func()->bitmap);

    _d->h = height;
    _d->w = width;

    _d->dc = CreateCompatibleDC(0);

    BITMAPINFOHEADER binfo;
    memset(&binfo, 0, sizeof(BITMAPINFOHEADER));

    binfo.biBitCount = 32;
    binfo.biWidth = width;
    binfo.biHeight = height;
    binfo.biSize = sizeof(binfo);
    binfo.biPlanes = 1;
    binfo.biCompression = BI_RGB;

    _d->bitmap = CreateDIBSection(0, (BITMAPINFO*)&binfo, DIB_RGB_COLORS, (void**)&(_d->b), 0, 0);
    SelectObject(_d->dc, _d->bitmap);

    if (_d->_tmp != nullptr)
        delete _d->_tmp;
    _d->_tmp = new unsigned char[_d->w * _d->h * 4];

    // Прямоугольник для текста

    glDeleteTextures(1, &(_d->tex_id));
    glGenTextures(1, &(_d->tex_id));

    glBindTexture(GL_TEXTURE_2D, _d->tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_BLEND);
}

int GuiTextRectangle::getWidth()
{
    return d_func()->h;
}

int GuiTextRectangle::getHeight()
{
    return d_func()->w;
}

void GuiTextRectangle::setPosition(int x, int y)
{
    d_func()->pos_x = x;
    d_func()->pos_y = y;
}

void GuiTextRectangle::setText(const wchar_t* text, char r, char g, char b)
{
    GuiTextRectanglePrivate* _d = d_func();
    _d->r.right = _d->w;
    _d->r.bottom = _d->h;

    std::fill<byte*, byte>((byte*)(_d->b), (byte*)(_d->b) + _d->w * _d->h * 4, (byte)255);
    // std::fill<byte*, byte>((byte*)(_d->_tmp), (byte*)(_d->_tmp) + _d->w * _d->h * 4, (byte)0);

    SetBkColor(_d->dc, RGB(255, 255, 255));
    SetTextColor(_d->dc, RGB(r, g, b));

    HFONT hFont = CreateFont(16, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Consolas");

    SelectObject(_d->dc, hFont);

    // Рисуем текст
    DrawText(_d->dc, text, -1, &(_d->r), 0);

    DeleteObject(hFont);

    glBindTexture(GL_TEXTURE_2D, _d->tex_id);

    unsigned char* _tmp = _d->_tmp;
    for (int i = 0; i < _d->h; ++i)
        for (int j = 0; j < _d->w; ++j)
        {
            *(_tmp + i * _d->w * 4 + j * 4 + 0) = *(_d->b + i * _d->w * 4 + j * 4 + 0);
            *(_tmp + i * _d->w * 4 + j * 4 + 1) = *(_d->b + i * _d->w * 4 + j * 4 + 1);
            *(_tmp + i * _d->w * 4 + j * 4 + 2) = *(_d->b + i * _d->w * 4 + j * 4 + 2);

            //*(_tmp + i * _d->w * 4 + j * 4 + 3) = 255 - *(_tmp + i * _d->w * 4 + j * 4 + 0) * 0.333 +
            //	*(_tmp + i * _d->w * 4 + j * 4 + 1) * 0.333 +
            //	*(_tmp + i * _d->w * 4 + j * 4 + 2) * 0.333;

            if (*(_d->b + i * _d->w * 4 + j * 4 + 0) == 255 && *(_d->b + i * _d->w * 4 + j * 4 + 1) == 255 &&
                *(_d->b + i * _d->w * 4 + j * 4 + 2) == 255)

                *(_tmp + i * _d->w * 4 + j * 4 + 3) = 0;
            else
                *(_tmp + i * _d->w * 4 + j * 4 + 3) = 255;
        }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _d->w, _d->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, _tmp);
}

void GuiTextRectangle::Draw()
{
    GuiTextRectanglePrivate* _d = d_func();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_LIGHTING);
    bool _b = glIsEnabled(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _d->tex_id);

    glColor4d(1, 1, 1, 1);
    glBegin(GL_QUADS);

    glTexCoord2d(0, 0);
    glVertex2d(_d->pos_x, _d->pos_y);

    glTexCoord2d(0, 1);
    glVertex2d(_d->pos_x, _d->pos_y + _d->h);

    glTexCoord2d(1, 1);
    glVertex2d(_d->pos_x + _d->w, _d->pos_y + _d->h);

    glTexCoord2d(1, 0);
    glVertex2d(_d->pos_x + _d->w, _d->pos_y);

    glEnd();

    if (!_b)
        glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
}
