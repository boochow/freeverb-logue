// Comb filter class declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _comb_
#define _comb_

#include "denormals.h"

class comb
{
public:
					comb();
			void	setbuffer(float *buf, int size);
	inline  float	process(float inp);
			void	mute();
			void	setdamp(float val);
			float	getdamp();
			void	setfeedback(float val);
			float	getfeedback();
	float	*buffer;
private:
	float	feedback;
	float	filterstore;
	float	damp1;
	float	damp2;
	int		bufsize;
	int		bufidx;
};


// Big to inline - but crucial for speed

inline float comb::process(float input)
{
	float output;

	output = buffer[bufidx];
	undenormalise(output);

	filterstore = (output*damp2) + (filterstore*damp1);
	undenormalise(filterstore);

	buffer[bufidx] = input + (filterstore*feedback);

	if(++bufidx>=bufsize) bufidx = 0;

	return output;
}

#endif //_comb_

//ends
