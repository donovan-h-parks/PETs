//=======================================================================
// Author: Donovan Parks
//
// Copyright 2013 Donovan Parks (donovan.parks@gmail.com)
//
// This file is part of PETs.
//
// PETs is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// PETs is distributed in the hope that it will be useful, bu
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with PETs. If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#include "Precompiled.hpp"

#include "PCoA.hpp"

void PCoA::project(const Matrix& distMatrix)
{
	if(distMatrix.size() == 0)
		return;

	m_projectedData = distMatrix;

	// Center squared distance matrix

	// Step 1: Square entries and divide by -2.0
	for(uint r = 0; r < m_projectedData.size(); ++r)
	{
		for(uint c = 0; c < m_projectedData.size(); ++c)
		{			
			m_projectedData[r][c] = m_projectedData.at(r).at(c)*m_projectedData.at(r).at(c) / -2.0;
		}
	}

	// Step 2: Calculate column and row means
	std::vector<double> colMeans(m_projectedData.size(), 0);
	std::vector<double> rowMeans(m_projectedData.size(), 0);
	for(uint r = 0; r < m_projectedData.size(); ++r)
	{
		for(uint c = 0; c < m_projectedData.size(); ++c)
		{			
			rowMeans[r] += m_projectedData.at(r).at(c);			
			colMeans[c] += m_projectedData.at(r).at(c);
		}
	}

	for(uint i = 0; i < m_projectedData.size(); ++i)
	{
		rowMeans[i] /= m_projectedData.size();
		colMeans[i] /= m_projectedData.size();
	}

	// Step 3: Calculate matrix mean
	double matrixMean = 0;
	for(uint i = 0; i < m_projectedData.size(); ++i)
		matrixMean += rowMeans.at(i);
	matrixMean /= m_projectedData.size();

	// Step 4: Calculate actual "centered" distance matrix
	for(uint r = 0; r < m_projectedData.size(); ++r)
	{
		for(uint c = 0; c < m_projectedData.size(); ++c)
		{		
			m_projectedData[r][c] = m_projectedData.at(r).at(c) - rowMeans.at(r) - colMeans.at(c) + matrixMean;
		}
	}

	// Step 5: Calculate eigenvectors and eigenvalues
	double* M = new double[m_projectedData.size() * m_projectedData.size()];	// input matrix
	double* U = new double[m_projectedData.size() * m_projectedData.size()];	// eigenvectors
	double* D = new double[m_projectedData.size()];	// real component of eigenvalues
	double* E = new double[m_projectedData.size()];	// imaginary component of eigenvalues

	for(uint r = 0; r < m_projectedData.size(); ++r)
	{
		for(uint c = 0; c < m_projectedData.size(); ++c)
		{	
			M[c+r*m_projectedData.size()] = m_projectedData.at(r).at(c);
		}
	}

	EigDecomp(M, U, D, E, m_projectedData.size());

	// Check for negative eignvalues
	for(uint i = 0; i < m_projectedData.size(); ++i)
	{
		if(D[i] < 0)
		{
			std::cout << "[Warning] Negative eigenvectors in PCoA calculation." << std::endl;
			D[i] = -D[i];
			for(uint j = 0; j < m_projectedData.size(); ++j)
				U[i + j*m_projectedData.size()] = -U[i + j*m_projectedData.size()];
		}
	}

	// Step 6: Find desending order of eigenvalues
	for(uint i = 0; i < m_projectedData.size(); ++i)
		D[i] = fabs(D[i]);

	m_eigenValues.clear();
	m_sumEigenvalues = 0;
	for(uint i = 0; i < m_projectedData.size(); ++i)
	{
		m_sumEigenvalues += D[i];
		m_eigenValues.push_back(D[i]);
	}

	std::sort(m_eigenValues.begin(), m_eigenValues.end(), std::greater<double>());

	std::vector<uint> indexOrder;
	for(uint i = 0; i < m_projectedData.size(); ++i)
	{
		bool bBreak = false;
		for(uint j = 0; j < m_projectedData.size() && !bBreak; ++j)
		{
			if(m_eigenValues.at(i) == D[j])
			{
				indexOrder.push_back(j);
				bBreak = true;
			}
		}
	}

	// Step 7: Calculate variance and summed variance for each embedded dimension
	m_variance.clear();
	m_sumVariance.clear();
	for(uint i = 0; i < m_projectedData.size(); ++i)
	{
		m_variance.push_back(m_eigenValues.at(i) / m_sumEigenvalues);

		if(i == 0)
			m_sumVariance.push_back(m_variance.at(0));
		else
			m_sumVariance.push_back(m_sumVariance.at(i-1) + m_variance.at(i));
	}

	// Step 8: Transform eigenvector matrix into new embedded space
	for(uint i = 0; i < m_projectedData.size(); ++i)
	{
		D[i] = sqrt(D[i]);
		for(uint r = 0; r < m_projectedData.size(); ++r)
		{
			for(uint c = 0; c < m_projectedData.size(); ++c)
			{		
				m_projectedData[r][c] = U[indexOrder.at(c)+r*m_projectedData.size()]*D[indexOrder.at(c)];
			}
		}
	}

	delete[] M;
	delete[] U;
	delete[] D;
	delete[] E;
}

///////////////////////////////////////////////////////////////////////////////
//
//	Computes m_eigenValues and eigenvectors of a real, symmetric
//	matrix. 
//
//	A matrix can be decomposed as A = U*D*U', where the eigenvalue matrix D is
//	diagonal and the eigenvector matrix U is orthogonal. That is,
//	the diagonal values of D are the m_eigenValues, and
//	U*U' = I, where I is the identity matrix.  The columns of U 
//	represent the eigenvectors in the sense that A*U = V*U.
//   
//	(Adapted from JAMA, a Java Matrix Library, developed by jointly 
//	by the Mathworks and NIST; see  http://math.nist.gov/javanumerics/jama).
//
//  Modified by: Donovan Parks, August 2007 (donovan.parks@gmail.com)
///////////////////////////////////////////////////////////////////////////////
EigDecomp::EigDecomp(double* A, double* U, double* D, double* E, int rows) 
{
  m_rows = rows;
  m_U = U;
  m_D = D;
	m_E = E;

	for(int j = 0; j < m_rows; ++j)
	{
		for(int i = 0; i < m_rows; ++i)
		{
			m_U[i+j*m_rows] = A[i+j*m_rows];
		}
	}

	// Tridiagonalize.
	tred2();

	// Diagonalize.
	tql2();
}

// Symmetric Householder reduction to tridiagonal form.
void EigDecomp::tred2() 
{
	//  This is derived from the Algol procedures tred2 by
	//  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
	//  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
	//  Fortran subroutine in EISPACK.

	for (int j = 0; j < m_rows; j++) 
	{
		 m_D[j] = m_U[(m_rows-1)*m_rows+j];
	}

	// Householder reduction to tridiagonal form.
	for (int i = m_rows-1; i > 0; i--) 
	{ 
		 // Scale to avoid under/overflow.

		double scale = 0.0;
		double h = 0.0;
		for (int k = 0; k < i; k++) 
		{
			scale = scale + fabs(m_D[k]);
		}

		if (scale == 0.0) 
		{
			m_E[i] = m_D[i-1];
			for (int j = 0; j < i; j++) 
			{
				 m_D[j] = m_U[(i-1)*m_rows+j];
				 m_U[i*m_rows+j] = 0.0;
				 m_U[j*m_rows+i] = 0.0;
			}
		} 
		else 
		{
			// Generate Householder vector.
			for (int k = 0; k < i; k++) 
			{
				 m_D[k] /= scale;
				 h += m_D[k] * m_D[k];
			}

			double f = m_D[i-1];
			double g = sqrt(h);
			if (f > 0) 
			{
				 g = -g;
			}
			m_E[i] = scale * g;
			h = h - f * g;
			m_D[i-1] = f - g;

			for (int j = 0; j < i; j++) 
			{
				 m_E[j] = 0.0;
			}

			// Apply similarity transformation to remaining columns.
			for (int j = 0; j < i; j++) 
			{
				f = m_D[j];
				m_U[j*m_rows+i] = f;
				g = m_E[j] + m_U[j*m_rows+j] * f;
			
				for (int k = j+1; k <= i-1; k++) 
				{
					g += m_U[k*m_rows+j] * m_D[k];
					m_E[k] += m_U[k*m_rows+j]* f;
				}

				m_E[j] = g;
			}

			f = 0.0;
			for (int j = 0; j < i; j++) 
			{
				m_E[j] /= h;
				f += m_E[j] * m_D[j];
			}

			double hh = f / (h + h);
			for (int j = 0; j < i; j++) 
			{
				m_E[j] -= hh * m_D[j];
			}

			for (int j = 0; j < i; j++) 
			{
				f = m_D[j];
				g = m_E[j];

				for (int k = j; k <= i-1; k++) 
				{
					m_U[k*m_rows+j] -= (f * m_E[k] + g * m_D[k]);
				}

				m_D[j] = m_U[(i-1)*m_rows+j];
				m_U[i*m_rows+j] = 0.0;
			}
		}	

		m_D[i] = h;
	}
 
  // Accumulate transformations. 
  for (int i = 0; i < m_rows-1; i++) 
	{
		m_U[(m_rows-1)*m_rows+i] = m_U[i*m_rows+i];
		m_U[i*m_rows+i] = 1.0;
		double h = m_D[i+1];
		if (h != 0.0) 
		{
			for (int k = 0; k <= i; k++) 
			{
				m_D[k] = m_U[k*m_rows+(i+1)] / h;
			}

			for (int j = 0; j <= i; j++) 
			{
				double g = 0.0;
				for (int k = 0; k <= i; k++) 
				{
					g += m_U[k*m_rows+(i+1)] * m_U[k*m_rows+j];
				}

				for (int k = 0; k <= i; k++) 
				{
					m_U[k*m_rows+j] -= g * m_D[k];
				}
			}
     }

     for (int k = 0; k <= i; k++) 
		 {
				m_U[k*m_rows+(i+1)] = 0.0;
     }
  }

  for (int j = 0; j < m_rows; j++) 
	{
     m_D[j] = m_U[(m_rows-1)*m_rows+j];
     m_U[(m_rows-1)*m_rows+j] = 0.0;
  }

  m_U[(m_rows-1)*m_rows+(m_rows-1)] = 1.0;
  m_E[0] = 0.0;
} 

// Symmetric tridiagonal QL algorithm.
void EigDecomp::tql2 () 
{

//  This is derived from the Algol procedures tql2, by
//  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
//  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
//  Fortran subroutine in EISPACK.
 
	for (int i = 1; i < m_rows; i++) 
	{
		m_E[i-1] = m_E[i];
	}
	m_E[m_rows-1] = 0.0;

	double f = 0.0;
	double tst1 = 0.0;
	double eps = pow(2.0,-52.0);
	for (int l = 0; l < m_rows; l++) 
	{
		// Find small subdiagonal element
		tst1 = std::max(tst1,fabs(m_D[l]) + fabs(m_E[l]));
		int m = l;

		// Original while-loop from Java code
		while (m < m_rows) 
		{
			if (fabs(m_E[m]) <= eps*tst1) 
			{
				 break;
			}
			m++;
		}
 
		// If m == l, d[l] is an eigenvalue,
		// otherwise, iterate. 
		if (m > l) 
		{
			int iter = 0;
			do 
			{
				iter = iter + 1;  // (Could check iteration count here.)

				// Compute implicit shift
				double g = m_D[l];
				double p = (m_D[l+1] - g) / (2.0 * m_E[l]);
				double r = hypot(p,1.0);
				if (p < 0) 
				{
					r = -r;
				}
				m_D[l] = m_E[l] / (p + r);
				m_D[l+1] = m_E[l] * (p + r);
				double dl1 = m_D[l+1];
				double h = g - m_D[l];
				for (int i = l+2; i < m_rows; i++) 
				{
					m_D[i] -= h;
				}
				f = f + h;

				// Implicit QL transformation.
				p = m_D[m];
				double c = 1.0;
				double c2 = c;
				double c3 = c;
				double el1 = m_E[l+1];
				double s = 0.0;
				double s2 = 0.0;
				for (int i = m-1; i >= l; i--) 
				{
					c3 = c2;
					c2 = c;
					s2 = s;
					g = c * m_E[i];
					h = c * p;
					r = hypot(p,m_E[i]);
					m_E[i+1] = s * r;
					s = m_E[i] / r;
					c = p / r;
					p = c * m_D[i] - s * g;
					m_D[i+1] = h + s * (c * g + s * m_D[i]);

					// Accumulate transformation.
					for (int k = 0; k < m_rows; k++) 
					{
						 h = m_U[k*m_rows+(i+1)];
						 m_U[k*m_rows+(i+1)] = s * m_U[k*m_rows+i] + c * h;
						 m_U[k*m_rows+i] = c * m_U[k*m_rows+i] - s * h;
					}
				 }

				 p = -s * s2 * c3 * el1 * m_E[l] / dl1;
				 m_E[l] = s * p;
				 m_D[l] = c * p;

				 // Check for convergence.
			} while (fabs(m_E[l]) > eps*tst1);
		}

		m_D[l] = m_D[l] + f;
		m_E[l] = 0.0;
	}
}

void PCoA::print(const std::string& filename)
{
	std::ofstream fout(filename.c_str());
	if(!fout.is_open())
	{
		std::cout << "[Error] Unable to create file: " << + filename.c_str() << std::endl;
		return;
	}

	// write projected data
	fout << "Projected data (each column is a point): " << std::endl;
	for(uint i = 0; i < m_projectedData.at(0).size(); ++i)
	{
		for(uint ptIndex = 0; ptIndex < m_projectedData.size(); ++ptIndex)
		{
			fout << m_projectedData.at(ptIndex).at(i) << '\t';
		}
		fout << std::endl;
	}

	// write captured variance
	fout << std::endl << "Captured variance for each dimension: " << std::endl;
	for(uint i = 0; i < m_variance.size(); ++i)
		fout << m_variance.at(i) << std::endl;

	fout.close();
}