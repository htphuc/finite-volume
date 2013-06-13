#include <iostream>
#include <fstream>
#include <math.h>
#include <limits.h>
#include "Vector.h"
#include "Flux.h"
#include "ZeroFlux.h"
#include "BurgersFlux1D.h"
#include "NSFlux.h"
#include "Flume2DConvectionFlux.h"
#include "Flume2DConvectionFluxNoVel.h"
#include "Flume2DSource.h"
#include "Equation.h"
#include "FD1Solver.h"
#include "timeSolver.h"
#include "EulerSolver.h"
//#include "RK2Solver.h"
#include "RK3Solver.h"
#include "ScalarField.h"
#include "PeriodicField.h"
#include "NullField.h"
#include "PrescribedField.h"
#include "WriteVectorField.h"

// Flume pb
//#include "Flume3D.h"
#include "Flume2D.h"

using namespace std;
int main()
{
  int dim = 2;
  Vector<double> dx(dim,0.05); dx[1] = 0.01; Vector<double> xI(dim); xI[0]=10; xI[1]=1; Vector<double> llc(dim,0); llc[0]=-xI[0];
  Flux *ptrCF = new Flume2DConvectionFluxNoVel();
  Flux *ptrDF = new ZeroFlux(2,1);
  Flux *ptrS = new Flume2DSource();
  Equation *eq = new Equation(ptrCF, ptrDF, ptrS); //don't forget to set the sr and velocity somewhere!!
  FD1Solver sol(dx, xI, eq, llc);

  Vector<int> xr (dim); for(int d=0;d<dim;d++) xr[d] = xI[d]/dx[d];
  VectorField pos = sol.get_position();

  //Sets initial fields 
  VectorField phi(1, SField (xr));
  SField bound(xr);
  VectorField u0(dim, SField (xr));
  SField dvdy0 (xr);
  for(int it=0;it<phi[0].get_size();++it)
    {
      phi[0][it] = phi0(pos[0][it], pos[1][it]);
      bound[it] = boundary(pos[0][it], pos[1][it]);
      u0[0][it] = u(pos[0][it], pos[1][it]);
      u0[1][it] = w(pos[0][it], pos[1][it]);
      dvdy0[it] = dvdy(pos[0][it], pos[1][it]);
    }
  // the velocity field is null outside the flow, as well as the sr
  u0 = bound*u0; //phi[0] = bound*phi[0];
  SField sr = 0.035*bound; ptrCF->set_parameter(sr); ptrS->set_parameter(dvdy0);

  // // specifies the value of the solved field on the surface enclosing the intergration domain (Dirichlet conditions)
  
  // ScalarField phiWest (xr.drop(0)); 
  // for(int it=0;it<phiWest.get_size();++it) phiWest[it] = 1;//phi0( llc[0]-dx[0] , pos[1][it] );
  // phi[0].set_bound(0, -1, phiWest);
  // ScalarField phiSouth (xr.drop(1));
  // for(int it=0;it<phiSouth.get_size();++it) phiSouth[it] = 1; //phi0( pos[0][it], llc[1]-dx[1] );
  // phi[0].set_bound(1, -1, phiSouth);
  // //phi is null at the East and North boundary surfaces, and the velocity field is null at the boundaries
  // ScalarField zeroVert(xr.drop(0)); zeroVert = 0; ScalarField zeroHoriz(xr.drop(1)); zeroHoriz = 0;
  // phi[0].set_bound(1, 1, zeroHoriz); phi[0].set_bound(0, 1, zeroVert);
  
  // sets the velocity field in the flux function
  ptrCF->set_parameter(u0);
  
  // plots the various fields and domain
  fstream init;
  init.open("Results/Flume2D_initial/phi0.tsv",ios::out);
  phi[0].write_in_file_matrixform(init);
  fstream speed;
  speed.open("Results/Flume2D_initial/speed.tsv",ios::out);
  write_VectorField(u0, pos, speed);
  fstream b;
  b.open("Results/Flume2D_initial/boundary.tsv",ios::out);
  bound.write_in_file_matrixform(b);
  fstream f;
  f.open("Results/Flume2D_initial/convection_flux.tsv",ios::out);
  VectorField jac(2);
  jac[0] = ptrCF->get_max_eigenvalue(phi, 0);
  jac[1] = ptrCF->get_max_eigenvalue(phi, 1);
  write_VectorField(jac, pos, f);

  double dt = 0.005; double T = 100;
  RK3Solver ts(dt, T, &sol, phi);
  ts.get_solution("Flume2D1eqSource",1);

  return 0;
}
