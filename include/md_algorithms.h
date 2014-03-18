#ifndef __MD_ALGORITHMS_H
#define __MD_ALGORITHMS_H

#include <md_types.h>
#include <math.h>
#include <stdlib.h>

void Lennard_Jones( double r, double epsilon, double sigma, double& force, double& potential );
void verlet( Molecule& mol, double dt );
void euler( Molecule& mol, double dt );
void periodic( Molecule& mol, double3 area_size );
double distance( Molecule& mol1, Molecule& mol2 );
void simple_interact( Molecule& mol1, Molecule& mol2 );


void verlet_step( std::vector<Molecule>& molecules, double dt );
void euler_step( std::vector<Molecule>& molecules, double dt );

#endif