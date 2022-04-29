#include "raylib.h"

struct AnimData
{
    /* data */
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updataAnimData(AnimData data, float deltaTime, int maxFrame)
{   
    //update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width; 
        data.frame++;
        if(data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{   
    // pencere boyutları
    int windowDimensions[2];
    windowDimensions[0]= 512;
    windowDimensions[1]=380;


    // pencere oluşturma
    InitWindow(windowDimensions[0],windowDimensions[1],"Dapper Dasher by Yasin Enes");

    // yer çekimi ivmesi (pixels/second)/second 
    const int gravity{1000};

    // nebula değişkenleri
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");


    const int sizeOfNebulae{5};
    AnimData nebulae[sizeOfNebulae]{}; 

    for(int i=0; i<sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.x = windowDimensions[0]+i*500;
        nebulae[i].pos.y = windowDimensions[1]-nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
    }

    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };


    // nebula x eksenindeki hızı (pixels/second)
    int nebula_Vel{-300};


    // scarfy değişkenleri
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;


    // dikdörtgen havada mı ?
    bool isInAir{};
    // zıplama hızı (pixels/second)
    const int jump_Vel{-650};

    int rec_Velocity{0};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

        bool collision{};
    

    SetTargetFPS(60);

    while(!WindowShouldClose())
    {   
        // delta time (bir önceki frame 'den bu yana geçen zaman)
        const float delta_Time{GetFrameTime()};


        // çizmeye başla
        BeginDrawing();
        ClearBackground(WHITE);

        // ön arka planı sola kaydır
        bgX -= 20 * delta_Time;
        if(bgX <= -background.width*2)
        {
            bgX = 0.0;
        }

        // orta arka planı sola kaydır
        mgX -= 40 * delta_Time;
        if(mgX <= -midground.width*2)
        {
            mgX = 0.0;
        }

        // geri arka planı sola kaydır
        fgX -= 80 * delta_Time;
        if(fgX <= -foreground.width*2)
        {
            fgX = 0.0;
        }

        // ön arka planı çiz
        Vector2 bg1Pos{bgX,0.0};
        DrawTextureEx(background,bg1Pos,0.0,2,WHITE);
        Vector2 bg2Pos{bgX+ background.width*2,0.0};
        DrawTextureEx(background,bg2Pos,0.0,2,WHITE);

        // orta arka planı çiz
        Vector2 mg1Pos{mgX,0.0};
        DrawTextureEx(midground,mg1Pos,0.0,2,WHITE);
        Vector2 mg2Pos{mgX+ midground.width*2,0.0};
        DrawTextureEx(midground,mg2Pos,0.0,2,WHITE);

        // geri arka planı çiz
        Vector2 fg1Pos{fgX,0.0};
        DrawTextureEx(foreground,fg1Pos,0.0,2,WHITE);
        Vector2 fg2Pos{fgX+ foreground.width*2,0.0};
        DrawTextureEx(foreground,fg2Pos,0.0,2,WHITE);

        // yer kontrol
        if(isOnGround(scarfyData, windowDimensions[1]))
        {
            // diktörtgen yerde
            rec_Velocity = 0;
            isInAir = false;
        }
        else
        {
            // dikdörtgen havada
            rec_Velocity += gravity * delta_Time;
            isInAir = true;
        }

        // jump check
        if(IsKeyPressed(KEY_SPACE) && !isInAir) // havada değilse ve boşluk tuşuna basılmışsa
        {
            rec_Velocity += jump_Vel;
        }

        // scarfy nin Y eksenindeki konumunu değiştirme
        scarfyData.pos.y += rec_Velocity * delta_Time;

        // nebula nın X eksenindeki konumunu değiştirme     
        for(int i=0; i<sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebula_Vel * delta_Time;        
        }

        // update finish line
        finishLine += nebula_Vel * delta_Time;

        // update Scarfy's animation frame
        if(!isInAir)
        {
            // update running time
            scarfyData = updataAnimData(scarfyData, delta_Time, 5);
        }

        // update Nebula's animation frame
        for(int i=0; i<sizeOfNebulae; i++)
        {
            nebulae[i] = updataAnimData(nebulae[i], delta_Time, 7);
        }

            for(AnimData nebula : nebulae)
        {   
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad            
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if(CheckCollisionRecs(nebRec,scarfyRec))
            {
                collision = true;
            }
        }

        if(collision)
        {
            //lose the game
            DrawText("Oyun bitti!",windowDimensions[0]/4,windowDimensions[1]/2,40,WHITE);

        }
        else if(scarfyData.pos.x >= finishLine)
        {
            //win the game
            DrawText("Oyunu Sen Kazandin!",windowDimensions[0]/8,windowDimensions[1]/2,40,WHITE);

        }
        else
        {
            // draw nebula
            for(int i=0 ; i<sizeOfNebulae ;  i++)
            {
                DrawTextureRec(nebula,nebulae[i].rec, nebulae[i].pos,WHITE);
            }
            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        

        // çizmeyi bitir
        EndDrawing();
    }

    UnloadTexture(nebula);
    UnloadTexture(scarfy);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();


}