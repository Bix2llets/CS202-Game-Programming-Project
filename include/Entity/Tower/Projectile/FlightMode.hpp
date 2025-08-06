#pragma once

class Tower;
class Projectile;

class FlightMode {
public:
    virtual ~FlightMode() = default;
    virtual void setUp(Projectile* projectile) = 0;
    virtual void update(Projectile* projectile) = 0;
};

class LinearFlightMode : public FlightMode {
public:
    void setUp(Projectile* projectile) override;
    void update(Projectile* projectile) override;
};
