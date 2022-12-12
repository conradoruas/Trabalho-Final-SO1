#include "Window.h"
#include <allegro5/allegro.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>

int main() {
   srand(time(0));
   
   Window shooty(800, 600, 60);
   shooty.init();
   shooty.setName("YOU");
   shooty.run();

   return 0;

}


