#include "common.h"
#include "baseobject.h"
#include "mainobject.h"
#include "timer.h"
#include "bulletobject.h" 
#include "threatobject.h"
#include "explosion.h"
#include "textobject.h"
#include "boss.h"
#include "geometry.h"
#include "menu.h"
SDL_Window* gwindow = NULL;
SDL_Renderer* grender = NULL;
SDL_Renderer* g_menu = NULL;
SDL_Event e;
Mix_Chunk* vuno;
Mix_Chunk* danban[3];
Mix_Chunk* danbann[3];
Mix_Chunk* start;
Mix_Chunk* music;
Mix_Chunk* button_click;
Mix_Chunk* reload;
Mix_Chunk* end_game;
Mix_Chunk* click;
BaseObject background;
Boss boss;
MainObject mainobject;
BulletObject bulletobject;
ThreatObject threatobject[7];
BaseObject about;
BaseObject heart[3];
BaseObject pause;
BaseObject resume;
BaseObject back;
BaseObject sound_on;
BaseObject music_on;
BaseObject sound_off;
BaseObject music_off;
BaseObject gameover;
TTF_Font* font_time = NULL;
BaseObject home;
BaseObject nenn;
BaseObject youwin;
bool load1 = true;
bool load2 = true;
bool init(){
    bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cout << "SDL khong the khoi tao\n" << SDL_GetError();
		success = false;
	}else{
		gwindow = SDL_CreateWindow("WAR_PLANE", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		if(gwindow == NULL){
			cout << "Khong the tao ra window\n" << SDL_GetError();
			success = false;
		}else{
			grender = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
			if(grender == NULL){
				cout << "Khong the tao ra render\n" << SDL_GetError();
				success = false;
			}else{
				SDL_SetRenderDrawColor(grender, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
        if(TTF_Init() == -1) return false;
        font_time = TTF_OpenFont("font/FreeMono.ttf", 15);
        if(font_time == NULL) return false;
        if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) return false;
        vuno = Mix_LoadWAV("audio/Explosion1.wav");
        danban[0] = Mix_LoadWAV("audio/danban.wav");
        danban[1] = Mix_LoadWAV("audio/gunshot.wav");
        danban[2] = Mix_LoadWAV("audio/shoot.wav");
        start = Mix_LoadWAV("audio/3_2_1_go.wav");
        music = Mix_LoadWAV("audio/music.wav");
        button_click = Mix_LoadWAV("audio/button_click_sound.wav");
        reload = Mix_LoadWAV("audio/nono.wav");
        end_game = Mix_LoadWAV("audio/end_screen_sound.wav");
        click = Mix_LoadWAV("audio/button_click_sound.wav");
        if(vuno == NULL || danban[0] == NULL || danban[1] == NULL || danban[2] == NULL 
        || start == NULL || music == NULL || button_click == NULL || reload == NULL 
        || end_game == NULL || click == NULL) return false;
	}
	return success;
}
bool loadmedia(){
    if(!nenn.loadFromFile("res/m_m_main.png", grender)){
        return false;
    }
    if(!home.loadFromFile("res/button_home.png", grender)){
        return false;
    }
    if(!gameover.loadFromFile("res/gameover.png", grender)){
        return false;
    }
    if(!sound_on.loadFromFile("res/button_sound_on.png", grender)){
        return false;
    }
    if(!sound_off.loadFromFile("res/button_sound_off.png", grender)){
        return false;
    }
    if(!music_on.loadFromFile("res/button_music_on.png", grender)){
        return false;
    }
    if(!music_off.loadFromFile("res/button_music_off.png", grender)){
        return false;
    }
    if(!back.loadFromFile("res/levellll.png", grender)){
        return false;
    }
    if(!resume.loadFromFile("res/button_resume.png",grender)){
        return false;
    }
    if(!pause.loadFromFile("res/button_pause.png", grender)){
        return false;
    }
    if(!about.loadFromFile("res/menu.png", grender)){
        return false;
    }
    if(!background.loadFromFile("res/bg.png", grender)){
        return false;
    }
    if(!boss.loadFromFile("res/boss.png", grender)){
        return false;
    }
    if(!mainobject.loadFromFile("res/a21.png", grender)){
        return false;
    }
    if(!bulletobject.loadFromFile("res/laser.png", grender)){
        return false;
    }
    if(!threatobject[0].loadFromFile("res/rk1.png", grender)){
        return false;
    }
    if(!threatobject[1].loadFromFile("res/rk2.png", grender)){
        return false;
    }
    if(!threatobject[2].loadFromFile("res/rk3.png", grender)){
        return false;
    }
    if(!threatobject[3].loadFromFile("res/a12.png", grender)){
        return false;
    }
    if(!threatobject[4].loadFromFile("res/UF1.png", grender)){
        return false;
    }
    if(!threatobject[5].loadFromFile("res/UF2.png", grender)){
        return false;
    }
    if(!threatobject[6].loadFromFile("res/a32.png", grender)){
        return false;
    }
    if(!heart[0].loadFromFile("res/hat.png", grender)){
        return false;
    }
    if(!heart[1].loadFromFile("res/hat.png", grender)){
        return false;
    }
    if(!heart[2].loadFromFile("res/hat.png", grender)){
        return false;
    }
    if(!youwin.loadFromFile("res/youwin.png", grender)){
        return false;
    }
    return true;
}
void close(){
	background.free();
	SDL_DestroyRenderer(grender);
	grender = NULL;
	SDL_DestroyWindow(gwindow);
	gwindow = NULL;
    IMG_Quit();
    SDL_Quit();
}
int main(int argc, char *argv[])
{
    aa:
    Timer fps_timer;
    int apply = 0;
    bool is_move = true;
    int blood = 200;
    int tong_threat = 3;
    int nen = 101;
    Menu menu;
    bool win = false;
    // bool load1 = true;
    // bool load2 = true;
    if(!init()){
        cout << "SDL khong the khoi tao\n";
    }else{
        if(!loadmedia()){
            cout << "Khong the load hinh anh\n";
        }else{
            bool quit = false;
            // gán menu GAME
            int res_menu = menu.showMenu(grender, font_time);
            tt:
            if(res_menu == 1){
                quit = true;
                SDL_Quit();
            }else if(res_menu == 0){
                if(load2 == true)
                Mix_PlayChannel(-1, start, 0);
                SDL_Delay(3000);
                if(load1 == true)
                Mix_PlayChannel(0, music, 0);
            }else if(res_menu == 2){
                bool thoat = false;
                SDL_Event m_e;
                while(!thoat){
                    while(SDL_PollEvent(&m_e) != 0){
                        if(m_e.type == SDL_QUIT){
                            thoat = true;
                        }else if(m_e.type == SDL_MOUSEBUTTONDOWN){
                            if(m_e.button.button == SDL_BUTTON_LEFT){
                                if(load2 == true)
                                Mix_PlayChannel(-1, click, 0);
                                if(m_e.button.x >= 1170 && m_e.button.y <= 55){
                                    thoat = true;
                                }
                            }
                        }
                    }
                    SDL_SetRenderDrawColor(grender, 0xFF, 0xFF, 0xFF, 0xFF);
				    SDL_RenderClear(grender);
                    about.setRect(8, 8);
                    about.render(grender, NULL);
                    SDL_RenderPresent(grender);
                }
                res_menu = menu.showMenu(grender, font_time);
                goto tt;
            }else if(res_menu == 4){
                bool thoat = false;
                SDL_Event m_e;
                while(!thoat){
                    while(SDL_PollEvent(&m_e) != 0){
                        if(m_e.type == SDL_QUIT){
                            thoat = true;
                        }else if(m_e.type == SDL_MOUSEBUTTONDOWN){
                            if(m_e.button.button == SDL_BUTTON_LEFT){
                                
                                if(m_e.button.x >= 1170 && m_e.button.y <= 55){
                                    if(load2 == true)
                                    Mix_PlayChannel(-1, click, 0);
                                    thoat = true;
                                }else if(m_e.button.x >= 440 && m_e.button.x <= 491 && m_e.button.y <= 383 && m_e.button.y >= 340){
                                    if(load2 == true)
                                    Mix_PlayChannel(-1, click, 0);
                                    tong_threat = 3;
                                    blood = 200;
                                    nen = 101;
                                    ThreatObject :: dem = 0;
                                }else if(m_e.button.x >= 507 && m_e.button.x <= 553 && m_e.button.y <= 383 && m_e.button.y >= 340){
                                    if(load2 == true)
                                    Mix_PlayChannel(-1, click, 0);
                                    tong_threat = 4;
                                    blood = 250;
                                    nen = 126;
                                    ThreatObject :: dem = 0;
                                    background.loadFromFile("res/m_bk1.png", grender);
                                }else if(m_e.button.x >= 570 && m_e.button.x <= 615 && m_e.button.y <= 383 && m_e.button.y >= 340){
                                    if(load2 == true)
                                    Mix_PlayChannel(-1, click, 0);
                                    tong_threat = 6;
                                    blood = 300;
                                    nen = 151;
                                    ThreatObject :: dem = 0;
                                    background.loadFromFile("res/m_bk4.png", grender);
                                }else if(m_e.button.x >= 420 && m_e.button.x <= 530 && m_e.button.y <= 120 && m_e.button.y >= 67){
                                    if(load2 == true)
                                    Mix_PlayChannel(-1, click, 0);
                                    mainobject.loadFromFile("res/green_plane.png", grender);
                                }else if(m_e.button.x >= 585 && m_e.button.x <= 680 && m_e.button.y <= 120 && m_e.button.y >= 67){
                                    if(load2 == true)
                                    Mix_PlayChannel(-1, click, 0);
                                    mainobject.loadFromFile("res/plane_i.png", grender);
                                }else if(m_e.button.x >= 755 && m_e.button.x <= 842 && m_e.button.y <= 120 && m_e.button.y >= 67){
                                    if(load2 == true)
                                    Mix_PlayChannel(-1, click, 0);
                                    mainobject.loadFromFile("res/a21.png", grender);
                                }
                            }
                        }
                    }
                    SDL_SetRenderDrawColor(grender, 0xFF, 0xFF, 0xFF, 0xFF);
				    SDL_RenderClear(grender);
                    back.setRect(8, 8);
                    back.render(grender, NULL);
                    SDL_RenderPresent(grender);
                }
                res_menu = menu.showMenu(grender, font_time);
                goto tt;
            }else{
                bool out = false;
                SDL_Event op;
                // bool load1 = true;
                // bool load2 = true;
                while(!out){
                    while(SDL_PollEvent(&op) != 0){
                        if(op.type == SDL_QUIT){
                            out = true;
                        }else if(op.type == SDL_MOUSEBUTTONDOWN){
                            int xtd = op.button.x;
                            int ytd = op.button.y;
                            if(load2 == true);
                            Mix_PlayChannel(-1, click, 0);
                            if(xtd >= screen_width - 50 && xtd <= screen_width && ytd >= 0 && ytd <= 55){
                                out = true;
                            }else if(xtd >= screen_width / 2 - 50 && xtd <= screen_width / 2 && ytd >= 0 && ytd <= 55){
                                if(load1 == true){
                                    music_on.loadFromFile("res/button_music_off.png", grender);
                                    load1 = false;
                                }else{
                                    music_on.loadFromFile("res/button_music_on.png", grender);
                                    load1 = true;
                                }
                            }else if(xtd >= screen_width / 2 && xtd <= screen_width / 2 + 50 && ytd >= 0 && ytd <= 55){
                                if(load2 == true){
                                    sound_on.loadFromFile("res/button_sound_off.png", grender);
                                    load2 = false;
                                }else{
                                    sound_on.loadFromFile("res/button_sound_on.png", grender);
                                    load2 = true;
                                }
                            }
                        }
                    }
                    SDL_SetRenderDrawColor(grender, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(grender);
                    nenn.setRect(8, 8);
                    nenn.render(grender, NULL);
                    home.setRect(screen_width - 50, 5);
                    home.render(grender, NULL);
                    music_on.setRect(screen_width / 2 - 50, 5);
                    music_on.render(grender, NULL);
                    sound_on.setRect(screen_width / 2 , 5);
                    sound_on.render(grender, NULL);
                    SDL_RenderPresent(grender);
                }
                res_menu = menu.showMenu(grender, font_time);
                goto tt;
            }
            int so_luong_mang = 3;
            
            BulletObject* p[tong_threat];


            //time text


            TextObject time;
            time.setcolor(TextObject :: WHITE_TEXT);
            BulletObject* dan = new BulletObject();
            TextObject mark;
            mark.setcolor(TextObject :: WHITE_TEXT);
            TextObject mang;
            mang.setcolor(TextObject :: WHITE_TEXT);
            TextObject so_lan_bay;
            so_lan_bay.setcolor(TextObject :: WHITE_TEXT);
            TextObject mau;
            mau.setcolor(TextObject :: WHITE_TEXT);
            int mark_value = 0;
            boss.khoi_tao_dan(dan, grender);
            for(int i = 0; i < tong_threat; i++) p[i] = new BulletObject();
            int y = 15;
            for(int i = 0; i < tong_threat; i++){
                threatobject[i].set_X(y);
                y += 2;
                if(y > 15 + (tong_threat - 1) * 2){
                    y = 15;
                }
            }
            int rand_y = 55 + rand()%400;
            if(rand_y > screen_height - 200)
            rand_y = screen_height * 0.3;
            for(int i = 0; i < tong_threat; i++){
                threatobject[i].setRect(screen_width, rand_y);
                threatobject[i].initBullet(p[i] ,grender);
                rand_y += 32;
                if(rand_y > screen_height - 200)
                rand_y = screen_height * 0.3;
            }
            Explosion Exp_threat;
            bool threat = Exp_threat.loadFromFile("res/exp_main.png", grender);
            Explosion Exp_main;
            bool mainn = Exp_main.loadFromFile("res/exp.png", grender);
            if(threat == true) Exp_threat.setclip();
            if(mainn == true) Exp_main.setclip();

			while(!quit){
                fps_timer.start();
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
                    }
                    if(load2 == true)
                    mainobject.handleInput(resume, e, grender, danban);
                    else mainobject.handleInput(resume, e, grender, danbann);
				}
				SDL_SetRenderDrawColor(grender, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(grender);
                if(is_move == true){
                    apply -= 50;
                    background.setRect(apply, 0);
                    background.render(grender, NULL);
                    if(apply <= -BG_WIDTH + screen_width){
                        boss.setRect(screen_width - BOSS_WIDTH, screen_height / 2 -BOSS_HEIGHT / 2);
                        is_move = false;
                    }
                }else{
                    background.setRect(apply, 0);
                    background.render(grender, NULL);
                    boss.render(grender, NULL);
                    boss.handle_move_boss();
                    boss.dan_ban(grender, screen_width, screen_height);
 
                    // xử lý va chạm giữa BOSS và MainObject

                    bool collision = COMMONF :: checkCollision(mainobject.getRect(), boss.getRect());
                    if(collision == true){
                        for(int ex = 0; ex < 4; ex++){
                            int x_posss = mainobject.getRect().x - 50;
                            int y_posss = mainobject.getRect().y - 50;
                            Exp_main.setFrame(ex);
                            Exp_main.setRect(x_posss, y_posss);
                            if(load2 == true)

                            Mix_PlayChannel(-1, vuno, 0);
                            Exp_main.showEXP(grender);
                            
                        }
                        so_luong_mang--;
                        if(so_luong_mang > 0){
                            mainobject.setRect(50, 0);
                            mainobject.render(grender, NULL);
                        }else{
                            mainobject.free();
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "OK", gwindow);
                            SDL_Delay(1000);
                            quit = true;
                        }
                    }


                    // xử lý va chạm giữa đạn của MainObject và Boss
                    int Getframe_w = Exp_threat.getW();
                    int Getframe_h = Exp_threat.getH();
                    vector<BulletObject*> hi = mainobject.get_v_bullet_list();
                    for(unsigned int j1 = 0 ; j1 < hi.size(); j1++){
                        BulletObject* dann = hi.at(j1);
                        bool collision = COMMONF :: checkCollision(dann->getRect(), boss.getRect());
                        if(collision == true){
                            blood--;
                            if(blood <= 0){
                                boss.free();
                                if(load2 == true)
                                Mix_PlayChannel(-1, end_game, 0);
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "CONGRATULATIONS, YOU WIN", "OK", gwindow);
                                
                                win = true;
                                quit = true;
                            }else if(blood == 160){
                                int t = boss.getRect().x;
                                int l = boss.getRect().y;
                                boss.setRect(t, l);
                                boss.loadFromFile("res/nat1.png",grender);
                                boss.render(grender, NULL);
                            }else if(blood == 120){
                                int t1 = boss.getRect().x;
                                int l1 = boss.getRect().y;
                                boss.setRect(t1, l1);
                                boss.loadFromFile("res/nat2.png",grender);
                                boss.render(grender, NULL);
                            }else if(blood == 80){
                                int t2 = boss.getRect().x;
                                int l2 = boss.getRect().y;
                                boss.setRect(t2, l2);
                                boss.loadFromFile("res/nat3.png",grender);
                                boss.render(grender, NULL);
                            }else if( blood == 40){
                                int t3 = boss.getRect().x;
                                int l3 = boss.getRect().y;
                                boss.setRect(t3, l3);
                                boss.loadFromFile("res/nat4.png",grender);
                                boss.render(grender, NULL);
                            }
                            for(int exp = 0; exp < 4; exp++){
                                int xpos = dann->getRect().x - Getframe_w * 0.5;
                                int ypos = dann->getRect().y -Getframe_h * 0.5;
                                Exp_threat.setFrame(exp);
                                Exp_threat.setRect(xpos, ypos);
                                if(load2 == true)
                                Mix_PlayChannel(-1, vuno, 0);
                                Exp_threat.showEXP(grender);
                            }
                            dann->free();
                        }
                    }


                    // xử lý va chạm giữa đạn đạn Boss và MainObject
                    int getframe_w = Exp_threat.getW();
                    int getframe_h = Exp_threat.getH();
                    vector<BulletObject*> vu = boss.getBullet();
                    for(unsigned int i1 = 0; i1 < vu.size(); i1++){
                        BulletObject* pp = vu.at(i1);
                        bool collision = COMMONF :: checkCollision(pp->getRect(), mainobject.getRect());
                        if(collision == true){
                            for(int exp = 0; exp < 4; exp++){
                                int xpos = pp->getRect().x - getframe_w * 0.5;
                                int ypos = pp->getRect().y -getframe_h * 0.5;
                                Exp_threat.setFrame(exp);
                                Exp_threat.setRect(xpos, ypos);
                                if(load2 == true)
                                Mix_PlayChannel(-1, vuno, 0);
                                Exp_threat.showEXP(grender);
                            }
                            so_luong_mang--;
                            if(so_luong_mang > 0){
                                pp->free();
                                mainobject.setRect(50, 0);
                                mainobject.render(grender, NULL);
                                BulletObject* p1 = new BulletObject();
                                boss.khoi_tao_dan(p1, grender);
                                boss.dan_ban(grender, screen_width, screen_height);
                            }else{
                                pp->free();
                                mainobject.free();
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "OK", gwindow);
                                SDL_Delay(1000);
                                quit = true;
                            }
                        }
                    }
                }


                if(ThreatObject :: dem >= 20){
                    so_luong_mang--;
                    for(int ex = 0; ex < 4; ex++){
                        int x_poss = mainobject.getRect().x - 20;
                        int y_poss = mainobject.getRect().y - 20;
                        Exp_main.setFrame(ex);
                        Exp_main.setRect(x_poss, y_poss);
                        if(load2 == true)
                        Mix_PlayChannel(-1, reload, 0);
                        Exp_main.showEXP(grender);
                    }
                    ThreatObject :: dem -= 20;
                    if(so_luong_mang <= 0){
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "OK", gwindow);
                        SDL_Delay(1000);
                        quit = true;
                    }else{
                        mainobject.setRect(50, 0);
                        mainobject.render(grender, NULL);
                    }
                }
                
                //Draw geometry
             
                GeometryFormat hcn_size(0, 0, screen_width, 55);
                ColorData color_data(36, 36, 36);
                Geometry :: Render_HCN(hcn_size, color_data, grender);

                GeometryFormat outLine_size(1, 1, screen_width - 1, 53);
                ColorData color_data2(255, 255, 255);
                Geometry :: Render_Outline(outLine_size, color_data2, grender);

                mainobject.handleBullet(grender);
                mainobject.render(grender, NULL);

                GeometryFormat gsize(screen_width / 2 + 73, 10, blood / 2, 30);
                ColorData gcolor(255, 0, 0);
                Geometry :: Render_HCN(gsize, gcolor, grender);

                GeometryFormat outLine(screen_width / 2 + 73, 9, nen, 32);
                ColorData color_data3(255, 255, 255);
                Geometry :: Render_Outline(outLine, color_data3, grender);

                // load nút home lên
                // pause.setRect(screen_width - 50, 60);
                // pause.render(grender, NULL);
                // xử lý va chạm giữa mainobject và threatobject
                // resume.render(grender, NULL);
                int get_frame_widthh = Exp_main.getW();
                int get_frame_heightt = Exp_main.getH();
                for(int i = 0; i < tong_threat; i++){
                    ThreatObject* p_threat = (threatobject + i);
                    p_threat->render(grender, NULL);
                    p_threat->HandleMove(screen_width, screen_height);
                    p_threat->makeBullet(grender, screen_width, screen_height);

                    bool is_col = COMMONF :: checkCollision(mainobject.getRect(), p_threat->getRect());
                    if(is_col == true){
                        for(int ex = 0; ex < 4; ex++){
                            int x_poss = mainobject.getRect().x - get_frame_widthh * 0.5;
                            int y_poss = mainobject.getRect().y - get_frame_heightt * 0.5;
                            Exp_main.setFrame(ex);
                            Exp_main.setRect(x_poss, y_poss);
                            if(load2 == true)
                            Mix_PlayChannel(-1, vuno, 0);
                            Exp_main.showEXP(grender);
                        }
                        so_luong_mang--;
                        if(so_luong_mang > 0){
                            p_threat->setRect(screen_width, 55 + rand() % 400);
                            p_threat->render(grender, NULL);
                            BulletObject* p = new BulletObject();
                            p_threat->initBullet(p,grender);
                            p_threat->makeBullet(grender, screen_width, screen_height);
                            mainobject.setRect(50, 0);
                            mainobject.render(grender, NULL);
                        }else{
                            p_threat->free();
                            mainobject.free();
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "OK", gwindow);
                            SDL_Delay(1000);
                            quit = true;
                            }
                       
                    }
                }
                    int get_frame_width = Exp_threat.getW();
                    int get_frame_height = Exp_threat.getH();


                    // xử lý đạn của mainobject va chạm với threatobject

                    vector<BulletObject*> bullet_list = mainobject.get_v_bullet_list();
                    for(unsigned int j = 0; j < bullet_list.size(); j++){
                        BulletObject* p = bullet_list.at(j);
                        if(p != NULL){
                            for(int k = 0; k < tong_threat; k++){
                                ThreatObject* p_threat = (threatobject + k);
                                bool res = COMMONF :: checkCollision(p->getRect(), p_threat->getRect());
                                if(res == true){
                                    mark_value++;
                                    if(mark_value >= 30){
                                        if(so_luong_mang < 3){
                                        so_luong_mang++;
                                        mark_value -= 30;
                                        }
                                    }
                                    for(int ex = 0; ex < 4; ex++){
                                        int x_pos = p->getRect().x - get_frame_width * 0.5;
                                        int y_pos = p->getRect().y - get_frame_height * 0.5;
                                        Exp_threat.setFrame(ex);
                                        Exp_threat.setRect(x_pos, y_pos);
                                        if(load2 == true)
                                        Mix_PlayChannel(-1, vuno, 0);
                                        Exp_threat.showEXP(grender);
                                    }
                                    p_threat->setRect(screen_width, 55 + rand() % 400);
                                    p->free();
                                }
                            }
                        }
                    }

                    // xử lý va chạm giữa đạn của threatobject và mainobject

                    int get_frame_w = Exp_threat.getW();
                    int get_frame_h = Exp_threat.getH();
                    for(int k1 = 0; k1 < tong_threat; k1++){
                        ThreatObject* giau = (threatobject + k1);
                        vector<BulletObject*> vu = giau->getBullet();
                        for(unsigned int i1 = 0; i1 < vu.size(); i1++){
                            BulletObject* pp = vu.at(i1);
                            bool collision = COMMONF :: checkCollision(pp->getRect(), mainobject.getRect());
                            if(collision == true){
                                for(int exp = 0; exp < 4; exp++){
                                    int xpos = pp->getRect().x - get_frame_w * 0.5;
                                    int ypos = pp->getRect().y -get_frame_h * 0.5;
                                    Exp_threat.setFrame(exp);
                                    Exp_threat.setRect(xpos, ypos);
                                    if(load2 == true)

                                    Mix_PlayChannel(-1, vuno, 0);
                                    Exp_threat.showEXP(grender);
                                }
                                so_luong_mang--;
                                if(so_luong_mang > 0){
                                    pp->free();
                                    mainobject.setRect(50, 0);
                                    mainobject.render(grender, NULL);
                                    BulletObject* p1 = new BulletObject();
                                    giau->initBullet(p1,grender);
                                    giau->makeBullet(grender, screen_width, screen_height);
                                }else{
                                    pp->free();
                                    mainobject.free();
                                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "OK", gwindow);
                                    SDL_Delay(1000);
                                    quit = true;
                                }
                            }
                        }
                    }


                // show game time

                string str_time = "Time : ";
                int timestart = SDL_GetTicks();
                Uint32 time_val = timestart / 1000;
                Uint32 val_time = 305 - time_val;
                if(val_time <= 0){
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "GAME OVER", "OK", gwindow);
                    quit = true;
                }else{
                    string str_val = to_string(val_time);
                    str_time += str_val;
                    time.setText(str_time);
                    time.LoadFromRenderText(font_time, grender);
                    time.renderText(grender, screen_width - 200, 15);
                }
                string value_mark = to_string(mark_value);
                string strmark = "Mark : ";
                strmark += value_mark;
                mark.setText(strmark);
                mark.LoadFromRenderText(font_time, grender);
                mark.renderText(grender,screen_width - 400, 15);
                string value_mang = to_string(so_luong_mang);
                string strmang = "Lives : ";
                strmang += value_mang;
                mang.setText(strmang);
                mang.LoadFromRenderText(font_time, grender);
                mang.renderText(grender, screen_width - 930, 15);
                int xxx = 460;
                string val_bay = to_string(ThreatObject :: dem);
                string slb = "Times ThreatObjects Passed : ";
                slb += val_bay;
                so_lan_bay.setText(slb);
                so_lan_bay.LoadFromRenderText(font_time, grender);
                so_lan_bay.renderText(grender, screen_width - 1230, 15);
                string k = to_string(blood);
                string strmau = "HP_BOSS : ";
                strmau += k;
                mau.setText(strmau);
                mau.LoadFromRenderText(font_time, grender);
                mau.renderText(grender, screen_width - 700, 15);
                for(int i = 0; i < so_luong_mang; i++){
                    heart[i].setRect(xxx, 10);
                    heart[i].render(grender, NULL);
                    xxx += 30;
                    if(xxx > 460 + (so_luong_mang - 1) * 30){
                        xxx = 460;
                    }
                }
				SDL_RenderPresent(grender);
                int real_time = fps_timer.get_ticks(); 
                int time_one_frame = 1000 / frame_per_second;
                if(real_time < time_one_frame){
                    int delay_time = time_one_frame - real_time;
                    if(delay_time >= 0)
                    SDL_Delay(delay_time);
                }
            }
        }
    }
    bool g = false;
    SDL_Event t;
    while(!g){
        while(SDL_PollEvent(&t) != 0){
            if(t.type == SDL_QUIT){
                g = true;
            }else if(t.type == SDL_MOUSEBUTTONDOWN){
                if(load2 == true)
                Mix_PlayChannel(-1, click, 0);
                if(t.button.x >= 460 && t.button.x <= 640 && t.button.y <= 595 && t.button.y >= 515){
                    g = true;
                    gameover.free();
                    load1 = true;
                    load2 = true;
                    Mix_HaltChannel(0);
                }else if(t.button.x >= 715 && t.button.x <= 845 && t.button.y <= 595 && t.button.y >= 515){
                    g = true;
                    close();
                    SDL_Quit();
                }
            }
            SDL_SetRenderDrawColor(grender, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(grender);
            if(win != true){
                gameover.setRect(0, 0);
                gameover.render(grender, NULL);
            }else{
                youwin.setRect(8, 8);
                youwin.render(grender, NULL);
            }
            SDL_RenderPresent(grender);
        }
    }
    if(g == true) goto aa;
    close();
    return 0;
}
