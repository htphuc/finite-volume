#include "Flux.h"

Flux::Flux() :  m_space_dimensions(2), m_solved_dimensions(1)
{
  m_evaluated_flux.resize(m_solved_dimensions);
  for(int i=0;i<m_solved_dimensions;i++) m_evaluated_flux[i].resize(m_space_dimensions);

  m_evaluated_flux_jacobian.resize(m_space_dimensions);
  for(int d=0;d<m_space_dimensions;d++)
    {
      m_evaluated_flux_jacobian[d].resize(m_solved_dimensions);
      for(int i=0;j<m_solved_dimensions;j++)
	m_evaluated_flux_jacobian[d][i].resize(m_solved_dimensions);
    }
}

Flux::Flux(int space_dimensions, int solved_dimensions) : m_space_dimensions(space_dimensions), m_solved_dimensions(solved_dimensions)
{
  m_evaluated_flux.resize(m_solved_dimensions);
  for(int i=0;i<m_solved_dimensions;i++) m_evaluated_flux[i].resize(m_space_dimensions);

  m_evaluated_flux_jacobian.resize(m_space_dimensions);
  for(int d=0;d<m_space_dimensions;d++)
    {
      m_evaluated_flux_jacobian[d].resize(m_solved_dimensions);
      for(int i=0;j<m_solved_dimensions;j++)
	m_evaluated_flux_jacobian[d][i].resize(m_solved_dimensions);
    }
}