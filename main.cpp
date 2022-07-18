#include <graphics.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdbool.h>
#include <stdint.h>

using namespace std;

struct vec3d{
   
    float x , y, z;
}; 

struct triangle
{
    vec3d points[3];
};

struct mesh
{
    vector<triangle> triangles;
};

struct matrix4x4{
   float any[4][4] = {0};

};
class meshPopulate{
   private:
      mesh meshcube;
      int windowWidth = 900, windowHeight =700; //for calculating aspect ratio
      float aspectRatio;
      float fTheta;
      float znear, zfar, fov; 
      matrix4x4 projmatrix;
      vec3d vcamera = {0,0,0};

      
      void MultiplyMatrixVector(vec3d &i, vec3d &o, matrix4x4 &any)
	   {
		   o.x = i.x * any.any[0][0] + i.y * any.any[1][0] + i.z * any.any[2][0] + any.any[3][0];
   		o.y = i.x * any.any[0][1] + i.y * any.any[1][1] + i.z * any.any[2][1] + any.any[3][1];
	   	o.z = i.x * any.any[0][2] + i.y * any.any[1][2] + i.z * any.any[2][2] + any.any[3][2];
	   	float w = i.x * any.any[0][3] + i.y * any.any[1][3] + i.z * any.any[2][3] + any.any[3][3];

		   if (w != 0.0f)
	   	{
	   		o.x /= w; o.y /= w; o.z /= w;
		   }
	}
   public:
      meshPopulate()
      {
         meshcube.triangles = {

            // SOUTH
		      { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		      { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

	      	// EAST                                                      
	      	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
	      	{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

	      	// NORTH                                                     
	      	{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
	      	{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

	      	// WEST                                                      
      		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
      		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

	      	// TOP                                                       
	      	{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
	      	{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

	      	// BOTTOM                                                    
	      	{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
   		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },


         };
      
      aspectRatio = float(windowHeight)/float(windowWidth);
      znear = 1;
      zfar = 100;
      fov = 90.0;
      float fovrad = 1.0f/tanf(fov * 0.5f / 180.0f * 3.14159f);
      projmatrix.any[0][0]= aspectRatio*fovrad;
      projmatrix.any[1][1]= fovrad;
      projmatrix.any[2][2]= zfar/(zfar-znear);
      projmatrix.any[2][3]= 1.0f;
      projmatrix.any[3][3]= 0.0f;
      projmatrix.any[3][2]= (-znear*zfar)/(zfar-znear);
      
         
      }

    
	void DrawCircle(int xc, int yc, int r, short c = 0x2588, short col = 0x000F)
	{
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		while (y >= x) // only formulate 1/8 of circle
		{
			putpixel(xc - x, yc - y, col);//upper left left
			putpixel(xc - y, yc - x, col);//upper upper left
			putpixel(xc + y, yc - x, col);//upper upper right
			putpixel(xc + x, yc - y, col);//upper right right
			putpixel(xc - x, yc + y, col);//lower left left
			putpixel(xc - y, yc + x, col);//lower lower left
			putpixel(xc + y, yc + x, col);//lower lower right
			putpixel(xc + x, yc + y, col);//lower right right
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	}

   void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3 ){
         line(x1,y1,x2,y2);
         line(x1,y1,x3,y3);
         line(x2,y2,x3,y3);
      }
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3,short col = 0x000F)
	{
		auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) putpixel(i, ny, col); };
		
		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
		if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
		if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) {   // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										 // Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i<dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:

			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);   										
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y>y3) return;
		}
	}

   void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3){
		//get length
		float d1 = sqrt(((y2-y1)*(y2-y1))+((x2-x1)*(x2-x1)));
	  	float d2 = sqrt(((y3-y2)*(y3-y2))+((x3-x2)*(x3-x2)));
  		float d3 = sqrt(((y1-y3)*(y1-y3))+((x1-x3)*(x1-x3)));
		if(((d1<d2)||(d1=d2))&&((d1<d2)||(d1=d2))){
			int tx = x1;
			int ty = y1;
			float vx =(x2-x1)/d1;
			float vy = (y2-y1)/d1;
			int counter =0;
			while(counter <d1){
				line(x3,y3,tx,ty);
				tx = tx+vx;
				ty = ty +vy;
				counter++;
			}
		}
		else if((d2<d3)||(d2=d3)){
			int tx = x2;
			int ty = y2;
			float vx =(x3-x2)/d2;
			float vy = (y3-y2)/d2;
			int counter =0;
			while(counter <d1){
				line(x1,y1,tx,ty);
				tx = tx+vx;
				ty = ty +vy;
				counter++;
			}
		}	
		else{
			int tx = x3;
			int ty = y3;
			float vx =(x1-x3)/d3;
			float vy = (y1-y3)/d3;
			int counter =0;
			while(counter <d1){
				line(x2,y2,tx,ty);
				tx = tx+vx;
				ty = ty +vy;
				counter++;
			}
		}	
	  }
   
   void floodFill(int x,int y,int oldcolor,int newcolor)
      {
         if(getpixel(x,y) == oldcolor)
         {
            putpixel(x,y,newcolor);
            floodFill(x+1,y,oldcolor,newcolor);
            floodFill(x,y+1,oldcolor,newcolor);
            floodFill(x-1,y,oldcolor,newcolor);
            floodFill(x,y-1,oldcolor,newcolor);
         }
      }
   void drawcycle(){
         
            // Set up rotation matrices
	   	   matrix4x4 matRotZ, matRotX;
	         fTheta += .05f  ;

		      // Rotation Z
	      	matRotZ.any[0][0] = cosf(fTheta);
	      	matRotZ.any[0][1] = sinf(fTheta);
    	   	matRotZ.any[1][0] = -sinf(fTheta);
	      	matRotZ.any[1][1] = cosf(fTheta);
   	   	matRotZ.any[2][2] = 1;
   	   	matRotZ.any[3][3] = 1;

	      	// Rotation X
	      	matRotX.any[0][0] = 1;
	       	matRotX.any[1][1] = cosf(fTheta * 0.5f);
	       	matRotX.any[1][2] = sinf(fTheta * 0.5f);
	      	matRotX.any[2][1] = -sinf(fTheta * 0.5f);
	      	matRotX.any[2][2] = cosf(fTheta * 0.5f);
	      	matRotX.any[3][3] = 1;
         
         for(auto tri : meshcube.triangles){
            triangle triprojected, tritranslated, trirotatedX, trirotatedXZ;
            
            //rotation x
            MultiplyMatrixVector(tri.points[0], trirotatedX.points[0], matRotX);
            MultiplyMatrixVector(tri.points[1], trirotatedX.points[1], matRotX);
            MultiplyMatrixVector(tri.points[2], trirotatedX.points[2], matRotX);

            //rotation Z
            MultiplyMatrixVector(trirotatedX.points[0], trirotatedXZ.points[0], matRotZ);
            MultiplyMatrixVector(trirotatedX.points[1], trirotatedXZ.points[1], matRotZ);
            MultiplyMatrixVector(trirotatedX.points[2], trirotatedXZ.points[2], matRotZ);

            tritranslated =trirotatedXZ;
            tritranslated.points[0].z += 3.0f;
            tritranslated.points[1].z += 3.0f;
            tritranslated.points[2].z += 3.0f;
            

            // Use Cross-Product to get surface normal
			vec3d normal, line1, line2;
			line1.x = tritranslated.points[1].x - tritranslated.points[0].x;
			line1.y = tritranslated.points[1].y - tritranslated.points[0].y;
			line1.z = tritranslated.points[1].z - tritranslated.points[0].z;

			line2.x = tritranslated.points[2].x - tritranslated.points[0].x;
			line2.y = tritranslated.points[2].y - tritranslated.points[0].y;
			line2.z = tritranslated.points[2].z - tritranslated.points[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			// It's normally normal to normalise the normal
			float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
			normal.x /= l; normal.y /= l; normal.z /= l;

         // MultiplyMatrixVector(tri.points[0], triprojected.points[0], projmatrix);
         // MultiplyMatrixVector(tri.points[1], triprojected.points[1], projmatrix);
         // MultiplyMatrixVector(tri.points[2], triprojected.points[2], projmatrix);

         //if (normal.z < 0)
			if(normal.x * (tritranslated.points[0].x - vcamera.x) + 
			   normal.y * (tritranslated.points[0].y - vcamera.y) +
			   normal.z * (tritranslated.points[0].z - vcamera.z) < 0.0f)
			{
            MultiplyMatrixVector(tritranslated.points[0], triprojected.points[0], projmatrix);
            MultiplyMatrixVector(tritranslated.points[1], triprojected.points[1], projmatrix);
            MultiplyMatrixVector(tritranslated.points[2], triprojected.points[2], projmatrix);

            //scale The triangle
            //bring it to center as project garda it is not centered
            triprojected.points[0].x += 1.0f; triprojected.points[0].y += 1.0f;
            triprojected.points[1].x += 1.0f; triprojected.points[1].y += 1.0f;
            triprojected.points[2].x += 1.0f; triprojected.points[2].y += 1.0f;

            //scale it to window size
            triprojected.points[0].x *= 0.5f * windowWidth;
            triprojected.points[0].y *= 0.5f * windowHeight;
            triprojected.points[1].x *= 0.5f * windowWidth;
            triprojected.points[1].y *= 0.5f * windowHeight;
            triprojected.points[2].x *= 0.5f * windowWidth;
            triprojected.points[2].y *= 0.5f * windowHeight;
            

            //draw the triangle
            //floodFill(triprojected.points[0].x+9,triprojected.points[0].y+9,0, 2);
            FillTriangle(triprojected.points[0].x,triprojected.points[0].y,triprojected.points[1].x,triprojected.points[1].y,
             triprojected.points[2].x,triprojected.points[2].y);
			
         }
         }
         
      }

};




int main() {
   int gd = DETECT, gm;
   float aspectRatio;
   initgraph(&gd, &gm, "C:\\TC\\BGI");
   initwindow(1000,1000, "the cube");
   meshPopulate m1;
   for(int i =0 ; i< 50000000; i++){
   //while(true){
      m1.drawcycle();   
      cleardevice();
   }

   getch();
   closegraph();
   return 0;
}