/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include "CMRGeometry.h"
#include "CMRDebug.h"
#include "CMRCommon.h"

/*******************  FUNCTION  *********************/
CMRVect2D::CMRVect2D ( int x, int y )
{
	this->x = x;
	this->y = y;
}

/*******************  FUNCTION  *********************/
CMRVect2D& CMRVect2D::operator+= ( const CMRVect2D& vect )
{
	x += vect.x;
	y += vect.y;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRVect2D::operator+ ( const CMRVect2D & vect ) const
{
	CMRVect2D tmp(*this);
	return (tmp+=vect);
}

/*******************  FUNCTION  *********************/
CMRVect2D& CMRVect2D::operator-= ( const CMRVect2D& vect )
{
	x -= vect.x;
	y -= vect.y;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRVect2D::operator- ( const CMRVect2D& vect ) const
{
	CMRVect2D tmp(*this);
	return (tmp-=vect);
}

/*******************  FUNCTION  *********************/
void CMRVect2D::set ( int x, int y )
{
	this->x = x;
	this->y = y;
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRVect2D::getRel(int dx, int dy) const
{
	return CMRVect2D(x + dx,y + dy);
}

/*******************  FUNCTION  *********************/
std::ostream& operator<< ( std::ostream& out, const CMRVect2D& vect )
{
	char buffer[256];
	int res = sprintf(buffer,"CMRVect2D [ %5d , %5d ]",vect.x,vect.y);
	assert(res < 256);
	out << buffer;
	return out;
}

/*******************  FUNCTION  *********************/
bool operator== ( const CMRVect2D& v1, const CMRVect2D& v2 )
{
	return ((v1.x == v2.x) && (v1.y == v2.y));
}

/*******************  FUNCTION  *********************/
CMRRect::CMRRect ( int x, int y, int width, int height )
{
	//errors
	assert(width >= 0);
	assert(height >= 0);

	//set default
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

/*******************  FUNCTION  *********************/
bool CMRRect::contains ( const CMRRect& rect, bool partially ) const
{
	if (partially)
	{
		//check if ! outside
		return !(rect.x >= x + width || rect.x + rect.width <= x || rect.y >= y + height || rect.y + rect.height <= y); 
	} else {
		return (rect.x >= this->x && rect.x + rect.width <= this->x + this->width)
			&& (rect.y >= this->y && rect.y + rect.height <= this->y + this->height);
	}
}

/*******************  FUNCTION  *********************/
bool CMRRect::contains ( const CMRVect2D& point ) const
{
	return (point.x >= x && point.x < x + width) && (point.y >= y && point.y < y + height);
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRRect::point1 ( void ) const
{
	return CMRVect2D(x,y);
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRRect::point2 ( void ) const
{
	return CMRVect2D(x+width,y+height);
}

/*******************  FUNCTION  *********************/
std::ostream& operator<< ( std::ostream& out, const CMRRect& rect )
{
	char buffer[256];
	int res = sprintf(buffer,"CMRRect2D [ %5d , %5d , %5d , %5d ]",rect.x,rect.y,rect.width,rect.height);
	assert(res < 256);
	out << buffer;
	return out;
}

/*******************  FUNCTION  *********************/
bool operator== ( const CMRRect& rect1, const CMRRect& rect2 )
{
	return (rect1.x == rect2.x)
		&& (rect1.y == rect2.y)
		&& (rect1.width == rect2.width)
		&& (rect1.height == rect2.height);
}

/*******************  FUNCTION  *********************/
void CMRRect::set ( int x, int y, int width, int height )
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

/*******************  FUNCTION  *********************/
long unsigned int CMRRect::surface ( void ) const
{
	return width*height;
}

/*******************  FUNCTION  *********************/
CMRRect& CMRRect::operator+= ( const CMRVect2D& vect )
{
	x += vect.x;
	y += vect.y;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRRect CMRRect::operator+ ( const CMRVect2D& vect ) const
{
	CMRRect tmp(*this);
	tmp+=vect;
	return tmp;
}

/*******************  FUNCTION  *********************/
CMRRect& CMRRect::expend ( int depth )
{
	//errors
	assert(depth >= - width / 2 && depth >= -height / 2);

	x -= depth;
	y -= depth;
	width += 2 * depth;
	height += 2 * depth;
	
	return *this;
}

/*******************  FUNCTION  *********************/
CMRRect CMRRect::expended ( int depth ) const
{
	CMRRect rect(*this);
	return rect.expend(depth);
}

/*******************  FUNCTION  *********************/
CMRRect CMRRect::relativeTo ( const CMRRect& rect ) const
{
	assert(rect.contains(*this));
	CMRRect res(*this);
	res.x -= rect.x;
	res.y -= rect.y;
	assert(res.x >= 0 && res.y >=0);
	return res;
}

/*******************  FUNCTION  *********************/
CMRRect CMRRect::relativeTo (const CMRVect2D& vect ) const
{
	assert(this->x >= vect.x && this->y >= vect.y);
	CMRRect res(*this);
	res.x -= vect.x;
	res.y -= vect.y;
	assert(res.x >= 0 && res.y >=0);
	return res;
}

/*******************  FUNCTION  *********************/
CMRRect CMRRect::getBorder ( CMRDirections direction, int depth, bool keepCorner )
{
	//var
	CMRRect res(*this);

	//errors
	assert(depth > 0);

	//cases
	switch (direction)
	{
		case CMR_TOP:
			res.height = depth;
			break;
		case CMR_BOTTOM:
			res.y += res.height - depth;
			res.height = depth;
			break;
		case CMR_LEFT:
			res.width = depth;
			break;
		case CMR_RIGHT:
			res.x += res.width - depth;
			res.width = depth;
			break;
		default:
			fatal("Invalid direction value : %d !",direction);
			break;
	}
	
	//remove corner if need
	if (keepCorner == false)
	{
		if (direction == CMR_TOP || direction == CMR_BOTTOM)
		{
			res.x += depth;
			res.width -= 2*depth;
		} else {
			res.y += depth;
			res.height -= 2*depth;
		}
	}
	
	//check
	assert(contains(res));
	
	return res;
}

/*******************  FUNCTION  *********************/
CMRRect CMRRect::intersect ( const CMRRect& rect ) const
{
	//check if at least partially in
	int x1 = cmrMax(x,rect.x);
	int y1 = cmrMax(y,rect.y);
	int x2 = cmrMin(x+width,rect.x+rect.width);
	int y2 = cmrMin(y+height,rect.y+rect.height);
	
	//if overlap, compute size
	if (x1 < x2 && y1 < y2)
		return CMRRect(x1,y1,x2-x1,y2-y1);
	else
		return CMRRect(0,0,0,0);
}
