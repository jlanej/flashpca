#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <Eigen/Sparse>

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iomanip>

#include <time.h>
#include <sys/time.h>

#define VAR_TOL 1e-9
#define STANDARDIZE_SD 1
#define STANDARDIZE_BINOMIAL 2

using namespace Eigen;

typedef SparseMatrix<double> SMd;

typedef Array<bool, Dynamic, Dynamic> ArrayXXb;
typedef Array<bool, Dynamic, 1> ArrayXb;


template <typename Derived>
void save(const char *filename, const MatrixBase<Derived>& m)
{
   unsigned int rows = m.rows(), cols = m.cols();
   std::ofstream f(filename, std::ios::out | std::ios::binary);
   f.write((char *)&rows, sizeof(rows));
   f.write((char *)&cols, sizeof(cols));
   f.write((char *)m.derived().data(), sizeof(typename Derived::Scalar) * rows * cols);
   f.close();
}

template <typename Derived>
void load(const char *filename, MatrixBase<Derived>& m)
{
   int nrows, ncols;
   std::ifstream f(filename, std::ios::binary);
   f.read(reinterpret_cast<char*>(&nrows), sizeof(int));
   f.read(reinterpret_cast<char*>(&ncols), sizeof(int));
   m.derived().resize(nrows, ncols);
   f.read((char*)m.derived().data(), sizeof(typename Derived::Scalar) * nrows * ncols);
}

template <typename Derived>
bool save_text(const char *filename, MatrixBase<Derived>& m,
   unsigned int precision=6)
{
   std::ofstream out(filename, std::ofstream::out);
   out << std::setprecision(precision);
   if(!out)
   {
      std::cerr << "Error while saving to file " << filename 
	 << ":" << strerror(errno) << std::endl;
      return false;
   }
   out << m << std::endl;
   out.close();
}

template <typename Derived>
std::string dim(MatrixBase<Derived>& m)
{
   std::stringstream ss;
   ss << m.rows() << " x " << m.cols();
   return ss.str();
}

// saves a compressed column sparse matrix
//
// dim: <nrow, ncol> of type int
// nonzero: of type int
// values: of type double
// inner indices: 
void save_sparse(const char *filename, const SparseMatrix<double>& m,
   bool beta_zerobased);

void usage();
double myatof(char* c);
std::string timestamp();

template <typename T>
int sign(T x)
{
   return (T(0) < x) - (x < T(0));
}

double sign_scalar(double x);
MatrixXd read_bed(const char *filename, const unsigned int nrows);
MatrixXd read_pheno(const char *filename, unsigned int firstcol);
MatrixXd standardize(const MatrixXd &X, bool scale=true,
   int method=STANDARDIZE_SD);
VectorXd zapsmall(const VectorXd& X, const double tol);
double maxabs(const SMd& B);
MatrixXd subsample_snps(const MatrixXd& X, double prop);

