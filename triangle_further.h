/*
mouse pointer detection
*/

enum { Invalid, WrongTriangle, Vertex, Side, Inside, Outside };

//is mouse in the triangle
int triangle(const char ax[], const char ay[], const char bx[], const char by[],
const char cx[], const char cy[], const char px[], const char py[]);
