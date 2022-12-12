#ifndef FONT_H
#define FONT_H

#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

class Font {

  public:
   
   /** @fn Font(string&, int)
    * @brief Constructor
    * @param s text to be drawn to the screen
    * @param i size of text
    */
   Font(const std::string& s, int i);
   void load();
   ~Font();

  
   bool isLoaded() const;
   void drawTextCentered(const ALLEGRO_COLOR&, const std::string&);
   void drawText(int, int, const ALLEGRO_COLOR&, const std::string&);
   void drawTextF(const ALLEGRO_COLOR&, int x, int y, const std::string&, int);
   void drawTextCenteredF(const ALLEGRO_COLOR&, const std::string&, int);

   
  private:
   int _size;
   std::string _filename;
   ALLEGRO_FONT* _font;
};



#endif
