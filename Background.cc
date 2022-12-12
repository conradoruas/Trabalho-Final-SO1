#include "Point.h"
#include "Vector.h"
#include "Background.h"
#include "Sprite.h"
#include <memory>

void Background::load_assets() {
   // represents the middle of the image width-wise, and top height-wise
   bgMid = Point(0, 0);
   fgMid = Point(800, 0);
   fg2Mid= Point(300, 300);

   // Vai para o path dos recursos
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));
   // carrega sprite
   bg = std::make_shared<Sprite> ("BGstars.png");
   al_destroy_path(path);       
}

// desenha o background
void Background::draw() {
    bg->draw_parallax_background(bgMid.x, 0);
}

// atualiza a posicao do background
void Background::update(double dt) {
   bgMid = bgMid + bgSpeed * dt;

   if (bgMid.x >= 800) {
      bgMid.x = 0;
   }
}
