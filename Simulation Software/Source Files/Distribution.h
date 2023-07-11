#pragma once
#include "Utility.h"

class Visitor;

namespace Distributions
{
	constexpr auto EXPONENTIAL = "Exponential";
	constexpr auto UNIFORM = "Uniform";
	constexpr auto TRIANGULAR = "Triangular";
	constexpr auto NORMAL = "Normal";
	constexpr auto POISSON = "Poisson";
	constexpr auto CONSTANT = "Constant";
	constexpr auto WEIBULL = "Weibull";
	constexpr auto ERLANG = "Erlang";
}

class Distribution
{
public:
	Distribution();
	~Distribution();
	virtual double GetRV() = 0;
	virtual void Accept(Visitor& visitor) = 0;
	const char* GetType();

protected:
	double Uniform_0_1();
	void SetType(const char* type);

	const char* m_type;
};

class Exponential : public Distribution
{
public:
	Exponential(double mean);
	double GetRV();
	double GetMean();
	void Accept(Visitor& visitor) override;

private:
	double m_mean;
};

class Uniform : public Distribution
{
public:
	Uniform(double min, double max);
	double GetRV();
	double GetMin();
	double GetMax();
	void Accept(Visitor& visitor) override;

private:
	double m_min;
	double m_max;
};

class Triangular : public Distribution
{
public:
	Triangular(double min, double expected, double max);
	double GetRV();
	double GetMin();
	double GetExpected();
	double GetMax();
	void Accept(Visitor& visitor) override;

private:
	double m_min;
	double m_expected;
	double m_max;
	double fc;
	double term1;
	double term2;
};

class Normal : public Distribution
{
public:
	Normal(double mean, double stdev);
	double GetRV();
	double GetMean();
	double GetVariance();
	void Accept(Visitor& visitor) override;

private:
	bool m_isSavedRV;
	double m_savedRV;
	double m_mean;
	double m_variance;
};

class Poisson : public Distribution
{
public:
	Poisson(double mean);
	double GetRV();
	double GetMean();
	void Accept(Visitor& visitor) override;

private:
	double m_mean;
};

class Constant : public Distribution
{
public:
	Constant(double mean);
	double GetRV();
	double GetMean();
	void Accept(Visitor& visitor) override;

private:
	double m_mean;
};

class Weibull : public Distribution
{
public:
	Weibull(double scale, double shape);
	double GetRV();
	double GetScale();
	double GetShape();
	void Accept(Visitor& visitor) override;

private:
	double m_scale;
	double m_shape;
};

class Erlang : public Distribution
{
public:
	Erlang(int scale, double shape);
	double GetRV();
	double GetScale();
	double GetShape();
	void Accept(Visitor& visitor) override;

private:
	int m_scale;
	double m_shape;
};