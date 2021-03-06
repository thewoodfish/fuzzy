
#include <iostream>
#include <cstdlib>

using namespace std;

class Fuzzy {
	float bs, fp;
	char bs_fuzzy[7];
	float mem_val[2];
	
	char mem_word_0[4];
	char mem_word_1[4];

	float ev;

	// small ev
	float s_lt, s_lg, s_hg;

	// medium ev
	float m_lt, m_lg, m_hg;

	// large ev
	float l_lt, l_lg, l_hg;

	// small chest size
	float s_lr, s_ur;

	// medium chest size
	float m_lr, m_ur;

	// large chest size
	float l_lr, l_ur;

	// real values  of ev to use based on chest size
	float r_lt, r_lg, r_hg;

	// classes of chest size
	// char* divs[3];

	char div_0[10];
	char div_1[10];
	char div_2[10];

	void det_fuzzy_bs();
	float calc_triangle_membership(float lw, float up);
	float calc_trapezoid_membership(float lf, float uf, bool i);


public:
	Fuzzy(float x, float y) {
		bs = x;
		fp = y;
		
	}

	void initialize_hc(const char* x, const char* y, const char* z)
	{
		// initialize the higher classification
		// here its small, medium and large

		strcpy_s(div_0, sizeof div_0, x);
		strcpy_s(div_1, sizeof div_1, y);
		strcpy_s(div_2, sizeof div_2, z);


	}

	void set_ev_ranges(const char* t, float a, float b, float c);
	void set_bs_range(const char* t, float x, float y);
	void calc_mem_membership();
	void calc_ev();
};


inline void Fuzzy::set_bs_range(const char* s, float x, float y)
{

	// initialize ease value ranges
	if (!strcmp(s, div_0)) {
		s_lr = x;
		s_ur = y;
	}
	else if (!strcmp(s, div_1)) {
		m_lr = x;
		m_ur = y;
	}
	else if (!strcmp(s, div_2)) {
		l_lr = x;
		l_ur = y;
	}
	else {
		cout << "Error: first value is wrong";
		exit(1);
	}

}


void Fuzzy::set_ev_ranges(const char* s, float a, float b, float c)
{
	// initialize ease value ranges
	if (!strcmp(div_0, s)) {
		s_lt = a;
		s_lg = b;
		s_hg = c;
	}
	else if (!strcmp(div_1, s)) {
		m_lt = a;
		m_lg = b;
		m_hg = c;
	}
	else if (!strcmp(div_2, s)) {
		l_lt = a;
		l_lg = b;
		l_hg = c;
	}


	det_fuzzy_bs();
}

inline void Fuzzy::det_fuzzy_bs()
{
	// determine the body size
	if (bs >= s_lr && bs < s_ur) {
		strcpy_s(bs_fuzzy, sizeof bs_fuzzy, div_0);
		r_lt = s_lt;
		r_lg = s_lg;
		r_hg = s_hg;
	}
	else if (bs >= m_lr && bs < m_ur) {
		strcpy_s(bs_fuzzy, sizeof bs_fuzzy, div_1);
		r_lt = m_lt;
		r_lg = m_lg;
		r_hg = m_hg;
	}
	else if (bs >= l_lr && bs <= l_ur) {
		strcpy_s(bs_fuzzy, sizeof bs_fuzzy, div_2);
		r_lt = l_lt;
		r_lg = l_lg;
		r_hg = l_hg;
	}
	else {
		cout << r_lt << " " << r_lg << " " << r_hg << "\n";
		cout << "Your chest size is out of range!";
		exit(1);
	}

}



inline float Fuzzy::calc_triangle_membership(float lw, float up)
{
	float a = 0.0; // memebership value
	float mid = (lw + up) / 2;

	// use the upper & lower bounds to determine membership based on val

	if (fp > lw && fp < up) {
		if (fp == mid)
			a = 1.0;
		else if (fp > lw && fp <= mid)
			a = 1 - ((mid - fp) / (mid - lw));
		else if (fp > mid && fp < up)
			a = 1 - ((fp - mid) / (up - mid));
	}

	return a;
}

inline float Fuzzy::calc_trapezoid_membership(float lw, float up, bool i)
{
	float a = 0.0; // memebership value
	float mid = (lw + up) / 2;

	// determine if trapezoid is left | right sided
	if (i) {
		// right sided
		if (fp >= lw && fp <= mid)
			a = 1.0;
		else if (fp > mid && fp < up)
			a = ((fp - up) / (mid - up));
	}
	else {
		// left sided
		if (fp >= mid && fp <= up)
			a = 1.0;
		else if (fp > lw && fp < mid)
			a = ((fp - lw) / (mid - lw));
	}

	return a;
}

inline void Fuzzy::calc_mem_membership()
{

	float a = 0.0;
	// calculate memness values and also get its fuzzy value
	if (fp >= 0 && fp <= 20) {
		// falls in the full trapezoid range

		mem_val[0] = calc_trapezoid_membership(0, 40, true);
		strcpy_s(mem_word_0, sizeof mem_word_0, "VLF");
	}
	else if (fp >= 20 && fp <= 40) {
		// falls in the range of the trapezoid and triangle 

		// determine who will come first 

		mem_val[0] = calc_trapezoid_membership(0, 40, true);
		mem_val[1] = calc_triangle_membership(20, 60);

		strcpy_s(mem_word_0, sizeof mem_word_0, "VLF");
		strcpy_s(mem_word_1, sizeof mem_word_1, "SLF");
	}
	else if (fp >= 40 && fp <= 60) {
		// falls in the range of the traingle and triangle

		mem_val[0] = calc_triangle_membership(20, 60);
		mem_val[1] = calc_triangle_membership(40, 80);

		strcpy_s(mem_word_0, sizeof mem_word_0, "SLF");
		strcpy_s(mem_word_1, sizeof mem_word_1, "SCF");
	}
	else if (fp >= 60 && fp <= 80) {
		// falls in the range of triangle and trapezoid

		mem_val[0] = calc_trapezoid_membership(60, 100, false);
		mem_val[1] = calc_triangle_membership(40, 80);

		strcpy_s(mem_word_0, sizeof mem_word_0, "VCF");
		strcpy_s(mem_word_1, sizeof mem_word_1, "SCF");
	}
	else {
		// falls in the range of the trapezoid only

		mem_val[0] = calc_trapezoid_membership(60, 100, false);
		strcpy_s(mem_word_0, sizeof mem_word_0, "VCF");
	}
}

inline void Fuzzy::calc_ev()
{
	// calculate ease value

	float vlf_0 = r_hg * mem_val[0];
	float vlf_1 = r_hg * mem_val[1];

	float slf_0 = mem_val[0] * ((0.7 * r_lg) + (0.3 * r_hg));
	float slf_1 = mem_val[1] * ((0.7 * r_lg) + (0.3 * r_hg));


	float scf_0 = mem_val[0] * ((0.7 * r_lg) + (0.3 * r_lt));
	float scf_1 = mem_val[1] * ((0.7 * r_lg) + (0.3 * r_lt));

	float vcf_0 = r_lt * mem_val[0];
	float vcf_1 = r_lt * mem_val[1];

	float sum = mem_val[0] + mem_val[1];

	// establish fuzzy rules
	if (!strcmp(mem_word_0, "VLF") && !mem_val[1]) {
		ev = vlf_0;
		cout << "Your ease value will be a very loose fit.\n";
	}
	else if (!strcmp(mem_word_0, "VLF") && !strcmp(mem_word_1, "SLF")) {
		ev = (vlf_0 + slf_1) / sum;
		cout << "Your ease value will be between a very loose fit and a slightly loose fit.\n";
	}
	else if (!strcmp(mem_word_0, "SLF") && !strcmp(mem_word_1, "SCF")) {
		ev = (slf_0 + scf_1) / sum;
		cout << "Your ease value will be between a slightly loose fit and a slightly close fit.\n";
	}
	else if (!strcmp(mem_word_0, "VCF") && !strcmp(mem_word_1, "SCF")) {
		ev = (vcf_0 + scf_1) / sum;
		cout << "Your ease value will between a slightly close fit and a very close fit.\n";
	}
	else if (!strcmp(mem_word_0, "VCF") && !mem_val[1]) {
		ev = vcf_0;
		cout << "Your ease value will be a very close fit.\n";
	}

	cout << "Your final ease allowance value based on your data is " << ev << "cm.\n";

}


int main() 
{
	float chest_size, memness_percent;
	float lr = 29.7;
	float hr = 43.4;

	cout << "Please choose a chest size from " << lr << " to " << hr << ": ";
	cin >> chest_size;

	cout << "Please choose a fitness percentage from 0 - 100% (0 = 'very loose fit') and (100 = 'very close fit'): ";

	cin >> memness_percent;

	// initialize chest size and memness percent
	Fuzzy io(chest_size, memness_percent);

	// initiallize classifications
	io.initialize_hc("small", "medium", "large");

	// set chest size ranges 
	io.set_bs_range("small", 29.7, 34.3);
	io.set_bs_range("medium", 34.3, 38.8);
	io.set_bs_range("large", 38.8, 43.4);

	// set the ranges based on the classifications
	io.set_ev_ranges("small", 2.01, 5.87, 8.85);
	io.set_ev_ranges("medium", 4.67, 6.98, 10.5);
	io.set_ev_ranges("large", 3.71, 5.97, 6.8);

	io.calc_mem_membership();
	io.calc_ev();

	return 0;
}
