// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"


int main(int, char*[]) {
bool keepPlaying = true;
   try {
    while (keepPlaying) {
    std::cout << "running" << std::endl;
    Engine* engine = new Engine;
    keepPlaying = engine->play();
    delete engine;
    }
//    delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
