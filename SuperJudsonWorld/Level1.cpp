#include "SuperJudsonWorld.h"
#include "Home.h"
#include "Level1.h"
#include "Level2.h"
#include "GameOver.h"
#include "Player.h"
#include "Platform.h"
#include "Background.h"

#include <string>
#include <fstream>
using std::ifstream;
using std::string;

#include "Enemy.h"
#include "Level2.h"

// ------------------------------------------------------------------------------
// Inicializa membros est�ticos da classe

Scene * Level1::scene = nullptr;

// ------------------------------------------------------------------------------

void Level1::Init()
{
    // cria gerenciador de cena
    scene = new Scene();

    // pano de fundo do jogo
    backg = new Background("Resources/Level1.png");
    scene->Add(backg, STATIC);


    // adiciona jogador na cena
    scene->Add(SuperJudsonWorld::player, MOVING);
    
    Platform* plat;
    Enemy* en;
    float posX, posY;
    uint  platType;
    Color white{ 1,1,1,1 };
    int cont = 0;

    ifstream fin;
    fin.open("Level1.txt");

    fin >> posX;
    while (!fin.eof())
    {
        if (fin.good())
        {
            // l� linha com informa��es da plataforma
            fin >> posY; fin >> platType;

            plat = new Platform(posX, posY, platType, white);
            scene->Add(plat, STATIC);

            if (cont == 0 || cont == 2 || cont == 3) {
                float x = plat->X() - (plat->Width() * 0.25f) / 2;
                float y = plat->Y() - (plat->Height() * 0.25f) / 2;

                en = new Enemy(x, y, plat);
                scene->Add(en, MOVING);
            }
            cont++;
        }
        else
        {
            // ignora coment�rios
            fin.clear();
            char temp[80];
            fin.getline(temp, 80);
        }

        fin >> posX;

        
    }
    fin.close();

    // ----------------------

    // inicia com m�sica
    //SuperJudsonWorld::audio->Frequency(MUSIC, 0.94f);
    //SuperJudsonWorld::audio->Frequency(TRANSITION, 1.0f);
    //SuperJudsonWorld::audio->Play(MUSIC);
}

// ------------------------------------------------------------------------------

void Level1::Update()
{
    if (window->KeyPress(VK_ESCAPE))
    {
        SuperJudsonWorld::audio->Stop(MUSIC);
        SuperJudsonWorld::player->Reset();
        SuperJudsonWorld::NextLevel<Home>();
    }

    if(window->KeyPress('N')) SuperJudsonWorld::NextLevel<Level2>();

    //else if (SuperJudsonWorld::player->Bottom() < 0 || SuperJudsonWorld::player->Top() > window->Height())
    //{
    //    SuperJudsonWorld::audio->Stop(MUSIC);
    //    SuperJudsonWorld::NextLevel<GameOver>();
    //    SuperJudsonWorld::player->Reset();
    //}
    //else if (SuperJudsonWorld::player->Level() == 1 || window->KeyPress('N'))
    //{
    //    //SuperJudsonWorld::NextLevel<Level2>();
    //}
    else
    {
        scene->Update();
        scene->CollisionDetection();
    }    
}

// ------------------------------------------------------------------------------

void Level1::Draw()
{
    backg->Draw();
    scene->Draw();

    if (SuperJudsonWorld::viewBBox)
        scene->DrawBBox();

    SuperJudsonWorld::hud->Draw();
}

// ------------------------------------------------------------------------------

void Level1::Finalize()
{
    scene->Remove(SuperJudsonWorld::player, MOVING);
    delete scene;
}

// ------------------------------------------------------------------------------
