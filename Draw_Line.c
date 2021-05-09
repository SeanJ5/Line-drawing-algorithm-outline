
typedef struct Point
{
	int x, y;
} Point;


/* Solve for the x or y coordinate of a point that lies along a line */
// If change in x is 0 -> returns y pos of first point
// If change in y is 0 -> returns x pos of first point
int Interpolate(Point* p0, Point* p1, 
				char c /* 'x' to solve for x, else solve for y */,
				int v  /* the known value for the point being solved */)
{
	if(c == 'x' || c == 'X')
	{
		if(p1->x - p0->x != 0) return p0->y + (v - p0->x) * (p1->y - p0->y) / (p1->x - p0->x);
		else return p0->y;
	}
	else if(p1->y - p0->y != 0) return p0->x + (v - p0->y) * (p1->x - p0->x) / (p1->y - p0->y);
	return p0->x;
}


void DrawLine(GFX* g /* graphics struct or some sort */, Point p0, Point p1)
{
	int *plotX, *plotY;	  // Points to int to draw on each axis
	signed char dir;	    // Increase in what dir.?
	signed char bx, by;	  // Used for check if points are on screen

	int dx, dy;		// Change in x and y
	int x, y;		  // x and y axis
	int t;			  // Used for swapping
	int step;		  // Keep track of change

	dx = abs(p1.x - p0.x);	// Change in x
	dy = abs(p1.y - p0.y);	// Change in y

	plotX = &x;		// Draw x on x axis
	plotY = &y;		// Draw y on y axis

	// Check is points are on screen and adjust if needed
	#define I Interpolate
	#define w g->width		// width  of gfx raster
	#define h g->height		// height of gfx raster
	#define bounds(pA, pB) \
	if(pA.x < 0)       { pA.y = I(&pA, &pB, 'x', 0);   pA.x = 0;   bx--; } \
	else if(pA.x >= w) { pA.y = I(&pA, &pB, 'x', w-1); pA.x = w-1; bx++; } \
	if(pA.y < 0)       { pA.x = I(&pA, &pB, 'y', 0);   pA.y = 0;   by--; } \
	else if(pA.y >= h) { pA.x = I(&pA, &pB, 'y', h-1); pA.y = h-1; by++; }

	bx = by = 0;
	bounds(p0, p1)	// Check p0
	bounds(p1, p0)	// Check p1

	if(bx == 2 || bx == -2) return;
	if(by == 2 || by == -2) return;

	if(dy > dx)
	{
		t = p0.x; p0.x = p0.y; p0.y = t;	  // Swap x and y (p0)
		t = p1.x; p1.x = p1.y; p1.y = t;	  // Swap x and y (p1)
		t = dx; dx = dy; dy = t;			      // Swap dx and dy
		plotX = &y; plotY = &x;				      // Draw y on x axis and x on y axis
	}

	if(p0.x > p1.x)
	{
		t = p0.x; p0.x = p1.x; p1.x = t;	// Swap x coordinates
		t = p0.y; p0.y = p1.y; p1.y = t;	// Swap y coordinates
	}

	dir = (p0.y > p1.y) ? -1 : 1;
	step = 0;
	y = p0.y;

	// Draw!
	for(x = p0.x; x <= p1.x; x++)
	{
		g->pixels[*plotX + *plotY * g->width] = g->drawColor;

		step += dy;
		if(step >= dx) { step -= dx; y += dir; }
	}
}
