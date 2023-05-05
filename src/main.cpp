#include "Application.h"

int main()
{
    Application app(1800, 1200, "Cube");
    app.init();
    app.render();     
   
    return 0;
}