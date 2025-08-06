#include "Entity/Tower/Projectile/FlightMode.hpp"
#include "Entity/Tower/Projectile/Projectile.hpp"

void LinearFlightMode::setUp(Projectile* projectile) {
    // Initialize linear flight mode settings
    projectile->velocity = (projectile->getTargetLocation() - projectile->getPosition()).normalized();
    projectile->velocity *= projectile->getSpeed();
}

void LinearFlightMode::update(Projectile* projectile) {
    // Update linear flight behavior
    if (!projectile->isFlying()) return;
    if (projectile->getTargetEntity() != nullptr) setUp(projectile);

    projectile->setPosition(projectile->getPosition() + projectile->getVelocity());
}
