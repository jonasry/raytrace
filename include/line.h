#ifndef __LINE_H__
#define __LINE_H__

#include "vector.h"

class CLine {

public:	// Constructors
	CLine();
	CLine(const CLine& l);
	CLine(const vector& d, const vector& l);
	CLine(const vector& d);


public:	// Services
	vector	Dir() const;
	vector	Loc() const;
	vector  PointAt(double t) const;
	void	Set(vector& d, vector& l);
	void	Move(double t);


private:
	vector	direction;
	vector	location;


};

inline CLine::CLine() {}
inline CLine::CLine(const CLine& l) { direction = l.Dir(); location = l.Loc(); }
inline CLine::CLine(const vector& d, const vector& l) { direction = !d; location = l; }
inline CLine::CLine(const vector& d) { direction = !d; }


inline vector CLine::Dir() const { return direction; }
inline vector CLine::Loc() const { return location ; }
inline vector CLine::PointAt(double t) const { return Loc() + t*Dir(); }
inline void	  CLine::Set(vector& d, vector& l) { direction=!d; location=l; }
inline void   CLine::Move(double t) { location=PointAt(t); }

#endif
