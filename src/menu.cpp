#include "menu.h"

Menu :: Menu(){
 
}

Menu :: ~Menu(){

}

int Menu :: showMenu(SDL_Renderer* screen, TTF_Font* font){
    bool res = g_image.loadFromFile("res/m_m_main.png", screen);
    if(!res){
        cout << "Khong the load hinh anh menu\n";
        return 1;
    }
    const int number_of_items = 5;
    int selected[number_of_items] = {0, 0, 0, 0, 0};
    BaseObject base[number_of_items];
    base[0].setRect(440, 240);
    base[0].loadFromFile("res/button_start.png", screen);
    base[1].setRect(440, 310);
    base[1].loadFromFile("res/button_exit.png", screen);
    base[2].setRect(440, 380);
    base[2].loadFromFile("res/button_about.png", screen);
    base[3].setRect(449, 460);
    base[3].loadFromFile("res/m_settings.png", screen);
    base[4].setRect(435, 515);
    base[4].loadFromFile("res/options.png", screen);
    Mix_Chunk* click = NULL;
    click = Mix_LoadWAV("audio/button_click_sound.wav");
    int xm = 0, ym = 0;
    SDL_Event m_event;
    while(true){
        g_image.setRect(8, 8);
        g_image.render(screen, NULL);
        for(int i = 0; i < number_of_items; i++){
            base[i].render(screen, NULL);
        }
        while(SDL_PollEvent(&m_event) != 0){
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
                break;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;
                    for(int i = 0; i < number_of_items; i++){
                        if(COMMONF :: checkfocus(xm, ym, base[i].getRect())){
                            if(selected[i] == 0){
                                selected[i] = 1;
                            }
                        }else{
                            if(selected[i] == 1){
                                selected[i] = 0;
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                    {
                        xm = m_event.button.x;
                        ym = m_event.button.y;
                        Mix_PlayChannel(-1, click, 0);
                        for(int i = 0; i < number_of_items; i++){
                            if(COMMONF :: checkfocus(xm, ym, base[i].getRect())){
                                return i;
                            }
                        }
                    }
                break;
            case SDL_KEYDOWN:
                if(m_event.key.keysym.sym == SDLK_ESCAPE){
                    return 1;
                }
                break;
            }
        }
        SDL_RenderPresent(screen);
    }
    return 1;
}