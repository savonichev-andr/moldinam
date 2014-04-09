#include <md_algorithms.h>

void Lennard_Jones( double r, double epsilon, double sigma, double& force, double& potential ) {
    double ri = 1 / r;
    double ri3 = ri * ri * ri;
    double ri6 = ri3 * ri3;
    
    force = 48 * epsilon * ( pow( sigma, 12 ) * ri6 - pow( sigma, 6 ) / 2 ) * ri6 * ri * ri;
    potential = 4 * epsilon * ri6 * ( ri6 * pow( sigma, 12 ) - pow( sigma, 6 ) );
}

void verlet( Molecule& mol, double dt ) {
    mol.pos_prev.x = 2 * mol.pos.x - mol.pos_prev.x + mol.accel.x * dt * dt;                            
    mol.pos_prev.y = 2 * mol.pos.y - mol.pos_prev.y + mol.accel.y * dt * dt;                            
    mol.pos_prev.z = 2 * mol.pos.z - mol.pos_prev.z + mol.accel.z * dt * dt;                            

    std::swap( mol.pos_prev, mol.pos );
}

void euler( Molecule& mol, double dt ) {
    std::swap( mol.pos_prev, mol.pos );

    mol.pos.x = mol.pos_prev.x + mol.speed.x * dt;
    mol.pos.y = mol.pos_prev.y + mol.speed.y * dt;
    mol.pos.z = mol.pos_prev.z + mol.speed.z * dt;
}

void periodic( Molecule& mol, double3 area_size ) {
    if ( mol.pos.x < 0 ) {
        mol.pos.x = area_size.x - abs( fmod( mol.pos.x, area_size.x ) );
    }
    if ( mol.pos.y < 0 ) {
        mol.pos.y = area_size.y - abs( fmod( mol.pos.y, area_size.y ) );
    }
    if ( mol.pos.z < 0 ) {
        mol.pos.z = area_size.z - abs( fmod( mol.pos.z, area_size.z ) );
    }


    if ( mol.pos.x > area_size.x ) {
        mol.pos.x = abs( fmod( mol.pos.x, area_size.x ) );
    }
    if ( mol.pos.y > area_size.y ) {
        mol.pos.y = abs( fmod( mol.pos.y, area_size.y ) );
    }
    if ( mol.pos.z > area_size.z ) {
        mol.pos.z = abs( fmod( mol.pos.z, area_size.z ) );
    }
}

void periodic( std::vector<Molecule>& molecules, double3 area_size ) {
    for ( Molecule& mol : molecules ) {
        periodic( mol, area_size );
    }
}

double distance( Molecule& mol1, Molecule& mol2 ) {
    double dx = mol1.pos.x - mol2.pos.x;
    double dy = mol1.pos.y - mol2.pos.y;
    double dz = mol1.pos.z - mol2.pos.z;

    return sqrt( dx*dx + dy*dy + dz*dz );
}

void simple_interact( Molecule& mol1, Molecule& mol2 ) {
    double r = distance( mol1, mol2 );
    double force_scalar = 0;
    double potential = 0;
    Lennard_Jones( r, 1, 1, force_scalar, potential );
    
    double3 force_vec { mol1.pos.x - mol2.pos.x, mol1.pos.y - mol2.pos.y, mol1.pos.z - mol2.pos.z };
    force_vec.x = force_vec.x * force_scalar / r;
    force_vec.y = force_vec.y * force_scalar / r;
    force_vec.z = force_vec.z * force_scalar / r;

    mol1.accel.x += force_vec.x;
    mol1.accel.y += force_vec.y;
    mol1.accel.z += force_vec.z;

    mol2.accel.x -= force_vec.x;
    mol2.accel.y -= force_vec.y;
    mol2.accel.z -= force_vec.z;
}
void verlet_step( std::vector<Molecule>& molecules, double dt ) {
    for ( Molecule& i : molecules ) {
        i.accel.x = i.accel.y = i.accel.z = 0;
    }

    for ( unsigned int i = 0; i < molecules.size() - 1; i++ ) {
        for ( unsigned int j = i + 1; j < molecules.size(); j++ ) {
            simple_interact( molecules[i], molecules[j] );
        }
    }
    
    for ( Molecule& i : molecules ) {
        verlet( i, dt );
    }
}
void euler_step( std::vector<Molecule>& molecules, double dt ) {
    for ( Molecule& i : molecules ) {
        i.accel.x = i.accel.y = i.accel.z = 0;
    }

    for ( unsigned int i = 0; i < molecules.size() - 1; i++ ) {
        for ( unsigned int j = i + 1; j < molecules.size(); j++ ) {
            simple_interact( molecules[i], molecules[j] );
        }
    }
    
    for ( Molecule& i : molecules ) {
        euler( i, dt );
    }
}
