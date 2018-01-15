/* Classify a triangle given with coordinates, and a P point */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

// Integer constants representing types of triangle.
enum { Invalid, WrongTriangle, Vertex, Side, Inside, Outside };

typedef struct points {
    int x;
    int y;
} point;

//invalid input e.g. 34x or 032 or 3.4
bool isValidInput(const char str[]){
    const int BIT8 = 256;
    char str2[BIT8];
    sprintf( str2, "%d", atoi(str) );
    if ( strlen (str) != strlen (str2) ){return false;}
    else {
        if (strlen(str) == 1 && str[0] == '0') return true;
        else if (strlen(str) == 1 && ! isdigit(str[0])) return false;
        else if ( str[0] == '0') return false;
        else {return true;}
    }
}

point convert(const char x[], const char y[]){
    point Point;
    Point.x = atoi (x);
    Point.y = atoi (y);
    return Point;
}

//flat, impossible triangle
bool isTriangle( point a, point b, point c ){
    //two or three vertices are the same
    if ( a.x == b.x && a.y == b.y) return false;
    else if ( a.x == c.x && a.y == c.y) return false;
    else if ( c.x == b.x && c.y == b.y) return false;

    //collinear
    //the area of triangle using matrices:
    /*
                   | x_1 y_1 1 |
     A = 1/2 * abs | x_2 y_2 1 |
                   | x_3 y_3 1 |
    */
    //if the triangle is collinear the A = 0, so the determinant = 0
    int det, plus, minus;
    plus = a.x * b.y + b.x * c.y + c.x * a.y;
    minus = a.x * c.y + b.x * a.y + c.x * b.y;
    det =  plus - minus;
    if ( det == 0 ) { return false; }
    else return true;

}

//P == A,B or C
bool isVertex( point a, point b, point c, point p ){
    if (a.x == p.x && a.y == p.y){ return true; }
    else if (b.x == p.x && b.y == p.y){ return true; }
    else if (c.x == p.x && c.y == p.y){ return true; }
    else return false;
}

//the greatest common divisor
//Euclidean algorithm
int gcd( int a, int b ){
    int r = a % b;
    if ( r == 0 ){ return b; }
    return gcd( b, r );
}

//We need sign function to shorten the code
int sgn(int x){
  if (x < 0) return -1;
  else if (x == 0) return 0;
// x > 0
  else return 1;
}

//PQ: segment
bool isSide( point p, point q, point s ){
//if the segment is vertical
     if ( p.x == q.x){
         int i = p.y;
         while(i != q.y){
              if ( p.x == s.x && i == s.y) return true;
              i = i + sgn(q.y - p.y);
         }
     }

//if the segment is horizontal
//enough: interchange x and y
      if ( p.y == q.y){
          int i = p.x;
          while(i != q.x){
               if ( p.y == s.y && i == s.x) return true;
               i = i + sgn(q.x - p.x);
          }
      }

/*the general case: not horizontal nor vertical*/
     int dx, dy;
     dx = abs ( p.x - q.x );
     dy = abs ( p.y - q.y );
     if ( dx > 0 && dy > 0){
         int num = gcd (dx, dy);
//the arrangement is important
//we change P and Q if p_x > q_x
      int i = 0;
      while(i <= num){
          if ((p.x + (dx / num) * i * sgn (q.x - p.x) == s.x) && (p.y + (dy / num) * i * sgn(q.y - p.y) == s.y))
              return true;
              i++;
          }
     }
     return false;
}

//the order of the points matter
bool isAllSides(point a, point b, point c, point p){
    if (isSide(a, b, p) || isSide(b, c, p) || isSide(c, a, p)) return true;
    else return false;
}

//\vec{PQ} and \vec{PS}
bool isLeftSide( point q, point r, point p){
//first translate everything -> p is the origin
    r.x -= q.x;
    r.y -= q.y;
    p.x -= q.x;
    p.y -= q.y;
    q.x = 0;
    q.y = 0;

//we need a double point struct
    typedef struct dPoints{
        double x, y;
    } dpoint;

    dpoint a, b;
    a.x = r.x;
    a.y = r.y;
    b.x = p.x;
    b.y = p.y;

//normalize the vectors:
     double length_a = sqrt(a.x * a.x + a.y * a.y);
     double length_b = sqrt(b.x * b.x + b.y * b.y);
     a.x /= length_a;
     b.x /= length_b;
     a.y /= length_a;
     b.y /= length_b;

     const double PI = 3.14159265358979;

     double angle_a = acos(a.x);
     if (a.y < 0) angle_a = 2 * PI - angle_a;

     double angle_b = acos(b.x);
     if (b.y < 0) angle_b = 2 * PI - angle_b;

     if (angle_b >= angle_a && angle_b <= angle_a + PI) return true;
     else return false;
}

bool isInside(point a, point b, point c, point p){
    if (isLeftSide(a,b,p) == isLeftSide(b,c,p) && isLeftSide(b,c,p) == isLeftSide(c,a,p) && isLeftSide(c,a,p) == isLeftSide(a,b,p)){
        return true;
    }
  else return false;
}

int triangle(const char ax[], const char ay[], const char bx[], const char by[],
const char cx[], const char cy[], const char px[], const char py[]){
    //the use of De Morgan's law could have made it shorter
    if ((! isValidInput(ax)) || (! isValidInput(bx)) || (! isValidInput(cx)) || (! isValidInput(px))){
      return Invalid;
    }
    if ((! isValidInput(ay)) || (! isValidInput(by)) || (! isValidInput(cy)) || (! isValidInput(py))){
      return Invalid;
    }

    point a, b, c, p;
    a = convert(ax, ay);
    b = convert(bx, by);
    c = convert(cx, cy);
    p = convert(px, py);

    if ( ! isTriangle( a, b, c ) ) return WrongTriangle;
    else if ( isVertex( a, b, c, p ) ) return Vertex;
    else if ( isAllSides( a, b, c, p )) return Side;
    else if (isInside(a,b,c,p)) return Inside;

    else return Outside;
    //else if ( isTriangle())


}

void testValidity(){
    assert(triangle("1","1","1","1","1","1","1","x") == Invalid);
    assert(triangle("1","1","1","1","1","1","x","1") == Invalid);
    assert(triangle("1","1","1","1","x","1","1","1") == Invalid);
    assert(triangle("1","1","x","1","x","1","1","1") == Invalid);
    assert(triangle("1","1","1","x","x","1","1","1") == Invalid);
    assert(triangle("x","x","1","1","x","1","1","1") == Invalid);
    assert(triangle("1.1","1","1","1","1","1","1","1") == Invalid);
    assert(triangle("01","1","1","1","1","1","1","1111") == Invalid);
}

void testTriangle(){
    assert(triangle("1","1","1","1","1","1","1","1") == WrongTriangle);
    assert(triangle("1","4","1","5","1","6","2","1") == WrongTriangle);
    assert(triangle("1","1","2","2","3","3","1","4") == WrongTriangle);
    assert(triangle("1","1","2","3","3","5","5","4") == WrongTriangle);
}

void testVertex(){
    assert(triangle("1","1","2","2","3","4","1","1") == Vertex);
    assert(triangle("1","12123","-223443","-852","-6653","3246","-223443","-852") == Vertex);
    assert(triangle("11","44","55","34","-101","303","-101","303") == Vertex);
    assert(triangle("244","123432552","719663","0","0","-3","244","123432552") == Vertex);
    assert(triangle("244","123432552","719663","0","0","-3","244","123432552") == Vertex);
    assert(triangle("-20","-20","-20","-100","0","0","0","0") == Vertex);
}

void testSides(){
    assert(triangle("1","1","3","3","0","2","2","2") == Side);
    assert(triangle("1","3","1","10","9","7","1","7") == Side);
    assert(triangle("1","0","3","0","-7","1","2","0") == Side);
    assert(triangle("0","0","0","9","12","0","2","0") == Side);
    assert(triangle("0","0","0","9","12","0","5","0") == Side);
    assert(triangle("0","0","0","9","12","0","0","7") == Side);
    assert(triangle("0","0","0","9","12","0","0","5") == Side);
    assert(triangle("0","0","0","9","12","0","4","6") == Side);
    assert(triangle("0","0","0","9","12","0","8","3") == Side);
}

void testInside(){
    assert(triangle("-1","-1","0","1","2","1","0","0") == Inside);
//(-2,1) (3,3) (1,-3) triangle all inside points
    assert(triangle("-2","1","3","3","1","-3","1","2") == Inside);
    assert(triangle("-2","1","3","3","1","-3","2","2") == Inside);
    assert(triangle("-2","1","3","3","1","-3","-1","1") == Inside);
    assert(triangle("-2","1","3","3","1","-3","0","1") == Inside);
    assert(triangle("-2","1","3","3","1","-3","1","1") == Inside);
    assert(triangle("-2","1","3","3","1","-3","-1","0") == Inside);
    assert(triangle("-2","1","3","3","1","-3","0","0") == Inside);
    assert(triangle("-2","1","3","3","1","-3","1","0") == Inside);
    assert(triangle("-2","1","3","3","1","-3","0","-1") == Inside);
    assert(triangle("-2","1","3","3","1","-3","1","-1") == Inside);

//test many points for (0,0) (1000,1) (1000,-1) triangle
    const char k[] = "1000";
    for (int i = 1; i < atoi(k); i++){
        char str[100];
        sprintf(str, "%d", i);
        assert(triangle("0","0",k,"1",k,"-1", str,"0") == Inside);
    }
}

void testOutside(){
//(-2,1) (3,3) (1,-3) triangle outside points near the sides
    assert(triangle("-2","1","3","3","1","-3","2","3") == Outside);
    assert(triangle("-2","1","3","3","1","-3","-1","2") == Outside);
    assert(triangle("-2","1","3","3","1","-3","3","2") == Outside);
    assert(triangle("-2","1","3","3","1","-3","3","1") == Outside);
    assert(triangle("-2","1","3","3","1","-3","-1","-1") == Outside);
    assert(triangle("-2","1","3","3","1","-3","2","-1") == Outside);
    assert(triangle("-2","1","3","3","1","-3","0","-2") == Outside);
    assert(triangle("-2","1","3","3","1","-3","2","-2") == Outside);
}

void test(){
    testValidity();
    testTriangle();
    testVertex();
    testSides();
    testOutside();

    printf("All tests passed!\n");
}

void print(int type) {
    switch (type) {
        case Invalid: printf("Invalid"); break;
        case WrongTriangle: printf("Wrong triangle"); break;
        case Vertex: printf("On the vertex"); break;
        case Side: printf("On the side"); break;
        case Inside: printf("Inside"); break;
        case Outside: printf("Outside"); break;
        default: printf("Valid"); break;
    }
    printf("\n");
}

// Run the program or, if there are no arguments, test it.
int Tringle_furtherMain(int n, char **argv) {
    if (n == 1) {
        test();
    }
    else if (n == 9) {
        int result = triangle(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
        print(result);
    }
    else {
        fprintf(stderr, "Use e.g.: ./triangle_further 3 4 5 6 7 8 9 10\n");
        exit(1);
    }
    return 0;
}
