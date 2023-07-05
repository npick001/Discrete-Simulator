#include "Distribution.h"
#include <stdlib.h>
#include <math.h>

Distribution::Distribution() {}


const char* Distribution::GetType()
{
	return m_type;
}

double Distribution::Uniform_0_1()
{
	return (((double)rand()) / (((double)RAND_MAX) + 1.0));
}

void Distribution::SetType(const char* type)
{
	m_type = type;
}

Exponential::Exponential(double mean) : Distribution()
{
	SetType(Distributions::EXPONENTIAL);

	m_mean = mean;
}

double Exponential::GetRV()
{
	return  -m_mean*log(Uniform_0_1());
}

double Exponential::GetMean()
{
	return m_mean;
}

void Exponential::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

Uniform::Uniform(double min, double max) : Distribution()
{
	SetType(Distributions::UNIFORM);

	m_min = min;
	m_max = max;
}

double Uniform::GetRV()
{
	return ((m_max - m_min)*Uniform_0_1() + m_min);
}

double Uniform::GetMin()
{
	return m_min;
}

double Uniform::GetMax()
{
	return m_max;
}

void Uniform::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

Triangular::Triangular(double min, double expected, double max) : Distribution()
{
	SetType(Distributions::TRIANGULAR);

	m_min = min;
	m_max = expected;
	m_expected = max;
	fc = (m_max - m_min) / (m_expected - m_min);
	term1 = (m_expected - m_min)*(m_max - m_min);
	term2 = (m_expected - m_min)*(m_expected - m_max);
}

double Triangular::GetRV()
{
	double u = Uniform_0_1();
	double x;
	if (u < fc)
		x = m_min + sqrt(u*term1);
	else
		x = m_expected - sqrt((1 - u)*term2);
	return x;
}

double Triangular::GetMin()
{
	return m_min;
}

double Triangular::GetExpected()
{
	return m_expected;
}

double Triangular::GetMax()
{
	return m_max;
}

void Triangular::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

Normal::Normal(double mean, double stdev)
{
	SetType(Distributions::NORMAL);

	m_mean = mean;
	m_variance = stdev*stdev;
	m_isSavedRV = false;
}

#define TWO_PI 6.2831853071795864769252866

double Normal::GetRV()
{
	if (m_isSavedRV) {
		m_isSavedRV = false;
		return m_savedRV;
	}
	else {
		m_isSavedRV = true;
		double u1 = sqrt(-2.0 * log(Uniform_0_1()));
		double u2 = TWO_PI*Uniform_0_1();
		m_savedRV = u1*sin(u2);
		return(u1*cos(u2));
	}
}

double Normal::GetMean()
{
	return m_mean;
}

double Normal::GetVariance()
{
	return m_variance;
}

void Normal::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

Poisson::Poisson(double mean)
{
	SetType(Distributions::POISSON);

	m_mean = mean;
}

double Poisson::GetRV()
{
	double sum = 0.0;
	int i;
	for (i = -1; sum <= m_mean; i++) {
		sum += -log(Uniform_0_1());
	}
	return (double)i;
}

double Poisson::GetMean()
{
	return m_mean;
}

void Poisson::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

Constant::Constant(double mean)
{
	SetType(Distributions::CONSTANT);

	m_mean = mean;
}

double Constant::GetRV()
{
	return m_mean;
}

double Constant::GetMean()
{
	return m_mean;
}

void Constant::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

Weibull::Weibull(double scale, double shape)
{
	SetType(Distributions::WEIBULL);

	m_scale = scale;
	m_shape = shape;
}

double Weibull::GetRV()
{
	return(m_scale*pow(-log(Uniform_0_1()), 1.0 / m_shape));
}

double Weibull::GetScale()
{
	return m_scale;
}

double Weibull::GetShape()
{
	return m_shape;
}

void Weibull::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}

Erlang::Erlang(int scale, double shape)
{
	SetType(Distributions::ERLANG);

	m_scale = scale;
	m_shape = shape;
}

double Erlang::GetRV()
{
	double rv = 1.0;
	for (int i = 1; i <= m_scale; i++) {
		rv *= Uniform_0_1();
	}
	return -m_shape*log(rv);
}

double Erlang::GetScale()
{
	return m_scale;
}

double Erlang::GetShape()
{
	return m_shape;
}

void Erlang::Accept(Visitor& visitor)
{
	return visitor.Visit(*this);
}
