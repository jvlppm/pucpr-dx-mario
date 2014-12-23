#include "Text.h"
#include "mage/GameWindow.h"

using namespace games;
using namespace mage;
using namespace std;

struct Text::private_implementation {
    private_implementation() {
    }

    ~private_implementation() {
    }

    void draw(IDirect3DDevice9* device, shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<Text> self) {
        if (!font) {
            ID3DXFont* nFont;
            HRESULT createFontRes = D3DXCreateFont(device,
                fontSize,         //Font height
                0,                //Font width
                FW_BOLD,        //Font Weight
                1,                //MipLevels
                false,            //Italic
                DEFAULT_CHARSET,  //CharSet
                OUT_DEFAULT_PRECIS, //OutputPrecision
                ANTIALIASED_QUALITY, //Quality
                DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
                _T("Arial"),          //pFacename,
                &nFont);
            if (createFontRes == S_OK) {
                font = shared_ptr<ID3DXFont>(nFont, [](ID3DXFont* nFont) { nFont->Release(); });
                mage::GameWindow& window = mage::GameWindow::get();
                font_rect = { 0, window.getHeight() - fontSize * 2, window.getWidth(), window.getHeight() };
            }
            else
                HR(createFontRes);
        }

        if (font) {
            font->DrawTextA(nullptr, text.c_str(), -1, &font_rect, DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);
        }
    }

    int fontSize;
    string text;
    shared_ptr<ID3DXFont> font;

    RECT font_rect;
};

Text::Text(const string& text, int fontSize) : pImpl(new Text::private_implementation())
{
    pImpl->text = text;
    pImpl->fontSize = fontSize;
}

Text::~Text()
{
}

void Text::draw(IDirect3DDevice9* device, shared_ptr<Scene> scene, shared_ptr<Camera> camera)
{
    pImpl->draw(device, scene, camera, shared_from_this());
}

D3DXMATRIX Text::world()
{
    return BaseObject::world();
}

D3DXVECTOR3 Text::worldPosition()
{
    return BaseObject::worldPosition();
}
