#ifndef menu_h
#define menu_h
#include "textobject.h"
#include "common.h"
#include "baseobject.h"
class Menu : public TextObject, BaseObject {
    public:
    Menu();
    ~Menu();
    int showMenu(SDL_Renderer* screen, TTF_Font* font);
    private:
    BaseObject g_image;
};

#endif // !