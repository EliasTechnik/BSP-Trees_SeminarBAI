#pragma once

class d {//Parent of all dimensions
private:
	double x;
public:
	d(double _x) {
		x = _x;
	}
	bool isInfront(plane p);
	bool isBehind(plane p) {
		return !this->isInfront(p);
	}
};

class plane {
	d origin;
	d 
};



class d1{
private:
	double x;
public:
	d1(double _x) {

	}
	
}